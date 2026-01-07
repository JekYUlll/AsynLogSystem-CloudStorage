#include "common/async_logger.h"
#include "storage/cloud_storage.h"

int main() {
    AsyncLogger logger;
    logger.Init("logs");
    CloudStorage storage(logger, "data");
    logger.Log(LogLevel::kInfo, "Server starting (HTTP/gRPC TODO)");
    // TODO: add networking (e.g., cpp-httplib/gRPC) and request handlers.
    return 0;
}
