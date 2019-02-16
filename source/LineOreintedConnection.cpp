#include "LineOreintedConnection.hpp"
#include <boost/asio.hpp>

using namespace MudServer;

void LineOreintedConnection::ReadLine() {
    async_read_until(Socket(), m_inputBuffer, '\n', 
    [this](boost::system::error_code err, std::size_t) {
        if (err) {
            std::cout << err << "\n\r";
        } else {
            std::string line;
            std::getline(m_inputStream, line);
            // m_interface.HandleLine(line);
            std::cout << "Receivied Line: " << line << std::endl;


            ReadLine();
        }
    });
}
