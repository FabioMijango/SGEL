cmake_minimum_required(VERSION 3.28)
project(01_Example_BasicWindow)

file(GLOB_RECURSE EXAMPLE_SRC "*.cpp" "*.hpp")

add_executable(${PROJECT_NAME} ${EXAMPLE_SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE SGEL)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/example/${PROJECT_NAME}
)

if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/res" DESTINATION "${CMAKE_BINARY_DIR}/bin/examples")
endif()