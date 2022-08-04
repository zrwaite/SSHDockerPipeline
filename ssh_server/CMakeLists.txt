add_library(
	ssh_server
	ssh_server.hpp
	ssh_server.cpp
	verify.hpp
	verify.cpp
	connection.hpp
	connection.cpp
)


include_directories(docker)
add_subdirectory(docker)
target_include_directories(docker PUBLIC docker)
