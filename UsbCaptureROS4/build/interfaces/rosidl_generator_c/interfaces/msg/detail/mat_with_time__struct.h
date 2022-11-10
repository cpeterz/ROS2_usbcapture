// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/MatWithTime.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MAT_WITH_TIME__STRUCT_H_
#define INTERFACES__MSG__DETAIL__MAT_WITH_TIME__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'image'
#include "sensor_msgs/msg/detail/image__struct.h"
// Member 'orientation'
#include "rosidl_runtime_c/string.h"

// Struct defined in msg/MatWithTime in the package interfaces.
typedef struct interfaces__msg__MatWithTime
{
  sensor_msgs__msg__Image image;
  rosidl_runtime_c__String orientation;
  double time_stamp;
  double true_time_stamp;
} interfaces__msg__MatWithTime;

// Struct for a sequence of interfaces__msg__MatWithTime.
typedef struct interfaces__msg__MatWithTime__Sequence
{
  interfaces__msg__MatWithTime * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__MatWithTime__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__MAT_WITH_TIME__STRUCT_H_
