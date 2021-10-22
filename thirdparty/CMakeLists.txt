cmake_minimum_required(VERSION 3.10)

project(dependencies LANGUAGES CXX)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${PROJECT_SOURCE_DIR}/stb)

############################
### Fetch Dependencies  ####
############################

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
    set(GLAD_API "gl=" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
    set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")
    add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG		   0.9.9.8
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
	FetchContent_Populate(glm)
	set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
	add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.83
)
FetchContent_MakeAvailable(imgui)

list(APPEND IMGUI_SOURCES
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"
    )

list(APPEND IMGUI_PUBLIC_INCLUDE_DIRS
    ${imgui_SOURCE_DIR}
    )

add_library(imgui_lib STATIC ${IMGUI_SOURCES})
target_link_libraries(imgui_lib PUBLIC glfw glad)
target_include_directories(imgui_lib PUBLIC ${IMGUI_PUBLIC_INCLUDE_DIRS})

FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG        v0.11
)
FetchContent_MakeAvailable(implot)

list(APPEND IMPLOT_PUBLIC_INCLUDE_DIRS
    ${implot_SOURCE_DIR}
    )

list(APPEND IMPLOT_SOURCES
    "${implot_SOURCE_DIR}/implot.cpp"
    "${implot_SOURCE_DIR}/implot_items.cpp"
    "${implot_SOURCE_DIR}/implot_demo.cpp"
    )

add_library(implot_lib STATIC ${IMPLOT_SOURCES})
target_link_libraries(implot_lib PUBLIC glfw glad imgui_lib)
target_include_directories(implot_lib PUBLIC ${IMPLOT_PUBLIC_INCLUDE_DIRS})

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE glad glm glfw stb_image imgui_lib implot_lib)