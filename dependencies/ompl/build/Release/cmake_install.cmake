# Install script for directory: /media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/ompl.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl/cmake" TYPE FILE FILES
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/omplConfig.cmake"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/omplConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ompl" TYPE FILE FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/packages" TYPE FILE FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/share/ament_index/resource_index/packages/ompl")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/src/cmake_install.cmake")
  include("/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/py-bindings/cmake_install.cmake")
  include("/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/tests/cmake_install.cmake")
  include("/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/demos/cmake_install.cmake")
  include("/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/scripts/cmake_install.cmake")
  include("/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/doc/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
