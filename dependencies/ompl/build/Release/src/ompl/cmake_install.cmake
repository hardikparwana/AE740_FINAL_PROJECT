# Install script for directory: /media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/src/ompl

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ompl-1.5/ompl" TYPE FILE FILES "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/src/ompl/config.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xomplx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libompl.so.1.5.2"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libompl.so.16"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libompl.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "/usr/local/lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/lib/libompl.so.1.5.2"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/lib/libompl.so.16"
    "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/lib/libompl.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libompl.so.1.5.2"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libompl.so.16"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libompl.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/usr/local/lib:"
           NEW_RPATH "/usr/local/lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

