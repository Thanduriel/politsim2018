cmake_minimum_required (VERSION 3.8)
project (politsim)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find all source and header files
file(GLOB_RECURSE SOURCE_FILES "src/*.hpp" "src/*.cpp" )

# organize the files according to the organisation in the file system
# from: http://cmake.3232098.n2.nabble.com/Keep-folder-structure-in-Visual-Studio-Project-generated-with-CMake-td7586044.html
macro(GroupSources curdir)
   file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
   foreach(child ${children})
          if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
                  GroupSources(${curdir}/${child})
          else()
                  string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
          endif()
   endforeach()
endmacro()

# execute tha macro
GroupSources(src)

include_directories(src)
include_directories(godot_headers)
add_executable(politsim ${SOURCE_FILES} demo/demo.cpp)

include_directories("src")