# Install script for directory: /media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE PROGRAM FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ompl/demos/KinematicChainPathPlot.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE PROGRAM FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ompl/demos/OptimalPlanning.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE PROGRAM FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ompl/demos/PlannerData.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE PROGRAM FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ompl/demos/Point2DPlanning.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE PROGRAM FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ompl/demos/RandomWalkPlanner.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE PROGRAM FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ompl/demos/RigidBodyPlanning.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE PROGRAM FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ompl/demos/RigidBodyPlanningWithControls.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE PROGRAM FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ompl/demos/RigidBodyPlanningWithODESolverAndControls.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE PROGRAM FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ompl/demos/StateSampling.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE FILE FILES
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/CForestCircleGridBenchmark.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/Diagonal.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/GeometricCarPlanning.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/HybridSystemPlanning.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/HypercubeBenchmark.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/KinematicChainBenchmark.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/LTLWithTriangulation.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/OpenDERigidBodyPlanning.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/OptimalPlanning.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/PlannerData.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/PlannerProgressProperties.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/Point2DPlanning.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/RigidBodyPlanning.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/RigidBodyPlanningWithControls.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/RigidBodyPlanningWithIK.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/RigidBodyPlanningWithIntegrationAndControls.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/RigidBodyPlanningWithODESolverAndControls.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/StateSampling.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/ThunderLightning.cpp"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/TriangulationDemo.cpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE DIRECTORY FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/Koules")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/demos" TYPE DIRECTORY FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/demos/VFRRT")
endif()

