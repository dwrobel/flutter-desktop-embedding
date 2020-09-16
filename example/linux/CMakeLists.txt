# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Damian Wrobel <dwrobel@ertelnet.rybnik.pl>
#

cmake_minimum_required (VERSION 3.3)
project (flutter_lanucher)
find_package(PkgConfig)

pkg_search_module(EGL egl REQUIRED)
pkg_search_module(GLES glesv2 REQUIRED)
pkg_search_module(GLIB glib-2.0 REQUIRED)
pkg_search_module(GTK gtk+-3.0 REQUIRED)

if (CMAKE_VERSION VERSION_LESS "3.8.2")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
else()
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

set(SOURCES main.cc)

# It support local and system-wide build version
find_path(FLUTTER_WRAPPER_INCLUDE_DIR "flutter/flutter_window_controller.h"
          PATH_SUFFIXES "flutter/ephemeral/cpp_client_wrapper_glfw/include"
                  HINTS "flutter/ephemeral/cpp_client_wrapper_glfw/include"
)
message(STATUS "FLUTTER_WRAPPER_INCLUDE_DIR: ${FLUTTER_WRAPPER_INCLUDE_DIR}")

find_path(FLUTTER_EPHEMERAL_INCLUDE_DIR "flutter_glfw.h"
          PATH_SUFFIXES "flutter/ephemeral" "flutter"
                  HINTS "flutter/ephemeral" "flutter"
)
message(STATUS "FLUTTER_EPHEMERAL_INCLUDE_DIR: ${FLUTTER_EPHEMERAL_INCLUDE_DIR}")

find_file(FLUTTER_WINDOW_CONTROLLER_FILE "flutter_window_controller.cc"
          PATH_SUFFIXES "flutter/ephemeral/cpp_client_wrapper_glfw"
                  HINTS "flutter/ephemeral/cpp_client_wrapper_glfw"
)
message(STATUS "FLUTTER_WINDOW_CONTROLLER_FILE: ${FLUTTER_WINDOW_CONTROLLER_FILE}")
set(SOURCES ${SOURCES} ${FLUTTER_WINDOW_CONTROLLER_FILE})

set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/flutter/generated_plugin_registrant.cc)

find_library(FLUTTER_LINUX_GLFW_LIBRARY NAMES "flutter_linux_glfw" PATHS "${CMAKE_CURRENT_SOURCE_DIR}/flutter/ephemeral")
message(STATUS "FLUTTER_LINUX_GLFW_LIBRARY: ${FLUTTER_LINUX_GLFW_LIBRARY}")
get_filename_component(FLUTTER_LINUX_GLFW_LIBRARY_DIRS ${FLUTTER_LINUX_GLFW_LIBRARY} DIRECTORY)

link_directories(${FLUTTER_LINUX_GLFW_LIBRARY_DIRS} ${EGL_LIBRARY_DIRS} ${GLES_LIBRARY_DIRS} ${GLIB_LIBRARY_DIRS} ${GTK_LIBRARY_DIRS})

add_executable(flutter-launcher ${SOURCES})
target_include_directories(flutter-launcher PRIVATE ${FLUTTER_WRAPPER_INCLUDE_DIR} ${FLUTTER_EPHEMERAL_INCLUDE_DIR} ${EGL_INCLUDE_DIRS} ${GLES_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS} ${GTK_INCLUDE_DIRS})
target_link_libraries(flutter-launcher flutter_linux_glfw ${EGL_LIBRARIES} ${GLES_LIBRARIES} ${GLIB_LIBRARIES} ${GTK_LIBRARIES})

install(TARGETS flutter-launcher DESTINATION bin)
