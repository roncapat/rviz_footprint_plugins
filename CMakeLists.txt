cmake_minimum_required(VERSION 3.5)

project(rviz_footprint_plugins)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Wnon-virtual-dtor -Woverloaded-virtual)
endif()

# Tests currently only run on OS X @ OSRF jenkins
# Enable on Linux by providing a display, enable on Windows via EnableDisplayTests=True
option(EnableDisplayTests "EnableDisplayTests")
set(DisplayTests "False" CACHE STRING "DisplayTestsVariable")

if(DEFINED ENV{DISPLAY})
  set(DISPLAYPRESENT TRUE)
endif()

if(APPLE OR DISPLAYPRESENT OR EnableDisplayTests STREQUAL "True")
  message(STATUS "Enabling tests requiring a display")
else()
  set(SKIP_DISPLAY_TESTS "SKIP_TEST")
endif()

# options and directories for visual tests (see visual_testing_framework documentation)
option(EnableVisualTests "decides whether or not to enable the tests")

add_definitions(-D_BUILD_DIR_PATH="${CMAKE_CURRENT_BINARY_DIR}")
add_definitions(-D_SRC_DIR_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_images)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/reference_images)

# Run visual tests only if "EnableVisualTests=True"
if(EnableVisualTests STREQUAL "True")
  message(STATUS "Enabling visual tests")
else()
  set(SKIP_VISUAL_TESTS "SKIP_TEST")
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

# We specifically don't turn on CMAKE_AUTOMOC, since it generates one huge
# mocs_compilation.cpp file that takes a lot of memory to compile.  Instead
# we create individual moc files that can be compiled separately.

find_package(ament_cmake REQUIRED)

find_package(rviz_common REQUIRED)
find_package(rviz_rendering REQUIRED)
find_package(rviz_ogre_vendor REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Widgets Test)

find_package(geometry_msgs REQUIRED)
find_package(footprint_msgs REQUIRED)

find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(resource_retriever REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

set(rviz_footprint_plugins_headers_to_moc
  include/rviz_footprint_plugins/displays/polygon/polygon_array_display.hpp
)

foreach(header "${rviz_footprint_plugins_headers_to_moc}")
  qt5_wrap_cpp(rviz_footprint_plugins_moc_files "${header}")
endforeach()

set(rviz_footprint_plugins_source_files
  src/rviz_footprint_plugins/displays/polygon/polygon_array_display.cpp
)

add_library(rviz_footprint_plugins SHARED
  ${rviz_footprint_plugins_moc_files}
  ${rviz_footprint_plugins_source_files}
)

target_include_directories(rviz_footprint_plugins PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${Qt5Widgets_INCLUDE_DIRS}
)

target_link_libraries(rviz_footprint_plugins PUBLIC
  rviz_ogre_vendor::OgreMain
  rviz_ogre_vendor::OgreOverlay
)

# Causes the visibility macros to use dllexport rather than dllimport,
# which is appropriate when building the dll but not consuming it.
target_compile_definitions(rviz_footprint_plugins PRIVATE "rviz_footprint_plugins_BUILDING_LIBRARY")

# prevent pluginlib from using boost
target_compile_definitions(rviz_footprint_plugins PUBLIC "PLUGINLIB__DISABLE_BOOST_FUNCTIONS")

pluginlib_export_plugin_description_file(rviz_common plugins_description.xml)

ament_target_dependencies(rviz_footprint_plugins
  PUBLIC
  geometry_msgs
  footprint_msgs
  rclcpp
  resource_retriever
  rviz_common
  rviz_rendering
  tf2
  tf2_geometry_msgs
  tf2_ros
)

ament_export_include_directories(include)
ament_export_targets(rviz_footprint_plugins HAS_LIBRARY_TARGET)
ament_export_dependencies(
  rviz_common
  geometry_msgs
  trav_msgs
  rclcpp
  resource_retriever
  rviz_ogre_vendor
  tf2
  tf2_geometry_msgs
  tf2_ros
)

install(
  TARGETS rviz_footprint_plugins
  EXPORT rviz_footprint_plugins
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/icons"
  DESTINATION "share/${PROJECT_NAME}"
)

ament_package(
  CONFIG_EXTRAS "rviz_footprint_plugins-extras.cmake"
)

