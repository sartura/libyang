cmake_minimum_required(VERSION 2.8.12)

set(deepstate_tests xml test_parser_xml)

foreach(target_name IN LISTS deepstate_tests)
    add_executable(${target_name} ${target_name}.cpp $<TARGET_OBJECTS:yangobj>)
    target_link_libraries(${target_name} yang deepstate)
    target_link_libraries(${target_name} ${CMAKE_THREADS_LIB_INIT})
endforeach(target_name)
