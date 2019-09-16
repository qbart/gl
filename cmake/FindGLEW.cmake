message(FindGLEW.cmake)

set(GLEW_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/deps/glew)

find_path(GLEW_INCLUDE_DIR GL/glew.h
    PATH_SUFFIXES include
    PATHS ${GLEW_SEARCH_PATHS})

find_library(GLEW_LIBRARY NAMES glew32s
    PATH_SUFFIXES lib
    PATHS ${GLEW_SEARCH_PATHS})

find_library(GLEW_MX_LIBRARY NAMES glew32mxs
    PATH_SUFFIXES lib
    PATHS ${GLEW_SEARCH_PATHS})

set(GLEW_LIBRARIES ${GLEW_LIBRARY} ${GLEW_MX_LIBRARY})
set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
if(GLEW_LIBRARIES AND GLEW_INCLUDE_DIRS)
    set(GLEW_FOUND true)
endif()

add_definitions(-DGLEW_STATIC)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW REQUIRED_VARS GLEW_LIBRARIES GLEW_INCLUDE_DIRS)
