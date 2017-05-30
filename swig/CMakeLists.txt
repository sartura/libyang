set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")

# find SWIG package
if(GEN_LANGUAGE_BINDINGS)
    find_package(SWIG)
    if(NOT SWIG_FOUND)
        message(WARNING "SWIG library not found")
    else()
        include(${SWIG_USE_FILE})
    endif()
endif()

# find Python package
if(GEN_PYTHON_BINDINGS AND SWIG_FOUND)
    message("-- Python version ${GEN_PYTHON_VERSION} was selected")
    if(${GEN_PYTHON_VERSION} STREQUAL "2")
        find_package(PythonLibs 2)
        find_package(PythonInterp)
        if(NOT PYTHONLIBS_FOUND)
            message(WARNING "Did not found Python version 2.x")
            message("-- Sysrepo supports Python 2.x and Python 3.x")
        endif()
    elseif(${GEN_PYTHON_VERSION} STREQUAL "3")
        find_package(PythonLibs 3)
        find_package(PythonInterp)
        if(NOT PYTHONLIBS_FOUND)
            message(WARNING "Did not found Python version 3.x")
            message("-- Sysrepo supports Python 2.x and Python 3.x")
        endif()
    else()
        message(WARNING "Sysrepo supports Python 2.x and Python 3.x")
    endif()
endif()

project(libyang_cpp)
set(LIBYANG_CPP_SOURCES
	${CMAKE_SOURCE_DIR}/swig/cpp/src/Libyang.cpp)

set(LIBYANG_H_SOURCES
	${CMAKE_SOURCE_DIR}/swig/cpp/src/Libyang.h
	${CMAKE_SOURCE_DIR}/swig/cpp/src/Internal.h)

FUNCTION(PREPEND var prefix)
	SET(listVar "")
	FOREACH(f ${ARGN})
		LIST(APPEND listVar "${prefix}/${f}")
	ENDFOREACH(f)
	SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

PREPEND(abs_libsrc ${CMAKE_SOURCE_DIR} ${libsrc})

# create static libyang C++ library
if(STATIC)
	add_library(libyang_cpp_static STATIC ${LIBYANG_CPP_SOURCES} ${abs_libsrc})
    set_target_properties(libyang_cpp_static PROPERTIES OUTPUT_NAME libyang_cpp)
	set_target_properties(libyang_cpp_static PROPERTIES PREFIX "")
	set_target_properties(libyang_cpp_static PROPERTIES COMPILE_FLAGS "-fvisibility=hidden")
    target_link_libraries(libyang_cpp_static m)
    target_link_libraries(libyang_cpp_static ${CMAKE_DL_LIBS})
    target_link_libraries(libyang_cpp_static ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(libyang_cpp_static ${PCRE_LIBRARIES})
    install(TARGETS libyang_cpp_static DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(STATIC)

add_library(libyang_cpp SHARED ${LIBYANG_CPP_SOURCES})
SET_TARGET_PROPERTIES(libyang_cpp PROPERTIES
              VERSION ${LIBYANG_VERSION} SOVERSION ${LIBYANG_SOVERSION})

set_target_properties(libyang_cpp PROPERTIES PREFIX "")

# include custom Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

include_directories(${CMAKE_SOURCE_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cpp/src")
target_link_libraries(libyang_cpp yang)

# install binary
install(TARGETS libyang_cpp DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cpp/src/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libyang
    FILES_MATCHING PATTERN "*.h")

# Examples
if(GEN_CPP_BINDINGS)
endif()

if(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND AND ${GEN_PYTHON_VERSION} STREQUAL "2")
	#add_subdirectory(python2)
endif()

if(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND AND ${GEN_PYTHON_VERSION} STREQUAL "3")
	#add_subdirectory(python3)
endif()

if(GEN_JAVASCRIPT_BINDING)
	include(javascript/CMakeLists.txt)
endif()
