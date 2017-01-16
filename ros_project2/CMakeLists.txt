cmake_minimum_required(VERSION 2.8.3)
project(ros_project2)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg geometry_msgs)
#pkg_check_modules ( ncurses++ REQUIRED ncurses++ )
## Declare ROS messages and services
#add_message_files(FILES Num.msg)
#add_service_files(FILES AddTwoInts.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs geometry_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(twist_ros src/twist.cpp)
target_link_libraries(twist_ros ${catkin_LIBRARIES} ncurses)

add_executable(kb_ros src/kb_commands.cpp)
target_link_libraries(kb_ros ${catkin_LIBRARIES} ncurses)
#add_dependencies(twist_ros beginner_tutorials_generate_messages_cpp)

# %EndTag(FULLTEXT)%

