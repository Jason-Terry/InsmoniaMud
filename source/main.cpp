#include <iostream>
#include "Server.hpp"

namespace {
	const int SERVER_PORT = 4141;
}

int main() {	
	MudServer::Server server(SERVER_PORT);
	server.Run();

	std::cout << "Server shutdown, terminating application normally." << std::endl;
	return 0;
}