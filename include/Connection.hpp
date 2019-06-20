#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>

#include "boost/asio.hpp"

namespace Mud {
    namespace Server {
        class Connection {
        public:
            typedef boost::asio::ip::tcp::socket SocketType;

             explicit Connection(boost::asio::io_service &io_service)
                :   m_socket(io_service),
                    m_outputStream1(&m_outputBuffer1),
                    m_outputStream2(&m_outputBuffer2),
                    m_outputStream(&m_outputStream1),
                    m_outputBuffer(&m_outputBuffer1),
                    m_streamBeingWritten(&m_outputStream2),
                    m_bufferBeingWritten(&m_outputBuffer2),
                    m_writing(false),
                    m_moreToWrite(false) {

            }

             void Write(const std::string &msg) {
                 *m_outputStream << msg;
                 WriteToSocket();
             }

            void Start() {
                Write("Connection Started!");
            
            }

            SocketType &Socket() { return m_socket;  }

        private:

            void WriteToSocket();

            bool m_writing, m_moreToWrite;
            SocketType m_socket;

            // Primary Output Stream Buffers
            std::ostream m_outputStream1, m_outputStream2;
            boost::asio::streambuf m_outputBuffer1, m_outputBuffer2;
            
            // Pointers to buffers
            boost::asio::streambuf *m_outputBuffer, *m_bufferBeingWritten;
            std::ostream *m_outputStream, *m_streamBeingWritten;


        };
    }
}


#endif // !CONNECTION_HPP
