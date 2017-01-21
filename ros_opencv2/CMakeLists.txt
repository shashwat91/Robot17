cmake_minimum_required(VERSION 2.8.3)
project(ros_opencv2)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg geometry_msgs cv_bridge sensor_msgs image_transport)

## Declare ROS messages and services
#add_message_files(FILES Num.msg)
#add_service_files(FILES AddTwoInts.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs geometry_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(linefollower_17 src/linefollower_17.cpp)
target_link_libraries(linefollower_17 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
#add_dependencies(twist_ros beginner_tutorials_generate_messages_cpp)

# %EndTag(FULLTEXT)%