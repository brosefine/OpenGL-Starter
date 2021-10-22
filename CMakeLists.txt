# CMake OpenGL Tutorial by Tomasz Gałaj
# https://github.com/Shot511/OpenGLSampleCmake

cmake_minimum_required(VERSION 3.17)

project(OpenGL VERSION 1.0 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(thirdparty)

# Configure assets header file
configure_file(${CMAKE_SOURCE_DIR}/include/helpers/RootDir.h.in ${CMAKE_BINARY_DIR}/include/helpers/RootDir.h)

# add source files
file(GLOB_RECURSE SRC_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# add header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/include/*.hpp
        ${CMAKE_SOURCE_DIR}/include/*.h)

add_library(SOURCE ${SRC_FILES} ${HEADER_FILES})
target_include_directories(SOURCE PUBLIC ${CMAKE_SOURCE_DIR}/include/ ${CMAKE_BINARY_DIR}/include/)
target_link_libraries(SOURCE dependencies)


file(GLOB EXAMPLE_FILES
        ${CMAKE_SOURCE_DIR}/app/ExampleApp/ex_main.cpp
        ${CMAKE_SOURCE_DIR}/app/ExampleApp/ex_app.h
        ${CMAKE_SOURCE_DIR}/app/ExampleApp/ex_app.cpp)

add_executable(EXAMPLE_main ${EXAMPLE_FILES})
target_include_directories(EXAMPLE_main PUBLIC ${CMAKE_SOURCE_DIR}/app/ExampleApp/)
target_link_libraries(EXAMPLE_main SOURCE dependencies)

file(GLOB SHADERS 
        ${CMAKE_SOURCE_DIR}/resources/shaders/*)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/saves)


# Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
        # Macro to preserve source files hierarchy in the IDE
        # Note: files have to be listed in add_library(...) or add_executable(...)
        #       files included via target_include_directories(..) WILL NOT show!
        macro(GroupSources curdir)
                file(GLOB children RELATIVE ${CMAKE_SOURCE_DIR}/${curdir} ${CMAKE_SOURCE_DIR}/${curdir}/*)

                foreach(child ${children})
                        if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${curdir}/${child})
                                GroupSources(${curdir}/${child})
                        else()
                                string(REPLACE "/" "\\" groupname ${curdir})
                                source_group(${groupname} FILES ${CMAKE_SOURCE_DIR}/${curdir}/${child})
                        endif()
                endforeach()
        endmacro()

        # Run macro
        GroupSources(include)
        GroupSources(src)
        GroupSources(app)
        # Create a symlink to .sln file to make switching between folder and solution view in MSVC easier
        # Note: not working because MSVC fails to open .sln file via symlink from folder view -__-
        #execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sln ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.sln)
endif()
