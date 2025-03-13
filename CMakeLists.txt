cmake_minimum_required(VERSION 3.31)
project(softbody)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(src/vector)
add_subdirectory(src/sb_body)
add_subdirectory(src/sb_blob)
add_subdirectory(src/sb_app)

add_executable(softbody src/main.c)

target_link_libraries(softbody PRIVATE SDL3::SDL3)
target_link_libraries(softbody PUBLIC vector)
target_link_libraries(softbody PUBLIC sb_body)
target_link_libraries(softbody PUBLIC sb_blob)
target_link_libraries(softbody PUBLIC sb_app)
