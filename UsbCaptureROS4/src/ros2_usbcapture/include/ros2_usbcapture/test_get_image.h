
// #ifndef COMPOSITION__LISTENER_COMPONENT_HPP_
// #define COMPOSITION__LISTENER_COMPONENT_HPP_

// #include "composition/visibility_control.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "interfaces/msg/mat_with_time.hpp"
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "UsbCaptureSystem.h"
#include "cv_bridge/cv_bridge.h"
#include "RosNode.h"

namespace wmj
{

    class Image_get_test : public rclcpp::Node
    {
    public:
        // COMPOSITION_PUBLIC
        explicit Image_get_test(const rclcpp::NodeOptions &options);

    private:
        //声明回调函数
        void call_back(const interfaces::msg::MatWithTime::SharedPtr msg);
        
        size_t count_;

        //声明发布者
        rclcpp::Subscription<interfaces::msg::MatWithTime>::SharedPtr sub_image;
        
        //用于测试帧率
        // int num;
        // double all_time=0;
        // int count = 0;
    };

} // namespace composition

// #endif // COMPOSITION__LISTENER_COMPONENT_HPP_
