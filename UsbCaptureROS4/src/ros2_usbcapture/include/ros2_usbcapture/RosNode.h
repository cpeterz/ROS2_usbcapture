#ifndef COMPOSITION__TALKER_COMPONENT_HPP_
#define COMPOSITION__TALKER_COMPONENT_HPP_

#include "UsbCaptureSystem.h"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "interfaces/msg/mat_with_time.hpp"
#include <cv_bridge/cv_bridge.h>

namespace wmj
{
    class camera_node : public rclcpp::Node
    {
    public:
        explicit camera_node(const rclcpp::NodeOptions & options);

        //利用 ros2_get_img 获取图像，传入类型为 sensor_msgs::msg::Image ，返回为 cv::Mat 类型
        static cv::Mat ros2_get_img(sensor_msgs::msg::Image img);


    private:
        //创建发布者
        rclcpp::Publisher<interfaces::msg::MatWithTime>::SharedPtr pub_image;
        
        //创建时间回调函数
        void timer_call_back();
        
        //创建定时器
        rclcpp::TimerBase::SharedPtr timer_;
        
        //获取相机数量
        int camera_num = 0;
        
        size_t count_;
        
        //声明相机用于读图
        std::shared_ptr<wmj::UsbCaptureSystem> capture ;
        
        //读取图像变量
        std::vector<wmj::MatWithTime> frame;
    };
}

#endif
