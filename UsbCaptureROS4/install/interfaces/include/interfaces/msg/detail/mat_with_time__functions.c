// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from interfaces:msg/MatWithTime.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/mat_with_time__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `image`
#include "sensor_msgs/msg/detail/image__functions.h"
// Member `orientation`
#include "rosidl_runtime_c/string_functions.h"

bool
interfaces__msg__MatWithTime__init(interfaces__msg__MatWithTime * msg)
{
  if (!msg) {
    return false;
  }
  // image
  if (!sensor_msgs__msg__Image__init(&msg->image)) {
    interfaces__msg__MatWithTime__fini(msg);
    return false;
  }
  // orientation
  if (!rosidl_runtime_c__String__init(&msg->orientation)) {
    interfaces__msg__MatWithTime__fini(msg);
    return false;
  }
  // time_stamp
  // true_time_stamp
  return true;
}

void
interfaces__msg__MatWithTime__fini(interfaces__msg__MatWithTime * msg)
{
  if (!msg) {
    return;
  }
  // image
  sensor_msgs__msg__Image__fini(&msg->image);
  // orientation
  rosidl_runtime_c__String__fini(&msg->orientation);
  // time_stamp
  // true_time_stamp
}

bool
interfaces__msg__MatWithTime__are_equal(const interfaces__msg__MatWithTime * lhs, const interfaces__msg__MatWithTime * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // image
  if (!sensor_msgs__msg__Image__are_equal(
      &(lhs->image), &(rhs->image)))
  {
    return false;
  }
  // orientation
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->orientation), &(rhs->orientation)))
  {
    return false;
  }
  // time_stamp
  if (lhs->time_stamp != rhs->time_stamp) {
    return false;
  }
  // true_time_stamp
  if (lhs->true_time_stamp != rhs->true_time_stamp) {
    return false;
  }
  return true;
}

bool
interfaces__msg__MatWithTime__copy(
  const interfaces__msg__MatWithTime * input,
  interfaces__msg__MatWithTime * output)
{
  if (!input || !output) {
    return false;
  }
  // image
  if (!sensor_msgs__msg__Image__copy(
      &(input->image), &(output->image)))
  {
    return false;
  }
  // orientation
  if (!rosidl_runtime_c__String__copy(
      &(input->orientation), &(output->orientation)))
  {
    return false;
  }
  // time_stamp
  output->time_stamp = input->time_stamp;
  // true_time_stamp
  output->true_time_stamp = input->true_time_stamp;
  return true;
}

interfaces__msg__MatWithTime *
interfaces__msg__MatWithTime__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  interfaces__msg__MatWithTime * msg = (interfaces__msg__MatWithTime *)allocator.allocate(sizeof(interfaces__msg__MatWithTime), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(interfaces__msg__MatWithTime));
  bool success = interfaces__msg__MatWithTime__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
interfaces__msg__MatWithTime__destroy(interfaces__msg__MatWithTime * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    interfaces__msg__MatWithTime__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
interfaces__msg__MatWithTime__Sequence__init(interfaces__msg__MatWithTime__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  interfaces__msg__MatWithTime * data = NULL;

  if (size) {
    data = (interfaces__msg__MatWithTime *)allocator.zero_allocate(size, sizeof(interfaces__msg__MatWithTime), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = interfaces__msg__MatWithTime__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        interfaces__msg__MatWithTime__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
interfaces__msg__MatWithTime__Sequence__fini(interfaces__msg__MatWithTime__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      interfaces__msg__MatWithTime__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

interfaces__msg__MatWithTime__Sequence *
interfaces__msg__MatWithTime__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  interfaces__msg__MatWithTime__Sequence * array = (interfaces__msg__MatWithTime__Sequence *)allocator.allocate(sizeof(interfaces__msg__MatWithTime__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = interfaces__msg__MatWithTime__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
interfaces__msg__MatWithTime__Sequence__destroy(interfaces__msg__MatWithTime__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    interfaces__msg__MatWithTime__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
interfaces__msg__MatWithTime__Sequence__are_equal(const interfaces__msg__MatWithTime__Sequence * lhs, const interfaces__msg__MatWithTime__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!interfaces__msg__MatWithTime__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
interfaces__msg__MatWithTime__Sequence__copy(
  const interfaces__msg__MatWithTime__Sequence * input,
  interfaces__msg__MatWithTime__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(interfaces__msg__MatWithTime);
    interfaces__msg__MatWithTime * data =
      (interfaces__msg__MatWithTime *)realloc(output->data, allocation_size);
    if (!data) {
      return false;
    }
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!interfaces__msg__MatWithTime__init(&data[i])) {
        /* free currently allocated and return false */
        for (; i-- > output->capacity; ) {
          interfaces__msg__MatWithTime__fini(&data[i]);
        }
        free(data);
        return false;
      }
    }
    output->data = data;
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!interfaces__msg__MatWithTime__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
