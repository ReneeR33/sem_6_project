cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -pedantic -ggdb -O0 -Iproduct -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

project(car-controller)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB SOURCES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
		"${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} -pthread)