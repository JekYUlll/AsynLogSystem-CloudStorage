cmake_minimum_required(VERSION 3.16)
project(AsyncLogCloud LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# libevent
set(EVENT__DISABLE_TESTS ON CACHE BOOL "" FORCE)
set(EVENT__DISABLE_BENCHMARK ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    libevent
    URL https://github.com/libevent/libevent/archive/refs/tags/release-2.1.12-stable.tar.gz
)
FetchContent_MakeAvailable(libevent)

# jsoncpp
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "" FORCE)
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    jsoncpp
    URL https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.5.tar.gz
)
FetchContent_MakeAvailable(jsoncpp)

# base64 (ReneNyffenegger/cpp-base64 header-only)
FetchContent_Declare(
    cpp_base64
    URL https://github.com/ReneNyffenegger/cpp-base64/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(cpp_base64)
add_library(base64 INTERFACE)
target_include_directories(base64 INTERFACE ${cpp_base64_SOURCE_DIR})

# bundle (header-only, archived mirror)
FetchContent_Declare(
    bundle
    GIT_REPOSITORY https://github.com/r-lyeh-archived/bundle.git
    GIT_TAG master
)
FetchContent_MakeAvailable(bundle)
add_library(bundle INTERFACE)
target_include_directories(bundle INTERFACE ${bundle_SOURCE_DIR})

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
target_link_libraries(client PRIVATE async_log cloud_storage event jsoncpp_lib base64 bundle)

add_executable(server src/server/main.cpp)
target_link_libraries(server PRIVATE async_log cloud_storage event jsoncpp_lib base64 bundle)
