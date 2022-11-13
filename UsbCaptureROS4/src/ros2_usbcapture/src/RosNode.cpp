#include "ros2_usbcapture/RosNode.h"

namespace wmj
{
    camera_node::camera_node(const rclcpp::NodeOptions &options) : rclcpp::Node("camera_node", options), count_(0)
    {
        //打开相机,并获取参数
        capture = std::make_shared<wmj::UsbCaptureSystem>(USBCAPTURE_CFG);
        camera_num = capture->activeCameraCount();
        std::cout << "Active camera num: " << camera_num << std::endl;
        capture->cameraMode("armor");
        capture->getCameraInfo();
        frame.resize(1);
        frame[0].m_orientation = "single";

        RCLCPP_INFO(this->get_logger(), "camera_node had successfully opened the camera!");

        //创建发布者
        pub_image = this->create_publisher<interfaces::msg::MatWithTime>("camera_image", 10);
        //创建发布函数，10ms一张
        timer_ = this->create_wall_timer(std::chrono::milliseconds(10), std::bind(&camera_node::timer_call_back, this));
    }

    void camera_node::timer_call_back()
    {
        //利用成员变量 capture 读图
        if ((*capture >> frame[0]) != 0)
        {
            std::cout << "get images wrong!!!\n";
        }
        if (frame[0].m_img.empty())
        {
            cv::Mat empty(1024, 1280, CV_8UC3, cv::Scalar::all(0));
            frame[0].m_img = empty;
            cv::putText(frame[0].m_img, "No Image", cv::Point(400, 400), cv::FONT_HERSHEY_COMPLEX, 3, cv::Scalar::all(255), 1);
            cv::putText(frame[0].m_img, "This frame", cv::Point(350, 500), cv::FONT_HERSHEY_COMPLEX, 3, cv::Scalar::all(255), 1);
            cv::putText(frame[0].m_img, "is empty!", cv::Point(350, 600), cv::FONT_HERSHEY_COMPLEX, 3, cv::Scalar::all(255), 1);
        }

        //创建发布消息
        auto msg = std::make_unique<interfaces::msg::MatWithTime>();

        //类型转换，将 cv::Mat 转换为 sensor_msgs 下的 image
        auto img_msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame[0].m_img);
        msg->image = *img_msg.toImageMsg();

        //添加消息内容
        msg->orientation = frame[0].m_orientation;
        msg->time_stamp = frame[0].m_time_stamp;

        //智能指针，左值发布
        pub_image->publish(std::move(msg));

        //测帧率使用
        // double  t = wmj::now();
        // msg->true_time_stamp = t;
    }

    cv::Mat camera_node::ros2_get_img(sensor_msgs::msg::Image img)
    {
        //声明 cv::image 指针，内含cv::Mat
        cv_bridge::CvImageConstPtr cv_ptr;
        // toCvShare() 使用
        // sensor_msgs::msg::Image::ConstPtr img_ptr;
        
        //获取图像，最终图像通过 cv_ptr->image 获取，为cv::Mat型
        try
        {
            //把msg里的 Image 转换为 cv::mat类型 , 有两种方式 toCvCopy() 和 toCvShare() ,toCvShare() 虽然可以实现直接引用不用拷贝，
            //但需要创建 sensors_msgs::ImageConst 指针，经测试二者差别不大，故用toCvCopy().后续还可改进.

            cv_ptr = cv_bridge::toCvCopy(img);

            // toCvShare() 方法
            // img_ptr = std::make_shared<const sensor_msgs::msg::Image>(msg->image);
            // cv_ptr = cv_bridge::toCvCopy(img_ptr);
        }
        catch (cv_bridge::Exception &e)
        {
            printf("cv_bridge exception:%s", e.what());
        }
        return (cv_ptr->image);
    }
}

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(wmj::camera_node)