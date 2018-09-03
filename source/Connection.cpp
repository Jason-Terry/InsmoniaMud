#include "Connection.hpp"
#include <iostream>

using namespace MudServer;

void Connection::WriteToSocket() {

	if (m_writing) {
		m_moreToWrite = true;
		return;
	}

	std::swap(m_outputBuffer, m_bufferBeingWritten);
	std::swap(m_outputStream, m_StreamBeingWritten);

	m_writing = true;

	async_write(m_socket, *m_bufferBeingWritten,
		[this](boost::system::error_code err, std::size_t) {
		m_writing = false;
		// Write
		if (err) {
			std::cout << "async_write returned error!" << std::endl;
		} else if (m_moreToWrite) {
			WriteToSocket();
			m_moreToWrite = false;
		}

	});
}