#include "common/async_logger.h"
#include "storage/cloud_storage.h"
#include <iostream>

int main() {
    AsyncLogger logger;
    logger.Init("logs");
    CloudStorage storage(logger, "data");

    storage.PutObject("hello.txt", "hello cloud storage");
    auto read = storage.GetObject("hello.txt");
    std::cout << "Read: " << read << "\n";
    storage.DeleteObject("hello.txt");

    logger.Log(LogLevel::kInfo, "Client done");
    return 0;
}
