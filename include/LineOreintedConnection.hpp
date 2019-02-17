#ifndef _LINE_OREINTED_CONNECTION_HPP_
#define _LINE_OREINTED_CONNECTION_HPP_

#include "Connection.hpp"
#include <iostream>

namespace MudServer {

    class LineOreintedConnection : public Connection {

    public:
        LineOreintedConnection(SocketType &&socket) :
            Connection(std::move(socket)),
            m_inputStream(&m_inputBuffer) {
            Write("Welcome User!");
            ReadLine();
        }


    private:
        boost::asio::streambuf m_inputBuffer;
        std::istream m_inputStream;
    
        // Read single line from inputstream
        void ReadLine();
    
    
    
    
    }; // End LineOreintedConnection
} // End MudServer




#endif // end _LINE_OREINTED_CONNECTION_HPP_ 