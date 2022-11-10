// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:msg/MatWithTime.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MAT_WITH_TIME__TRAITS_HPP_
#define INTERFACES__MSG__DETAIL__MAT_WITH_TIME__TRAITS_HPP_

#include "interfaces/msg/detail/mat_with_time__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

// Include directives for member types
// Member 'image'
#include "sensor_msgs/msg/detail/image__traits.hpp"

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::msg::MatWithTime>()
{
  return "interfaces::msg::MatWithTime";
}

template<>
inline const char * name<interfaces::msg::MatWithTime>()
{
  return "interfaces/msg/MatWithTime";
}

template<>
struct has_fixed_size<interfaces::msg::MatWithTime>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<interfaces::msg::MatWithTime>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<interfaces::msg::MatWithTime>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__MSG__DETAIL__MAT_WITH_TIME__TRAITS_HPP_
