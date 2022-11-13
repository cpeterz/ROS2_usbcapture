cmake_minimum_required(VERSION 3.5)
project(ros2_usbcapture)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


include_directories(include)
find_package(common REQUIRED)
find_package(OpenCV 3.4.13 REQUIRED)
find_package(interfaces REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(rclcpp_components REQUIRED)

#component所用camke
add_library(camera_component SHARED
 src/RosNode.cpp
 src/UsbCaptureSystem.cpp
)

add_library(test_component SHARED
 src/test_get_image.cpp
 src/RosNode.cpp
)
# target_compile_definitions(camera_component
#  PRIVATE "COMPOSITION_BUILDING_DLL")

ament_target_dependencies(camera_component
      "common"
      "OpenCV"
      "rclcpp"
      "interfaces"
      "cv_bridge"
      "rclcpp_components"
)

ament_target_dependencies(test_component
      "common"
      "OpenCV"
      "rclcpp"
      "interfaces"
      "cv_bridge"
      "rclcpp_components"
)



target_link_libraries(camera_component gxiapi)  #大恒使用必带

add_definitions(-DUSBCAPTURE_CFG="${CMAKE_CURRENT_LIST_DIR}/UsbCapture.yaml")

install(TARGETS 
  camera_component
  test_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

rclcpp_components_register_nodes(camera_component "wmj::camera_node")
rclcpp_components_register_nodes(test_component "wmj::Image_get_test")
#set(node_plugins "${node_plugins}wmj::camera_node;$<TARGET_FILE:camera_component>\n")

#add_executable(camera_node src/UsbCaptureSystem.cpp src/RosNode.cpp)


install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
