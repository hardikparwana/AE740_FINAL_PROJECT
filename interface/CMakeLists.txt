cmake_minimum_required(VERSION 3.0.2)
project(interface)

set(CMAKE_CXX_STANDARD 11)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  sensor_msgs
  std_msgs
  roscpp
  tf
  eigen_catkin
  eigen_conversions
  eigen_checks
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES vehicle_description
#  CATKIN_DEPENDS urdf
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

# Build the utils library
add_library(utilslib src/utils.cpp)
target_link_libraries(utilslib ${catkin_LIBRARIES})

add_executable(master_node nodes/master_node.cpp)# src/utils.cpp)
target_link_libraries(master_node ${catkin_LIBRARIES} utilslib)

