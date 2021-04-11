# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget octomath octomath-static octomap octomap-static)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  unset(_targetsDefined)
  unset(_targetsNotDefined)
  unset(_expectedTargets)
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Create imported target octomath
add_library(octomath SHARED IMPORTED)

# Create imported target octomath-static
add_library(octomath-static STATIC IMPORTED)

# Create imported target octomap
add_library(octomap SHARED IMPORTED)

set_target_properties(octomap PROPERTIES
  INTERFACE_LINK_LIBRARIES "octomath"
)

# Create imported target octomap-static
add_library(octomap-static STATIC IMPORTED)

# Import target "octomath" for configuration "Release"
set_property(TARGET octomath APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(octomath PROPERTIES
  IMPORTED_LOCATION_RELEASE "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/octomap/lib/liboctomath.so.1.9.6"
  IMPORTED_SONAME_RELEASE "liboctomath.so.1.9"
  )

# Import target "octomath-static" for configuration "Release"
set_property(TARGET octomath-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(octomath-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/octomap/lib/liboctomath.a"
  )

# Import target "octomap" for configuration "Release"
set_property(TARGET octomap APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(octomap PROPERTIES
  IMPORTED_LOCATION_RELEASE "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/octomap/lib/liboctomap.so.1.9.6"
  IMPORTED_SONAME_RELEASE "liboctomap.so.1.9"
  )

# Import target "octomap-static" for configuration "Release"
set_property(TARGET octomap-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(octomap-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/octomap/lib/liboctomap.a"
  )

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget octomath octomath-static octomap octomap-static)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  unset(_targetsDefined)
  unset(_targetsNotDefined)
  unset(_expectedTargets)
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Create imported target octomath
add_library(octomath SHARED IMPORTED)

# Create imported target octomath-static
add_library(octomath-static STATIC IMPORTED)

# Create imported target octomap
add_library(octomap SHARED IMPORTED)

set_target_properties(octomap PROPERTIES
  INTERFACE_LINK_LIBRARIES "octomath"
)

# Create imported target octomap-static
add_library(octomap-static STATIC IMPORTED)

# Import target "octomath" for configuration "Release"
set_property(TARGET octomath APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(octomath PROPERTIES
  IMPORTED_LOCATION_RELEASE "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/octomap/lib/liboctomath.so.1.9.6"
  IMPORTED_SONAME_RELEASE "liboctomath.so.1.9"
  )

# Import target "octomath-static" for configuration "Release"
set_property(TARGET octomath-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(octomath-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/octomap/lib/liboctomath.a"
  )

# Import target "octomap" for configuration "Release"
set_property(TARGET octomap APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(octomap PROPERTIES
  IMPORTED_LOCATION_RELEASE "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/octomap/lib/liboctomap.so.1.9.6"
  IMPORTED_SONAME_RELEASE "liboctomap.so.1.9"
  )

# Import target "octomap-static" for configuration "Release"
set_property(TARGET octomap-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(octomap-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/octomap/lib/liboctomap.a"
  )

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
