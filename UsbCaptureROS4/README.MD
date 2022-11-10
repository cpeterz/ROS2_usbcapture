# ROS2相机封装 v1.0

==此版暂时只实现了单目软触发==

**利用cv_bridge作为中间桥梁将  sensor::msgs::Image 和 cv::Mat 作了相互转换，这里依旧存在拷贝现象**

**目前发布传输耗时大概在 0.8 ms左右**



###使用方法

1.需要定义订阅者，如：

`sub_image = this->create_subscription<interfaces::msg::MatWithTime>("camera_image", 10, std::bind(call_back, this, std::placeholders::_1));`

2.假设 `msg`是回调函数指针形参:

获取图像的orientation :`msg->orientation`  

获取图像的时间戳: `msg->time_stamp`   

类型转换:` cv_ptr = cv_bridge::toCvCopy(msg->image)`    

获取图像: `cv_ptr->image`

​      

###注册构件方法

 使用的容器是ROS2自带的`rclcpp_component`,请将需要的读图构件加入进去，注意开启`intra_process`加快传输速度。

```c++
//实例
ComposableNode(
      package='ros2_usbcapture',
      plugin='wmj::Image_get_test',
      name='listener',
      extra_arguments=[{'use_intra_process_comms': True}]
)
```

