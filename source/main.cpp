#include <iostream>

#include "Server.hpp"

using namespace Mud::Server;

namespace {
    const int SERVER_PORT = 4141;
}

int main() {

    Server server(SERVER_PORT);
    server.Run();

    std::cout << "Server Shutdown Normally." << std::endl;

    return 0;
}