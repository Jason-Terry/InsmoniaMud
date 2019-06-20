#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <iostream>

#include "boost/asio.hpp"

#include "Connection.hpp"

namespace Mud {
namespace Server {

    class Server {
        // Constructor
    public:
        
        Server(int port) 
            : m_signal_set(m_io_service, SIGINT, SIGTERM),
              m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(
                                        boost::asio::ip::tcp::v6(), port))
        {
            m_signal_set.async_wait(
                [this](boost::system::error_code err, int sig) {
                std::cout << "Server received signal(" << sig << ")"
                    << " requesting shutdown." << std::endl;
                m_acceptor.cancel();
            }
            );
        
        }

        void Run() {
            Accept();
            std::cout << "Server running..." << std::endl;
            m_io_service.run();
        }



    private:

        // Accept a inbound connection.
        void Accept() {
            
            // Why use emplace_back instead of push_back
            m_connections.emplace_back(m_io_service);
            auto &connection = m_connections.back();

            // async_accept
            m_acceptor.async_accept(connection.Socket(),
                [this, &connection](boost::system::error_code err) {
                if (!err) {
                    
                    std::cout << "Accepting a new connection!" << std::endl;
                    connection.Start();
                    // connection;

                    Accept();
                } 
            }
            );
        }

        boost::asio::io_service m_io_service;
        boost::asio::ip::tcp::acceptor m_acceptor;
        boost::asio::signal_set m_signal_set;
        std::list<Connection> m_connections;


    };

} // End Server Namespace
} // End Mud Namespace






#endif // !SERVER_HPP


