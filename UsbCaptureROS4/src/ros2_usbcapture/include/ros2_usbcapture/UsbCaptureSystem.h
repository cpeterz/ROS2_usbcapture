// UsbCaptureSystem.h
#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <functional>
#include <GxIAPI.h>
#include <DxImageProc.h>
#include <opencv2/opencv.hpp>
#include "common/common.h"
#include "rclcpp/rclcpp.hpp"


/*
 * UsbCaptureSystem是2022赛季WMJ战队编写用来管理大恒水星相机的模块。
 * 以可以自定义的std::string orientation作为统一标识对相机和所得图像进行管理。
 * 需要opencv库
 * 需要大恒水星相机库
 * 需要wmj的Base库
 */
namespace wmj
{
    // UsbParam与旧代码基本保持一致。删改了一些
    struct UsbParam
    {
        float exposure = 4000;
        float gain = 10.0;
        float wb_red = 1.6484;
        float wb_blue = 1.000;
        float wb_green = 1.5664;
        void readParam(std::string path, std::string mode = "normal", std::string exposure_mode = "middle");
    };
    // FrameData用于记录相机RGB缓冲数据信息。但不负责管理RGB数据的内存。每个成员都有默认值。
    struct FrameData
    {
        // std::string orientation = "NONE";
        int frame_id = -1;
        double time_stamp = -1;
        int width = -1;
        int height = -1;
        int data_size = -1;
        void *p_data = NULL;
    };
    //相机的颜色通道枚举
    typedef enum
    {
        RED,
        BLUE,
        GREEN,
        ALL
    } COLOR;
    //相机系统管理类。可同时管理多个相机
    class UsbCaptureSystem
    {
    private:
        //可见设备数。在打开设备时数据更新。
        uint32_t m_device_num = 0;
        // orientation对应的GxI设备句柄。在打开设备时，以下这些map中的相机orientation对应的key被初始化
        std::map<std::string, GX_DEV_HANDLE> m_handles;
        // orientation对应时候开启硬触发，硬触发的设置见yaml
        std::map<std::string, bool> m_triggers;
        //用于存放RGB图像数据的缓冲信息
        std::map<std::string, FrameData> m_buffers;
        //缓冲区是否被读取过的标识。是在新图像被解码完毕时，用来决定是否释放旧图像内存的。
        std::map<std::string, FrameData> m_image_in_use;
        std::map<std::string, bool> m_get_flags;
        std::map<std::string, int> m_frame_freq;
        std::map<std::string, double> m_frame_time;
        std::map<std::string, std::mutex> m_mutex;
        UsbParam param;
        //临时变量，来自于wmj::now(),frame的图像时间戳来自大恒相机
        std::map<std::string, double> m_last_frame_times_tamp;

    private:
        // private成员函数包括了所有目前认为有使用意义的大恒相机功能。没有仔细阅读大恒官方文档前不建议更改。
        //按枚举打开所有相机，并按"1"、"2"...命名orientation。trigger为true则以触发模式打开。
        int enumOpenDevice(bool enable_trigger = false);
        //按指定序列号打开一个相机，并将其命名为指定orientation。trigger为true则以触发模式打开。
        int serialOpenDevice(std::string serial_number, std::string orientation, bool enable_trigger = false);
        //取得相机序列号。以下几个函数用handle作为参数，避免取到map中不存在的orientation后来这里找问题。
        std::string getSerialNumber(GX_DEV_HANDLE handle);

        //一些或许可以在打开相机之后也可以设置的图像参数
        int setExposureTime(GX_DEV_HANDLE handle, int value);
        int setWhiteBalance(GX_DEV_HANDLE handle, float value, COLOR channel);
        int setGain(GX_DEV_HANDLE handle, float value, COLOR channel);
        int setContrast(GX_DEV_HANDLE handle, int value);
        int setSharpness(GX_DEV_HANDLE handle, float value);

        // DQBuf-QBuf循环是大恒提供的读图方法。这是当前版本的图像流主函数，应以多线程调用此循环函数。这个没法以handle作为参数。
        void DQBufLoop(std::string orientation);
        //硬触发使用的图像流读取方法，硬触发是通过回调函数来获取图像指针的
        void CallbackLoop(std::string orientation);
        //获得指定orientation的图像。因为只传递指针所以比旧代码快得多。
        int getImage(cv::Mat &frame, std::string orientation);
        int StopTrigger(GX_DEV_HANDLE hDevice);
        

    public:
        //一些允许被外部访问，且只影响public成员函数、与private成员函数无关的变量
        std::string m_config_path;
    public:
        // public成员函数除了构造、析构以外都是纯封装，不涉及任何核心功能。如有需要时可以按需求修改。
        //默认构造函数
        UsbCaptureSystem();
        //这个构造函数包括取图片前的所有操作，除了跟默认构造函数一样的部分以外，也都可以按需求修改。
        UsbCaptureSystem(std::string config_path, bool enable_trigger = false);
        ~UsbCaptureSystem();

        //输出相机的orientation和序列号。
        void getCameraInfo(std::string orientation = "all");
        //无需前置条件地打开相机并打开图像流。所以不需要序列号。单目如果直接用这个，那orientation就是 "1"，可以直接读图。
        void blindOpenDevice();
        //按序列号打开相机并打开图像流。
        void setSerialOpenDevice(std::string serial_number, std::string orientation, bool m_trigger);
        //在cv::namedWindow里显示所有相机的当前图像。需要先打开相机
        void displayAllCameras();

        //与旧代码基本一致的接口。单独设置图像参数
        int setExposureTime(int value, std::string orientation = "all");
        int setWhiteBalance(COLOR channel, float value, std::string orientation = "all");
        int setGain(COLOR channel, float value, std::string orientation = "all");
        int setSharpness(float value, std::string orientation = "all");
        int setContrast(int value, std::string orientation = "all");
        //按yaml文件设置参数。要求m_config_path有效。
        void cameraMode(std::string mode, std::string exposure_mode = "middle", std::string orientation = "all");
        //按yaml文件打开相机。注意：与旧yaml格式不兼容，需要先修改yaml。要求m_config_path有效。
        void loadSerialOpenDevice();
        float getExposureTimeParam();

        //取得可见的相机数目
        int totalCameraCount() { return m_device_num; };
        //取得已经打开的相机数目
        int activeCameraCount() { return m_handles.size(); };
        //取得已经打开的相机的orientation。无序排列。
        std::vector<std::string> activeCameraName();

        //读图接口。根据兵种不同可以有所不同，改起来很方便
        //读取MatWithTime里指定的orientation的图像和信息，存入MatWithTime。需要先设置MatWithTime里的orientation
        int operator>>(wmj::MatWithTime &frame);
        //双目读取left和right。
        int operator>>(std::vector<wmj::MatWithTime> &frames);
        //重置时间戳
        int timeStampReset();
        //查看相机的电平状态
        int getIOStatus(std::string, int64_t &);
        int getFrameFreq(std::string,int &);
        int getFrameTime(std::string,double &);
        friend void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);
    };
    
}

