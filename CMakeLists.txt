cmake_minimum_required(VERSION 3.16)
project(AsyncLogCloud LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(async_log
    src/common/async_logger.cpp
    src/common/log_sink.cpp
    src/common/thread_pool.cpp
)
target_include_directories(async_log PUBLIC src)

add_library(cloud_storage
    src/storage/cloud_storage.cpp
    src/storage/metadata_store.cpp
)
target_link_libraries(cloud_storage PUBLIC async_log)

add_executable(client src/client/main.cpp)
target_link_libraries(client PRIVATE async_log cloud_storage)

add_executable(server src/server/main.cpp)
target_link_libraries(server PRIVATE async_log cloud_storage)
