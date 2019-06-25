#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>

#include "boost/asio.hpp"

namespace Mud {
    namespace Server {
        class Connection {
        public:
            typedef boost::asio::ip::tcp::socket SocketType;

            explicit Connection(SocketType &&socket)
                :   m_socket(std::move(socket)),
                    m_outputStream1(&m_outputBuffer1),
                    m_outputStream2(&m_outputBuffer2),
                    m_outputStream(&m_outputStream1),
                    m_outputBuffer(&m_outputBuffer1),
                    m_streamBeingWritten(&m_outputStream2),
                    m_bufferBeingWritten(&m_outputBuffer2),
                    m_writing(false),
                    m_moreToWrite(false),
                    m_reading(true) {

            }

            template <class T>
            void Write(const T &msg) {
                *m_outputStream << msg;
                WriteToSocket();
            }

            template <class T>
            std::ostream &operator<<(const T &msg) {
                Write(msg);
                m_moreToWrite = true;
                return *m_outputStream;
            }

            std::ostream &ostream() {
                WriteToSocket();
                m_moreToWrite = true;
                return *m_outputStream;
            }

            //template <class Handler>
            //void SetCloseHandler(Handler &&handler) {
            //    m_closeHandler = std::forward<Handler>(handler);
            //}

            void Close() {
                // ???
                SetInactive();
            }

            bool Active() {
                return m_active;
            }
            
            // Either IS active, or is NOT active.
            void SetActive() {
                m_active = true;
            }

            void SetInactive() {
                m_active = false;
            }

        protected:
            void DoneReading() {
                m_reading = false;
                WriteToSocket();
            }

            SocketType m_socket;

        private:

            void WriteToSocket();

            bool m_reading, m_writing, m_moreToWrite, m_active;

            // -- Handlers
            std::function<void()> m_closeHandler;

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
