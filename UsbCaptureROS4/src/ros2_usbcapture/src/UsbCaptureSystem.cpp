// UsbCaptureSystem.cpp
#include "ros2_usbcapture/UsbCaptureSystem.h"
namespace wmj
{
    //读取参数
    void UsbParam::readParam(std::string path, std::string mode, std::string exposure_mode)
    {
        cv::FileStorage fs(path, cv::FileStorage::READ);
        if (mode == "armor" || mode == "dark" || mode == "lob")
            fs["exposure"][exposure_mode] >> this->exposure;
        else
            fs["exposure"][mode] >> this->exposure;

        fs["gain"][mode] >> this->gain;
        fs["whitebalance"]["wb_red"] >> this->wb_red;
        fs["whitebalance"]["wb_green"] >> this->wb_green;
        fs["whitebalance"]["wb_blue"] >> this->wb_blue;
        fs.release();
    }
    //构造和析构函数
    UsbCaptureSystem::UsbCaptureSystem()
    {
        std::cout << "[UCS]Usb Capture System initializing. Initialize without param.\n";
        GX_STATUS status = GX_STATUS_SUCCESS;
        status = GXInitLib();
        status = GXUpdateDeviceList(&m_device_num, 1000);
        if (status != GX_STATUS_SUCCESS)
        {
            std::cout << _warning("[UCS]init lib error! Error code: " + std::to_string(status)) << std::endl;
            return;
        }
        std::cout << "[UCS]Camera count for open: " << m_device_num << std::endl;
        std::cout << "[UCS]initialized.\n";
    }
    UsbCaptureSystem::UsbCaptureSystem(std::string config_path, bool enable_trigger)
    {
        std::cout << "[UCS]Usb Capture System initializing. Params:" << std::endl;
        std::cout << "[UCS]config_path: " << config_path << std::endl;
        // std::cout << "[UCS]enable_trigger: " << enable_trigger << std::endl;
        GX_STATUS status = GX_STATUS_SUCCESS;
        status = GXInitLib();
        status = GXUpdateDeviceList(&m_device_num, 1000);
        if (status != GX_STATUS_SUCCESS)
        {
            std::cout << _warning("[UCS]init lib error! Error code: " + std::to_string(status)) << std::endl;
            return;
        }
        std::cout << "[UCS]Camera count for open: " << m_device_num << std::endl;
        m_config_path = config_path;
        loadSerialOpenDevice();
        cameraMode("normal");
        std::cout << "[UCS]initialized.\n";
    }
    // private函数
    //按枚举打开所有相机
    //暂时默认以软触发打开
    int UsbCaptureSystem::enumOpenDevice(bool enable_trigger)
    {
        std::cout << "[UCS]Enum open device...\n";
        GX_STATUS status = GX_STATUS_SUCCESS;
        GX_OPEN_PARAM open_param;
        open_param.accessMode = GX_ACCESS_EXCLUSIVE;
        open_param.openMode = GX_OPEN_INDEX;
        //逐个打开相机
        for (int i_device = 0; i_device < m_device_num; i_device++)
        {
            //为相机按编号命名并打开相机
            std::string orientation = std::to_string(i_device + 1);
            open_param.pszContent = (char *)orientation.data();
            GX_DEV_HANDLE handle;
            status = GXOpenDevice(&open_param, &handle);
            if (status != GX_STATUS_SUCCESS)
            {
                std::cout << _warning("[UCS]Open camera " + orientation + " error! Error code: " + std::to_string(status) + " Continue.\n");
                FrameData init_data;
                m_buffers[orientation] = init_data;
                m_image_in_use[orientation] = init_data;
                continue;
            }
            status = GXSetEnum(handle, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
            if (status == GX_STATUS_SUCCESS)
            {
                //初始化各map
                // FrameData每个成员都有默认值。直接赋值给map问题不大
                FrameData init_data;
                m_handles[orientation] = handle;
                m_buffers[orientation] = init_data;
                m_image_in_use[orientation] = init_data;
                //设为true是防止程序尝试释放FrameData里默认的NULL。第一帧解码完成后会设为false
                m_get_flags[orientation] = true;
                std::cout << "[UCS]Open camera " << orientation << " success!\n";
            }
            else
            {
                //这里不直接return，而是跳过这个相机
                std::cout << _warning("[UCS]Open camera " + orientation + "error! Error code: " + std::to_string(status) + ". Continue.\n");
                FrameData init_data;
                m_buffers[orientation] = init_data;
                m_image_in_use[orientation] = init_data;
                continue;
            }
        }
        return 0;
    }
    //按序列号打开指定相机
    int UsbCaptureSystem::serialOpenDevice(std::string serial_number, std::string orientation, bool enable_trigger)
    {
        //将相机命名为指定orientation并打开
        GX_STATUS status = GX_STATUS_SUCCESS;
        GX_OPEN_PARAM open_param;
        open_param.accessMode = GX_ACCESS_EXCLUSIVE;
        open_param.openMode = GX_OPEN_SN;
        open_param.pszContent = (char *)serial_number.data();
        GX_DEV_HANDLE handle;
        status = GXOpenDevice(&open_param, &handle); // bug maybe exists 可能触发模式无法使用
        if (status != GX_STATUS_SUCCESS)
        {
            std::cout << _warning("[UCS]Open camera " + orientation + " error! Error code: " + std::to_string(status)) << std::endl;
            FrameData init_data;
            m_buffers[orientation] = init_data;
            m_image_in_use[orientation] = init_data;
            return status;
        }
        // 触发模式设置
        if (enable_trigger)
        {
            //设置触发激活方式为硬触发
            status = GXSetEnum(handle, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
            //设置触发激活方式为上升沿还可以设置下降沿触发，指电平上升或下降时为一次触
            status = GXSetEnum(handle, GX_ENUM_TRIGGER_ACTIVATION, GX_TRIGGER_ACTIVATION_RISINGEDGE);
            //设置触发开关为line2
            status = GXSetEnum(handle, GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_LINE2);
            //我也不知道是啥反正加去了，Selects the physical line (or pin) of the external device connector to
            status = GXSetEnum(handle, GX_ENUM_LINE_SELECTOR, GX_ENUM_LINE_SELECTOR_LINE2);
            //设置 line2 模式为 input ，同样可以设置为 outpot ，则可以通过相机控制外部设备
            status = GXSetEnum(handle, GX_ENUM_LINE_MODE, GX_ENUM_LINE_MODE_INPUT);
            status = GXCloseDevice(handle);
            status = GXOpenDevice(&open_param, &handle);
            if (status != GX_STATUS_SUCCESS)
            {
                std::cout << _warning("set trigger mode failed") << std::endl;
                std::cout << _warning("[UCS]Open camera " + orientation + " error! Error code: " + std::to_string(status)) << std::endl;
            }
            else
            {
                std::cout << "set trigger mode successfully" << std::endl;
            }
        }
        else
        {
            status = GXSetEnum(handle, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
        }
        if (status == GX_STATUS_SUCCESS)
        {
            FrameData init_data;
            m_handles[orientation] = handle;
            m_buffers[orientation] = init_data;
            m_image_in_use[orientation] = init_data;
            m_triggers[orientation] = enable_trigger;
            // true的理由同上个函数
            m_get_flags[orientation] = true;
            std::cout << "[UCS]Open camera " << orientation << " success!\n";
            return 0;
        }
        else
        {
            std::cout << _warning("[UCS]Open camera " + orientation + " error! Error code: " + std::to_string(status)) << std::endl;
            FrameData init_data;
            m_buffers[orientation] = init_data;
            m_image_in_use[orientation] = init_data;
            return status;
        }
    }

    //通过handle重置时间戳，这里是所有相机时间戳重置
    int UsbCaptureSystem::timeStampReset()
    {
        GX_STATUS status = GX_STATUS_SUCCESS;
        for (auto pairs : m_handles)
        {
            status = GXSendCommand(pairs.second, GX_COMMAND_TIMESTAMP_RESET);
            if (status != GX_STATUS_SUCCESS)
            {
                std::cout << _warning("Reset timestamp failed") << std::endl;
                std::cout << _warning("[UCS]Open camera " + pairs.first + " error! Error code: " + std::to_string(status)) << std::endl;
                return status;
            }
        }
        return 0;
    }
    int UsbCaptureSystem::getIOStatus(std::string orientation, int64_t &nAllLineStatus)
    {
        GX_STATUS status = GX_STATUS_SUCCESS;
        status = GXGetInt(m_handles[orientation], GX_INT_LINE_STATUS_ALL, &nAllLineStatus);
        if (status != GX_STATUS_SUCCESS)
        {
            std::cout << _warning("Reset timestamp failed") << std::endl;
            std::cout << _warning("[UCS]Open camera " + orientation + " error! Error code: " + std::to_string(status)) << std::endl;
            return status;
        }
        return 0;
    }
    //获得指定相机的序列号。所有需要handle的函数都必须在打开相机之后调用。
    std::string UsbCaptureSystem::getSerialNumber(GX_DEV_HANDLE handle)
    {
        GX_STATUS status = GX_STATUS_SUCCESS;
        size_t nSize = 0;
        status = GXGetStringMaxLength(handle, GX_STRING_DEVICE_SERIAL_NUMBER, &nSize);
        char *pszText = new char[nSize];
        status = GXGetString(handle, GX_STRING_DEVICE_SERIAL_NUMBER, pszText, &nSize);
        std::string serial_number(pszText);
        delete[] pszText;
        return serial_number;
    }
    //曝光
    int UsbCaptureSystem::setExposureTime(GX_DEV_HANDLE handle, int value)
    {
        GX_STATUS status = GX_STATUS_SUCCESS;
        status = GXSetFloat(handle, GX_FLOAT_EXPOSURE_TIME, (float)value);
        return status;
    }
    //白平衡
    int UsbCaptureSystem::setWhiteBalance(GX_DEV_HANDLE handle, float value, COLOR channel)
    {
        GX_STATUS status = GX_STATUS_SUCCESS;
        GX_BALANCE_RATIO_SELECTOR_ENTRY entry;
        switch (channel)
        {
        case RED:
            entry = GX_BALANCE_RATIO_SELECTOR_RED;
            break;
        case GREEN:
            entry = GX_BALANCE_RATIO_SELECTOR_GREEN;
            break;
        case BLUE:
            entry = GX_BALANCE_RATIO_SELECTOR_BLUE;
            break;
        default:
            std::cout << "[UCS]White balance channel invalid\n";
            break;
        }
        status = GXSetEnum(handle, GX_ENUM_BALANCE_RATIO_SELECTOR, entry);
        status = GXSetFloat(handle, GX_FLOAT_BALANCE_RATIO, value);
        return status;
    }
    //增益
    int UsbCaptureSystem::setGain(GX_DEV_HANDLE handle, float value, COLOR channel)
    {
        GX_STATUS status = GX_STATUS_SUCCESS;
        GX_GAIN_SELECTOR_ENTRY entry;
        switch (channel)
        {
        case RED:
            entry = GX_GAIN_SELECTOR_RED;
            break;
        case GREEN:
            entry = GX_GAIN_SELECTOR_GREEN;
            break;
        case BLUE:
            entry = GX_GAIN_SELECTOR_BLUE;
            break;
        case ALL:
            entry = GX_GAIN_SELECTOR_ALL;
            break;
        default:
            std::cout << "[UCS]Gain channel invalid\n";
            break;
        }
        status = GXSetEnum(handle, GX_ENUM_GAIN_SELECTOR, entry);
        status = GXSetFloat(handle, GX_FLOAT_GAIN, value);
        return status;
    }
    //对比度
    int UsbCaptureSystem::setContrast(GX_DEV_HANDLE handle, int value)
    {
        GX_STATUS status = GX_STATUS_SUCCESS;
        status = GXSetInt(handle, GX_INT_CONTRAST_PARAM, value);
        return status;
    }
    //锐度
    int UsbCaptureSystem::setSharpness(GX_DEV_HANDLE handle, float value)
    {
        GX_STATUS status = GX_STATUS_SUCCESS;
        if (value == 0)
        {
            status = GXSetEnum(handle, GX_ENUM_SHARPNESS_MODE, GX_SHARPNESS_MODE_OFF);
            return status;
        }
        status = GXSetEnum(handle, GX_ENUM_SHARPNESS_MODE, GX_SHARPNESS_MODE_ON);
        status = GXSetFloat(handle, GX_FLOAT_SHARPNESS, value);
        return status;
    }

    // DQBuf-QBuf循环。图像流主程序。理论上比GetImage方法快，因为不用复制。
    //理论上比Callback慢，因为多个图像解码不能并行(我也不知道回调函数能不能并行)。不用Callback方法是因为当时我没搞定类成员回调,搞定之后已经来不及了。
    void UsbCaptureSystem::DQBufLoop(std::string orientation)
    {
        std::cout << "[UCSLoop]Try start DQBufLoop-" << orientation << std::endl;
        if (m_handles.count(orientation) != 1)
            return;
        bool camera_fall_down = false;
        FrameData data;
        GX_STATUS status = GX_STATUS_SUCCESS;
        status = GXStreamOn(m_handles[orientation]);
        if (status != GX_STATUS_SUCCESS)
        {
            std::cout << _warning("[UCSLoop]Start camera stream failed. Error code: " + std::to_string(status)) << std::endl;
            return;
        }
        // raw是一种叫做bayer格式的图像的后缀名。bayer格式广泛用于工业相机。
        PGX_FRAME_BUFFER raw_buffer;
        int64_t payloadSize = 0;
        status = GXGetInt(m_handles[orientation], GX_INT_PAYLOAD_SIZE, &payloadSize);
        //循环地读取、解码、保存数据
        std::cout << "[UCSLoop]DQBufLoop-" << orientation << " begin successfully.\n";
        int i_frame = 1;
        bool cal_freq = false;
        double LastFrameTime = wmj::now();
        while (i_frame++)
        {
            // DQBuf是取相机内缓冲区。大恒默认缓冲区有五个，所以在不进行QBuf时，最多取五次。
            status = GXDQBuf(m_handles[orientation], &raw_buffer, 1000);
            if (status != GX_STATUS_SUCCESS)
            {
                std::cout << _warning("[UCSLoop]At loop frame " + std::to_string(i_frame) + ", " + orientation + " get frame failed.\n");
                std::cout << _warning("[UCSLoop]Error code: " + std::to_string(status) + ". Now DQBufLoop interrupted.") << std::endl;
                camera_fall_down = true;
                break;
            }
            double cur_time = wmj::now();
            if (cal_freq)
            {
                std::cout << "SoftFps:" << 1 / (cur_time - LastFrameTime) << std::endl;
            }
            if (raw_buffer->nStatus != GX_FRAME_STATUS_SUCCESS)
            {
                std::cout << _warning("[UCSLoop]At loop frame " + std::to_string(i_frame) + ", " + orientation + " frame error.\n");
                std::cout << _warning("[UCSLoop]Error code: " + std::to_string(raw_buffer->nStatus)) << std::endl;
                usleep(10000);
                continue;
            }
            //如果成功取到了bayer图像，就新分配一段内存保存解码得到的rgb图像，作为rgb缓冲区
            void *rgb_buffer = malloc(3 * raw_buffer->nImgSize);
            DxRaw8toRGB24(raw_buffer->pImgBuf, rgb_buffer, raw_buffer->nWidth, raw_buffer->nHeight, RAW2RGB_NEIGHBOUR, BAYERRG, false);
            //将图像信息记录下来
            data.frame_id = raw_buffer->nFrameID;
            data.time_stamp = cur_time;
            data.p_data = rgb_buffer;
            data.data_size = 3 * raw_buffer->nImgSize;
            data.width = raw_buffer->nWidth;
            data.height = raw_buffer->nHeight;
            //归还相机内缓冲区
            GXQBuf(m_handles[orientation], raw_buffer);

            //新图像准备完毕，可以切换数据指针。
            //读取旧图像数据的指针
            void *ptemp = m_buffers[orientation].p_data;
            //互斥锁，避免多线程同时处理一段内存，也避免一些逻辑错误
            m_mutex[orientation].lock();
            //更新缓冲数据，让指针指向新的rgb数据，并更新图像信息
            m_buffers[orientation] = data;
            //如果旧的rgb数据没被读取过，释放它。如果被读取过，则内存不再由DQBuf循环管理，而是读图时在m_image_in_use中管理。
            if (!m_get_flags[orientation])
                free(ptemp);
            //将新图像状态设为“没被读取过”
            m_get_flags[orientation] = false;
            m_mutex[orientation].unlock();
            if (cal_freq)
                LastFrameTime = cur_time;
            // long long int time_stamp = cur_time;
            // std::cout << "time_stamp" << time_stamp << std::endl;
            
        }
        if (camera_fall_down)
        {
            std::cout << "[UCSLoop]Camera " << orientation << " offline. Frames got by user would be empty from then on.\n";
            m_buffers[orientation].p_data = NULL;
        }
    }
    struct ClassAndOrientation
    {
        UsbCaptureSystem *This;
        std::string orientation;
        int FrameCount = 0;
        double LastFrameTime = 0;
        bool cal_freq = false;
    };
    void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
    {
        ClassAndOrientation *CAO = (ClassAndOrientation *)(pFrame->pUserParam);
        CAO->FrameCount++;
        std::string orientation = CAO->orientation;
        if (pFrame->status == 0)
        {
            FrameData data;
            //如果成功取到了bayer图像，就新分配一段内存保存解码得到的rgb图像，作为rgb缓冲区
            void *rgb_buffer = malloc(3 * pFrame->nImgSize);
            DxRaw8toRGB24(const_cast<void *>(pFrame->pImgBuf), rgb_buffer, pFrame->nWidth, pFrame->nHeight, RAW2RGB_NEIGHBOUR, BAYERRG, false);
            //将图像信息记录下来
            data.frame_id = CAO->FrameCount;
            data.time_stamp = wmj::now();
            data.p_data = rgb_buffer;
            data.data_size = 3 * pFrame->nImgSize;
            data.width = pFrame->nWidth;
            data.height = pFrame->nHeight;
            //新图像准备完毕，可以切换数据指针。
            //读取旧图像数据的指针
            void *ptemp = CAO->This->m_buffers[orientation].p_data;
            //互斥锁，避免多线程同时处理一段内存，也避免一些逻辑错误
            CAO->This->m_mutex[orientation].lock();
            //更新缓冲数据，让指针指向新的rgb数据，并更新图像信息
            CAO->This->m_buffers[orientation] = data;
            //如果旧的rgb数据没被读取过，释放它。如果被读取过，则内存不再由DQBuf循环管理，而是读图时在m_image_in_use中管理。
            if (!CAO->This->m_get_flags[orientation])
                free(ptemp);
            //将新图像状态设为“没被读取过”
            CAO->This->m_get_flags[orientation] = false;
            CAO->This->m_mutex[orientation].unlock();
            if (CAO->cal_freq == true)
            {
                std::cout << "[UCS]Inner FPS :" + orientation << 1 / (-CAO->LastFrameTime + data.time_stamp) << std::endl;
            }
            CAO->LastFrameTime = data.time_stamp; //开始时间
        }
        else
        {
            std::cout << _warning("callback frame wrong") << std::endl;
            std::cout << _warning("[UCS]Open camera " + orientation + " error! Error code: " + std::to_string(pFrame->status)) << std::endl;
        }
        return;
    }

    void UsbCaptureSystem::CallbackLoop(std::string orientation)
    {
        std::cout << "[UCSLoop]Try start CallbackLoop-" << orientation << std::endl;
        if (m_handles.count(orientation) != 1)
            return;
        //发 送 开 采 命 令
        GX_STATUS status = GX_STATUS_SUCCESS;

        ClassAndOrientation CAO;
        CAO.This = this;
        CAO.orientation = orientation;
        CAO.LastFrameTime = wmj::now();
        CAO.cal_freq = false;
        // ClassAndOrientation* CAO = new ClassAndOrientation(this,orientation);
        // status = GXRegisterCaptureCallback(m_handles[orientation], (void*)CAO, OnFrameCallbackFun);
        status = GXRegisterCaptureCallback(m_handles[orientation], (void *)&CAO, OnFrameCallbackFun);
        status = GXSendCommand(m_handles[orientation], GX_COMMAND_ACQUISITION_START);
        if (status != GX_STATUS_SUCCESS)
        {
            std::cout << _warning("[UCSLoop]Camera " + orientation + " Start camera callback stream failed.");
            std::cout << _warning("[UCSLoop]Error code: " + std::to_string(status)) << std::endl;
            return;
        }
        while (1)
        {
            //不可终止，否则CAO消失
        }
    }
    int UsbCaptureSystem::getImage(cv::Mat &frame, std::string orientation)
    {
        int timeout = 0;
        //如果图像没有更新，等一会。如果都是NULL，就不用等。因为NULL在图像循环流中意味着图像读取失败，在此读图函数中被视为一张空图像并返回
        while (m_image_in_use[orientation].p_data == m_buffers[orientation].p_data && m_image_in_use[orientation].p_data != NULL)
        {
            usleep(500);
            timeout += 500;
            if (timeout > 3000000)
            {
                cv::Mat empty;
                frame = empty;
                return -1;
            }
        }
        m_mutex[orientation].lock();
        m_get_flags[orientation] = true;
        //保存旧的被读图像的地址
        void *ptemp = m_image_in_use[orientation].p_data;
        //取得当前缓冲区的图像
        m_image_in_use[orientation] = m_buffers[orientation];
        m_mutex[orientation].unlock();
        //释放旧图。
        if (ptemp != NULL)
        {
            free(ptemp);
            ptemp = NULL;
        }
        //构造并返回Mat
        if (m_image_in_use[orientation].p_data != NULL)
            frame = cv::Mat(m_image_in_use[orientation].height, m_image_in_use[orientation].width, CV_8UC3, m_image_in_use[orientation].p_data);
        else
        {
            cv::Mat empty;
            frame = empty;
        }
        return 0;
    }

    // public成员函数
    void UsbCaptureSystem::getCameraInfo(std::string orientation)
    {
        if (m_handles.empty())
        {
            std::cout << "[UCS]No camera opened\n";
            return;
        }
        if (orientation != "all")
        {
            std::cout << "[UCS]Camera orientation: " << orientation << std::endl;
            std::cout << "[UCS]Serial number:" << getSerialNumber(m_handles[orientation]) << std::endl;
        }
        else
        {
            for (auto pairs : m_handles)
            {
                std::cout << "[UCS]Camera orientation: " << pairs.first << std::endl;
                std::cout << "[UCS]Serial number:" << getSerialNumber(pairs.second) << std::endl;
            }
        }
    }
    void UsbCaptureSystem::blindOpenDevice()
    {
        enumOpenDevice(false);
        //把成功打开的相机都启动图像流。没打开的相机不在这个map里
        for (auto pairs : m_handles)
        {
            std::thread(std::bind(&UsbCaptureSystem::DQBufLoop, this, pairs.first)).detach();
        }
    }
    void UsbCaptureSystem::setSerialOpenDevice(std::string serial_number, std::string orientation, bool m_trigger)
    {
        if (serialOpenDevice(serial_number, orientation, m_trigger) == 0)
        {
            if (m_trigger)
            {
                std::thread(std::bind(&UsbCaptureSystem::CallbackLoop, this, orientation)).detach();
            }
            if (!m_trigger)
            {
                std::thread(std::bind(&UsbCaptureSystem::DQBufLoop, this, orientation)).detach();
            }
        }
        else
            std::cout << "[UCS]Open " << orientation << " failed\n";
    }
    void UsbCaptureSystem::displayAllCameras()
    {
        for (auto pairs : m_handles)
        {
            cv::namedWindow(pairs.first);
        }
        int i_frame = 0;
        while (++i_frame)
        {
            for (auto pairs : m_handles)
            {
                std::string orientation = pairs.first;
                cv::Mat frame;
                getImage(frame, orientation);
                if (frame.empty())
                    continue;
                cv::imshow(orientation, frame);
            }
            if (cv::waitKey(1) == 'q')
                return;
        }
    }
    int UsbCaptureSystem::setExposureTime(int value, std::string orientation)
    {
        if (orientation == "all")
        {
            // orientation为all时，遍历所有已经打开的相机并设置参数。如有有设置失败，中止并返回错误码
            GX_STATUS status = GX_STATUS_SUCCESS;
            for (auto pairs : m_handles)
            {
                status = setExposureTime(pairs.second, value);
                if (status != GX_STATUS_SUCCESS)
                    break;
            }
            return status;
        }
        else if (m_handles.count(orientation) == 1)
        {
            // orintation有效时，单独设置参数
            return setExposureTime(m_handles[orientation], value);
        }
        else
        {
            // orientation无效时，返回-1
            std::cout << "[UCS]No such orientation while setting exposure time: " << orientation << std::endl;
            return -1;
        }
    }
    int UsbCaptureSystem::setWhiteBalance(COLOR channel, float value, std::string orientation)
    {
        if (orientation == "all")
        {
            GX_STATUS status = GX_STATUS_SUCCESS;
            for (auto pairs : m_handles)
            {
                //此处无需考虑channel是否为all，只需考虑应该给哪一个设备设置参数。因为此调用的私有函数已经考虑这种情况
                status = setWhiteBalance(pairs.second, value, channel);
                if (status != GX_STATUS_SUCCESS)
                    break;
            }
            return status;
        }
        else if (m_handles.count(orientation) == 1)
        {
            return setWhiteBalance(m_handles[orientation], value, channel);
        }
        else
        {
            std::cout << "[UCS]No such orientation while setting white balance: " << orientation << std::endl;
            return -1;
        }
    }
    int UsbCaptureSystem::setGain(COLOR channel, float value, std::string orientation)
    {
        if (orientation == "all")
        {
            GX_STATUS status = GX_STATUS_SUCCESS;
            for (auto pairs : m_handles)
            {
                status = setGain(pairs.second, (int)value, ALL);
                if (status != GX_STATUS_SUCCESS)
                    break;
            }
            return status;
        }
        else if (m_handles.count(orientation) == 1)
        {
            return setGain(m_handles[orientation], (int)value, ALL);
        }
        else
        {
            std::cout << "[UCS]No such orientation while setting gain: " << orientation << std::endl;
            return -1;
        }
    }
    int UsbCaptureSystem::setSharpness(float value, std::string orientation)
    {
        if (orientation == "all")
        {
            GX_STATUS status = GX_STATUS_SUCCESS;
            for (auto pairs : m_handles)
            {
                status = setSharpness(pairs.second, value);
                if (status != GX_STATUS_SUCCESS)
                    break;
            }
            return status;
        }
        else if (m_handles.count(orientation) == 1)
        {
            return setSharpness(m_handles[orientation], value);
        }
        else
        {
            std::cout << "[UCS]No such orientation while setting sharpness: " << orientation << std::endl;
            return -1;
        }
    }
    int UsbCaptureSystem::setContrast(int value, std::string orientation)
    {
        if (orientation == "all")
        {
            GX_STATUS status = GX_STATUS_SUCCESS;
            for (auto pairs : m_handles)
            {
                status = setContrast(pairs.second, value);
                if (status != GX_STATUS_SUCCESS)
                    break;
            }
            return status;
        }
        else if (m_handles.count(orientation) == 1)
        {
            return setContrast(m_handles[orientation], value);
        }
        else
        {
            std::cout << "[UCS]No such orientation while setting contrast: " << orientation << std::endl;
            return -1;
        }
    }
    void UsbCaptureSystem::cameraMode(std::string mode, std::string exposure_mode, std::string orientation)
    {
        // UsbParam param;
        param.readParam(m_config_path, mode, exposure_mode);
        if (orientation == "all")
        {
            for (auto pairs : m_handles)
            {
                setExposureTime(pairs.second, param.exposure);
                setWhiteBalance(pairs.second, param.wb_blue, BLUE);
                setWhiteBalance(pairs.second, param.wb_red, RED);
                setWhiteBalance(pairs.second, param.wb_green, GREEN);
                setGain(pairs.second, param.gain, ALL);
            }
        }
        else if (m_handles.count(orientation) == 1)
        {
            setExposureTime(m_handles[orientation], param.exposure);
            setWhiteBalance(m_handles[orientation], param.wb_blue, BLUE);
            setWhiteBalance(m_handles[orientation], param.wb_red, RED);
            setWhiteBalance(m_handles[orientation], param.wb_green, GREEN);
            setGain(m_handles[orientation], param.gain, ALL);
        }
        else
        {
            std::cout << _warning("[UCS]Set camera mode error.\n");
            return;
        }
    }
    void UsbCaptureSystem::loadSerialOpenDevice()
    {
        cv::FileStorage fs(m_config_path, cv::FileStorage::READ);
        for (auto node : fs["cameras"])
        {
            std::string orientation = node["orientation"];
            std::string serial_number = node["serial_number"];
            int m_trigger = node["trigger"];
            std::cout << "[UCS]enable_trigger: " << m_trigger << std::endl;
            serialOpenDevice(serial_number, orientation, m_trigger);
        }
        for (auto pairs : m_handles)
        {
            if (m_triggers[pairs.first] == false)
                std::thread(std::bind(&UsbCaptureSystem::DQBufLoop, this, pairs.first)).detach();
            if (m_triggers[pairs.first] == true)
                std::thread(std::bind(&UsbCaptureSystem::CallbackLoop, this, pairs.first)).detach();
        }
        return;
    }

    float UsbCaptureSystem::getExposureTimeParam()
    {
        return param.exposure;
    }

    std::vector<std::string> UsbCaptureSystem::activeCameraName()
    {
        std::vector<std::string> names;
        for (auto camera : m_handles)
        {
            names.push_back(camera.first);
        }
        return names;
    }

    int UsbCaptureSystem::operator>>(MatWithTime &frame)
    {
        std::string orientation = frame.m_orientation;
        if (m_buffers.count(orientation) == 1)
        {
            if (getImage(frame.m_img, orientation) == -1)
                std::cout << "[UCS]Particular image update timeout!\n";
            //这个时间戳是大恒自带的。可能和现在用的不一样。
            frame.m_time_stamp = m_buffers[orientation].time_stamp;
            return 0;
        }
        else if (m_buffers.count("1") == 1)
        {
            HINT if (getImage(frame.m_img, "1") == -1)
                    std::cout
                << "[UCS]Single image update timeout!\n";
            frame.m_orientation = "single";
            frame.m_time_stamp = m_buffers["1"].time_stamp;
            return 0;
        }
        else
            return -1;
    }
    int UsbCaptureSystem::operator>>(std::vector<wmj::MatWithTime> &frames)
    {
        //检测frames大小是否为2
        if (frames.size() != 2)
            frames.resize(2);

        if (getImage(frames[0].m_img, "left") == -1)
            std::cout << "[UCS]Left image update timeout!\n";
        frames[0].m_orientation = "left";
        frames[0].m_time_stamp = m_buffers["left"].time_stamp;

        if (getImage(frames[1].m_img, "right") == -1)
            std::cout << "[UCS]Right image update timeout!\n";
        frames[1].m_orientation = "right";
        frames[1].m_time_stamp = m_buffers["right"].time_stamp;
        return 0;
    }
    int UsbCaptureSystem::StopTrigger(GX_DEV_HANDLE hDevice)
    {
        //停止开采
        GX_STATUS status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);
        //注 销 采 集 回 调
        status = GXUnregisterCaptureCallback(hDevice);
        //关闭设备
        status = GXCloseDevice(hDevice);
        hDevice = NULL;
        return 0;
    }
    int UsbCaptureSystem::getFrameFreq(std::string orientation, int &Freq)
    {
        Freq = m_frame_freq[orientation];
        return 0;
    }
    int UsbCaptureSystem::getFrameTime(std::string orientation, double &Time)
    {
        Time = m_frame_time[orientation];
        return 0;
    }
    UsbCaptureSystem::~UsbCaptureSystem()
    {
        for (auto orientation : m_handles)
        {
            if (m_triggers[orientation.first] == true)
            {
                StopTrigger(orientation.second);
            }
            else
            {
                GXCloseDevice(orientation.second);
            }
            GXCloseLib();
        }
    }

}