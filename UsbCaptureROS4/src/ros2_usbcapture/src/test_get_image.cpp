#include "ros2_usbcapture/test_get_image.h"

namespace wmj
{
    Image_get_test::Image_get_test(const rclcpp::NodeOptions &options) : rclcpp::Node("listener", options)
    {
        RCLCPP_INFO(this->get_logger(), "image_component get is working ");
        //创建订阅者
        sub_image = this->create_subscription<interfaces::msg::MatWithTime>("camera_image", 10, std::bind(&Image_get_test::call_back, this, std::placeholders::_1));
    }

    void Image_get_test::call_back(const interfaces::msg::MatWithTime::SharedPtr msg)
    {
        //获取图像，最终图像通过 cv_ptr->image 获取，为cv::Mat型
        try
        {
            //把msg里的 Image 转换为 cv::mat类型 , 有两种方式 toCvCopy() 和 toCvShare() ,toCvShare() 虽然可以实现直接引用不用拷贝，
            //但需要创建 sensors_msgs::ImageConst 指针，经测试二者差别不大，故用toCvCopy().后续还可改进.
            cv_ptr = cv_bridge::toCvCopy(msg->image);
            
            // toCvShare() 方法
            //img_ptr = std::make_shared<const sensor_msgs::msg::Image>(msg->image);
            //cv_ptr = cv_bridge::toCvCopy(img_ptr);
        
        }
        catch (cv_bridge::Exception &e)
        {
            RCLCPP_INFO(this->get_logger(), "cv_bridge exception:%s", e.what());
            return;
        }

        //以下代码均为测试用：
        
        //获取地址
        // std::cout << "addr: 0x" << std::hex << &img << std::endl;

        //显示图像
        cv::imshow(msg->orientation, cv_ptr->image);
        cv::waitKey(1);

        // 输出图像地址
        // std::cout << "addr: " << std::hex << &(msg->image) << std::endl;
        
        //获取当前时间戳
        // double t = wmj::now();

        //计算帧率
        // all_time += t - msg->true_time_stamp;
        // if (all_time > 1)
        // {
        //     std::cout << "speed" << count << std::endl;
        //     all_time = 0;
        //     count =0;
        // }
        // else
        //     count++;

        //计算时间差
        // std::cout <<"hey:"<< t - msg->true_time_stamp  <<std::endl;
    }

}

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(wmj::Image_get_test)