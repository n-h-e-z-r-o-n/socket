cmake_minimum_required(VERSION 3.24)
project(met C)

set(CMAKE_C_STANDARD 11)

#add_executable(met Server_client/server.c)
#add_executable(met Server_client/client.c)

add_executable(met "File Write/data.c")