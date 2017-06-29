set(PYTHON_SWIG_BINDING_3 libyang_python3)
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_SWIG_FLAGS "-c++")
set(CMAKE_SWIG_FLAGS "-I${PROJECT_SOURCE_DIR}")
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
set(PYTHON3_SWIG_MIN_VERSION "3.0.5")

find_package(SWIG ${PYTHON3_SWIG_MIN_VERSION})
if (NOT SWIG_FOUND)
  message(WARNING "Minimum SWIG version for Python is ${PYTHON3_SWIG_MIN_VERSION}. Tests won't be generated.")
endif()

set_source_files_properties(${PYTHON_SWIG_BINDING_3}.i PROPERTIES CPLUSPLUS ON)

swig_add_module(${PYTHON_SWIG_BINDING_3} python ${PYTHON_SWIG_BINDING_3}.i)
swig_link_libraries(${PYTHON_SWIG_BINDING_3} ${PYTHON_LIBRARIES} libyang_cpp)

execute_process(COMMAND
    ${PYTHON_EXECUTABLE} -c
    "from distutils.sysconfig import get_python_lib; print(get_python_lib())"
OUTPUT_VARIABLE PYTHON_MODULE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)

install( FILES "${CMAKE_CURRENT_BINARY_DIR}/_${PYTHON_SWIG_BINDING_3}.so" DESTINATION ${PYTHON_MODULE_PATH} )
install( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_SWIG_BINDING_3}.py" DESTINATION ${PYTHON_MODULE_PATH} )
