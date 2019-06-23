#include "Connection.hpp"


using namespace Mud::Server;

void Connection::WriteToSocket() {
    if (m_writing) {
        m_moreToWrite = true;
        return;
    }

    m_writing = true;

    std::swap(m_outputBuffer, m_bufferBeingWritten);
    std::swap(m_outputStream, m_streamBeingWritten);

    async_write(m_socket, *m_bufferBeingWritten,
        [this](boost::system::error_code err, std::size_t) {
        
        m_writing = false;
        
        if (err) {
            std::cout << "async_write returned an error." << std::endl;
        } else if (m_moreToWrite) {
            WriteToSocket();
            m_moreToWrite = false;
            return;
        }

        if (!m_reading) {
            m_closeHandler();
        }
    });

}