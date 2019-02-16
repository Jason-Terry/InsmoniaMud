#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include "Connection.hpp"
#include "LineOreintedConnection.hpp"

/*
    The Server Object accepts, manages, and contains all active connections. 
*/

namespace MudServer {

    class Server {
    public:
        typedef boost::shared_ptr<LineOreintedConnection> sptr;
        
        // CONSTRUCTOR
        Server(int port) : m_signal_set(m_io_service, SIGINT, SIGTERM), m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), port)) {
            
            // Server Constructor
            m_signal_set.async_wait( [this](boost::system::error_code err, int signal) {
                if (err) {
                    std::cout << "Error (" << err << ")\n";
                } else {
                    std::cout << "Server received signal (" << signal << ")" << " requesting shutdown.\n";
                }
                m_acceptor.cancel();
            });
        }; // end Server()

        void Run() {
            Accept();
            std::cout << "Server is running..." << std::endl;
            m_server_running = true;
            m_io_service.run();
        }; // end Run()

    private:

        void Accept() {
            m_connections.push_back(boost::make_shared<LineOreintedConnection>(m_io_service));
            auto connection = m_connections.back();
            m_acceptor.async_accept(connection->Socket(),
                [this, connection](boost::system::error_code err) {
                if (!err) {
                    std::cout << "Connection made!\n";
                    std::cout << "Total Active Connections (" << m_connections.size() << ")\n";
                    connection->Start(); // START THE CONNECTION!
                    Accept();
                };
            });
        }; // end Accept()

        bool m_server_running;
        boost::asio::io_service m_io_service;
        boost::asio::ip::tcp::acceptor m_acceptor;
        boost::asio::signal_set m_signal_set;
        std::list<sptr> m_connections;

    };	// end Server Class
} // end Namespace MudServer
#endif // _SERVER_HPP_
