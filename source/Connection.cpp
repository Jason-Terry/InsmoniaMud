#include "Connection.hpp"
#include <iostream>

using namespace MudServer;

/*
	
*/

// Write outbound to a connection.
void Connection::ReadFromSocket() {
	boost::asio::async_read_until(m_socket, m_inputBuffer1, '\r', 
	[this](boost::system::error_code err, std::size_t) {
		// auto me = this->shared_from_this();
		std::cout << "INPUT: " << this->ParseInputs();
		this->Write("INPUT SENT!");
	});
}

void Connection::WriteToSocket() {

	if (m_writing) {
		m_moreToWrite = true;
		return;
	}

	m_writing = true;

	async_write(m_socket, *m_bufferBeingWrittenPtr,
		[this](boost::system::error_code err, std::size_t) {
		m_writing = false;
		// Write
		if (err) {
			std::cout << "async_write returned error!" << std::endl;
		} else if (m_moreToWrite) {
			WriteToSocket();
			m_moreToWrite = false;
		} else {
			this->Read();
		}

	});
}