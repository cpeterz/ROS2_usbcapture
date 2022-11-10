// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/MatWithTime.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MAT_WITH_TIME__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__MAT_WITH_TIME__BUILDER_HPP_

#include "interfaces/msg/detail/mat_with_time__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_MatWithTime_true_time_stamp
{
public:
  explicit Init_MatWithTime_true_time_stamp(::interfaces::msg::MatWithTime & msg)
  : msg_(msg)
  {}
  ::interfaces::msg::MatWithTime true_time_stamp(::interfaces::msg::MatWithTime::_true_time_stamp_type arg)
  {
    msg_.true_time_stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::MatWithTime msg_;
};

class Init_MatWithTime_time_stamp
{
public:
  explicit Init_MatWithTime_time_stamp(::interfaces::msg::MatWithTime & msg)
  : msg_(msg)
  {}
  Init_MatWithTime_true_time_stamp time_stamp(::interfaces::msg::MatWithTime::_time_stamp_type arg)
  {
    msg_.time_stamp = std::move(arg);
    return Init_MatWithTime_true_time_stamp(msg_);
  }

private:
  ::interfaces::msg::MatWithTime msg_;
};

class Init_MatWithTime_orientation
{
public:
  explicit Init_MatWithTime_orientation(::interfaces::msg::MatWithTime & msg)
  : msg_(msg)
  {}
  Init_MatWithTime_time_stamp orientation(::interfaces::msg::MatWithTime::_orientation_type arg)
  {
    msg_.orientation = std::move(arg);
    return Init_MatWithTime_time_stamp(msg_);
  }

private:
  ::interfaces::msg::MatWithTime msg_;
};

class Init_MatWithTime_image
{
public:
  Init_MatWithTime_image()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MatWithTime_orientation image(::interfaces::msg::MatWithTime::_image_type arg)
  {
    msg_.image = std::move(arg);
    return Init_MatWithTime_orientation(msg_);
  }

private:
  ::interfaces::msg::MatWithTime msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::MatWithTime>()
{
  return interfaces::msg::builder::Init_MatWithTime_image();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__MAT_WITH_TIME__BUILDER_HPP_
