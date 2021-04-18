cmake_minimum_required(VERSION 3.0.2)
project(vehicle_description)

set(CMAKE_CXX_STANDARD 11)

find_package(catkin REQUIRED COMPONENTS
  urdf
  geometry_msgs
  sensor_msgs
  gazebo_msgs
  roscpp
  tf
  ackermann_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES vehicle_description
#  CATKIN_DEPENDS urdf
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

find_package(Eigen3)

# add_executable(move_cart src/move_cart.cpp)
# target_link_libraries(move_cart ${catkin_LIBRARIES})

add_executable(move_cart_holonomic src/move_cart_holonomic.cpp)
target_link_libraries(move_cart_holonomic ${catkin_LIBRARIES})

# add_executable(move_van_holonomic src/move_van_holonomic.cpp)
# target_link_libraries(move_van_holonomic ${catkin_LIBRARIES})

add_executable(drive_van src/drive_van.cpp)
target_link_libraries(drive_van ${catkin_LIBRARIES})
