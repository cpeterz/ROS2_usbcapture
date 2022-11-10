import launch
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode

def generate_launch_description():
    container = ComposableNodeContainer(
            name='my_container',
            namespace='',
            package='rclcpp_components',
            executable='component_container',
            
            composable_node_descriptions=[
                ComposableNode(
                    package='ros2_usbcapture',
                    plugin='wmj::camera_node',
                    name='talker',
                    extra_arguments=[{'use_intra_process_comms': True}]
                    ),
                ComposableNode(
                    package='ros2_usbcapture',
                    plugin='wmj::Image_get_test',
                    name='listener',
                    extra_arguments=[{'use_intra_process_comms': True}]
                    )
            ],
            output='screen',
    )

    return launch.LaunchDescription([container])
