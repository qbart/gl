message(FindImGui.cmake)

set(IMGUI_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/deps/imgui)

find_path(IMGUI_INCLUDE_DIR imconfig.h
    PATH_SUFFIXES include
    PATHS ${IMGUI_SEARCH_PATHS})
set(UI_INCLUDE_DIRS ${IMGUI_INCLUDE_DIR})
message("..dir.include: " ${UI_INCLUDE_DIRS})

file(GLOB_RECURSE UI_SRC_FILES
    ${IMGUI_SEARCH_PATHS}/*.h
    ${IMGUI_SEARCH_PATHS}/*.cpp
)
message("..dir.src: " ${UI_SRC_FILES})

source_group("imgui" FILES ${UI_SRC_FILES})
message("\n")

add_definitions(-DUI_IMGUI)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IMGUI REQUIRED_VARS UI_INCLUDE_DIRS UI_SRC_FILES)
