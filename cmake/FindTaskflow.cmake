message(FindTaskflow.cmake)

set(TASKFLOW_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/deps/taskflow)

find_path(TASKFLOW_INCLUDE_DIR taskflow.hpp
    PATH_SUFFIXES include
    PATHS ${TASKFLOW_SEARCH_PATHS})
set(TASKFLOW_INCLUDE_DIRS ${TASKFLOW_INCLUDE_DIR})
message("..dir.include: " ${TASKFLOW_INCLUDE_DIRS})

file(GLOB_RECURSE TASKFLOW_SRC_FILES
    ${TASKFLOW_SEARCH_PATHS}/*.hpp
)
message("..dir.src: " ${TASKFLOW_SRC_FILES})

source_group("taskflow" FILES ${TASKFLOW_SRC_FILES})
message("\n")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TASKFLOW REQUIRED_VARS TASKFLOW_INCLUDE_DIRS TASKFLOW_SRC_FILES)
