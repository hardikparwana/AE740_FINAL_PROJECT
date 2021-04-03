cmake_minimum_required(VERSION 3.0.2)
project(vehicle_description)

find_package(catkin REQUIRED COMPONENTS
  urdf
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES m2wr_description
#  CATKIN_DEPENDS urdf
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)
