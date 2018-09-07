// WINDOWS DEFINES

// INCLUDE

#include <iostream>
#include "Server.hpp"


namespace MudServer {
	const int SERVER_PORT = 4141;
	// const std::string NEW_LINE = "\r\n";
}

int main() {	
	MudServer::Server server(MudServer::SERVER_PORT);
	server.Run();

	std::cout << "Server shutdown, terminating application normally." << std::endl;
	return 0;
}