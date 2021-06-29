cmake_minimum_required(VERSION 3.0)

project(caps2esc60)

add_executable(caps2esc60 caps2esc60.c)
target_compile_options(caps2esc60 PRIVATE -Wall -Wextra)

install(TARGETS caps2esc60 RUNTIME DESTINATION bin)
