cmake_minimum_required(VERSION 3.12)
project(MultiDockerServerPipeline VERSION 1.0.0)

include(.env.cmake)

include_directories(${LIBSSH_PATH}/include)
find_package(libssh CONFIG REQUIRED)

add_library(
	docker
	docker.hpp
	docker.cpp
)

add_executable(ssh_deploy main.cpp)

target_link_libraries(ssh_deploy docker)
target_link_libraries(ssh_deploy ssh)