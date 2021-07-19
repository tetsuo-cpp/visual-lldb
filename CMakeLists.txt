cmake_minimum_required(VERSION 3.16.0)

project(visual-lldb VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup Qt6.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(visual-lldb
  MainWindow.ui
  MainWindow.cpp
  LldbDebugger.cpp
  BreakpointModel.cpp
  visual-lldb.cpp
  resources.qrc
  )

target_link_libraries(visual-lldb PRIVATE Qt6::Widgets lldb)
