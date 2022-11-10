// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from interfaces:msg/MatWithTime.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "interfaces/msg/detail/mat_with_time__rosidl_typesupport_introspection_c.h"
#include "interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "interfaces/msg/detail/mat_with_time__functions.h"
#include "interfaces/msg/detail/mat_with_time__struct.h"


// Include directives for member types
// Member `image`
#include "sensor_msgs/msg/image.h"
// Member `image`
#include "sensor_msgs/msg/detail/image__rosidl_typesupport_introspection_c.h"
// Member `orientation`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  interfaces__msg__MatWithTime__init(message_memory);
}

void MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_fini_function(void * message_memory)
{
  interfaces__msg__MatWithTime__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_message_member_array[4] = {
  {
    "image",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(interfaces__msg__MatWithTime, image),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "orientation",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(interfaces__msg__MatWithTime, orientation),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "time_stamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(interfaces__msg__MatWithTime, time_stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "true_time_stamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(interfaces__msg__MatWithTime, true_time_stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_message_members = {
  "interfaces__msg",  // message namespace
  "MatWithTime",  // message name
  4,  // number of fields
  sizeof(interfaces__msg__MatWithTime),
  MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_message_member_array,  // message members
  MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_init_function,  // function to initialize message memory (memory has to be allocated)
  MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_message_type_support_handle = {
  0,
  &MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, interfaces, msg, MatWithTime)() {
  MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, sensor_msgs, msg, Image)();
  if (!MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_message_type_support_handle.typesupport_identifier) {
    MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &MatWithTime__rosidl_typesupport_introspection_c__MatWithTime_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
