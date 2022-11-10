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
        auto img_msg = cv_bridge::CvImage(std_msgs::msg::Header(),"bgr8",frame[0].m_img);
        msg->image =  *img_msg.toImageMsg();
        
        //添加消息内容
        msg->orientation = frame[0].m_orientation;
        msg->time_stamp = frame[0].m_time_stamp;

        //智能指针，左值发布
        pub_image->publish(std::move(msg));
        
        //测帧率使用
        //double  t = wmj::now();
        //msg->true_time_stamp = t;
    }
}

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(wmj::camera_node)