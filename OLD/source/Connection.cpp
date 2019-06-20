#include "Connection.hpp"
#include <iostream>

using namespace MudServer;

/*

*/

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
            std::cout << "an async_write returned error!\n";
        } else if (me->m_moreToWrite) {
            me->WriteToSocket();
            me->m_moreToWrite = false;
        }
    });
}