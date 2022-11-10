// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/MatWithTime.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MAT_WITH_TIME__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__MAT_WITH_TIME__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


// Include directives for member types
// Member 'image'
#include "sensor_msgs/msg/detail/image__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__interfaces__msg__MatWithTime __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__MatWithTime __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct MatWithTime_
{
  using Type = MatWithTime_<ContainerAllocator>;

  explicit MatWithTime_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : image(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->orientation = "";
      this->time_stamp = 0.0;
      this->true_time_stamp = 0.0;
    }
  }

  explicit MatWithTime_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : image(_alloc, _init),
    orientation(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->orientation = "";
      this->time_stamp = 0.0;
      this->true_time_stamp = 0.0;
    }
  }

  // field types and members
  using _image_type =
    sensor_msgs::msg::Image_<ContainerAllocator>;
  _image_type image;
  using _orientation_type =
    std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other>;
  _orientation_type orientation;
  using _time_stamp_type =
    double;
  _time_stamp_type time_stamp;
  using _true_time_stamp_type =
    double;
  _true_time_stamp_type true_time_stamp;

  // setters for named parameter idiom
  Type & set__image(
    const sensor_msgs::msg::Image_<ContainerAllocator> & _arg)
  {
    this->image = _arg;
    return *this;
  }
  Type & set__orientation(
    const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other> & _arg)
  {
    this->orientation = _arg;
    return *this;
  }
  Type & set__time_stamp(
    const double & _arg)
  {
    this->time_stamp = _arg;
    return *this;
  }
  Type & set__true_time_stamp(
    const double & _arg)
  {
    this->true_time_stamp = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::MatWithTime_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::MatWithTime_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::MatWithTime_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::MatWithTime_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::MatWithTime_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::MatWithTime_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::MatWithTime_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::MatWithTime_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::MatWithTime_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::MatWithTime_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__MatWithTime
    std::shared_ptr<interfaces::msg::MatWithTime_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__MatWithTime
    std::shared_ptr<interfaces::msg::MatWithTime_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MatWithTime_ & other) const
  {
    if (this->image != other.image) {
      return false;
    }
    if (this->orientation != other.orientation) {
      return false;
    }
    if (this->time_stamp != other.time_stamp) {
      return false;
    }
    if (this->true_time_stamp != other.true_time_stamp) {
      return false;
    }
    return true;
  }
  bool operator!=(const MatWithTime_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MatWithTime_

// alias to use template instance with default allocator
using MatWithTime =
  interfaces::msg::MatWithTime_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__MAT_WITH_TIME__STRUCT_HPP_
