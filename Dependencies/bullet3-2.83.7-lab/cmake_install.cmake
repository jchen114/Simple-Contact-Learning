# Install script for directory: C:/Users/jchen114/Documents/Research/Simple Contact Learning/Dependencies/bullet3-2.83.7

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/BULLET_PHYSICS")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/bullet" TYPE FILE FILES
    "C:/Users/jchen114/Documents/Research/Simple Contact Learning/Dependencies/bullet3-2.83.7/UseBullet.cmake"
    "C:/Users/jchen114/Documents/Research/Simple Contact Learning/Dependencies/bullet3-2.83.7-lab/BulletConfig.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/jchen114/Documents/Research/Simple Contact Learning/Dependencies/bullet3-2.83.7-lab/examples/cmake_install.cmake")
  include("C:/Users/jchen114/Documents/Research/Simple Contact Learning/Dependencies/bullet3-2.83.7-lab/Extras/cmake_install.cmake")
  include("C:/Users/jchen114/Documents/Research/Simple Contact Learning/Dependencies/bullet3-2.83.7-lab/src/cmake_install.cmake")
  include("C:/Users/jchen114/Documents/Research/Simple Contact Learning/Dependencies/bullet3-2.83.7-lab/test/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "C:/Users/jchen114/Documents/Research/Simple Contact Learning/Dependencies/bullet3-2.83.7-lab/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "C:/Users/jchen114/Documents/Research/Simple Contact Learning/Dependencies/bullet3-2.83.7-lab/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
