#include "Connection.hpp"
#include <iostream>

using namespace MudServer;

/*

*/

// Write outbound to a connection.
void Connection::ReadFromSocket() {

    boost::asio::async_read_until(m_socket, m_inputBuffer1, '\r',
        [me = shared_from_this()](boost::system::error_code err, std::size_t t) {
        // Bail on error.
        if (err) {
            std::cout << "async_read returned error!" << std::endl;
            return;
        } else {
            std::cout << "INPUT: " << me->ParseInputs(t) << "\n\r";
            me->Write("INPUT SENT!");
            me->Read();
        }
    });
}

void Connection::WriteToSocket() {

    if (m_writing) {
        m_moreToWrite = true;
        return;
    }

    std::swap(m_outputBufferPtr, m_bufferBeingWrittenPtr);
    std::swap(m_outputStreamPtr, m_StreamBeingWrittenPtr);

    m_writing = true;

    async_write(m_socket, *m_bufferBeingWrittenPtr,
        [me = shared_from_this()](boost::system::error_code err, std::size_t t) {
        me->m_writing = false;
        // Write
        if (err) {
            std::cout << "async_write returned error!" << std::endl;
        } else if (me->m_moreToWrite) {
            me->WriteToSocket();
            me->m_moreToWrite = false;
        }
    });
}