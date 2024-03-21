cmake_minimum_required(VERSION 3.8)

project(RelativeMouseHandler C)

add_executable(RelativeMouseHandler
  src/ainput.c
  src/buffers.c
  src/main.c
)

target_link_libraries(RelativeMouseHandler
  WtsApi32.lib
)
