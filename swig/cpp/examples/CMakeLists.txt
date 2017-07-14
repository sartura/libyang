include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(process_tree process_tree.cpp)

target_link_libraries(process_tree libyang_cpp)
