cmake_minimum_required(VERSION 3.16.0)

project(visual-lldb VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup Qt6.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Setup LLVM.
find_package(LLVM REQUIRED CONFIG)
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
include(AddLLVM)
find_package(LLDB REQUIRED CONFIG)
include_directories(SYSTEM ${LLDB_INCLUDE_DIRS})
add_definitions(${LLDB_DEFINITIONS})

# Dynamically link to LLVM.
set(LLVM_LINK_LLVM_DYLIB ON)

add_executable(visual-lldb
  MainWindow.ui
  MainWindow.cpp
  visual-lldb.cpp
  resources.qrc
  )

# Find the libraries that correspond to the LLVM components that we wish to use.
llvm_map_components_to_libnames(llvm_libs support core)

target_link_libraries(visual-lldb PRIVATE Qt6::Widgets ${llvm_libs} liblldb)
