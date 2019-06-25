#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <iostream>
#include "boost/asio.hpp"
#include "Connection.hpp"
#include "LineBasedConnection.hpp"

namespace Mud {
namespace Server {

    class Server {
    public:
        Server(int port) :
            m_signal_set(m_io_service, SIGINT, SIGTERM),
            m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), port)),
            m_nextSocket(m_io_service) {
            m_signal_set.async_wait(
            [this](boost::system::error_code err, int sig) {
                std::cout << "Server received signal(" << sig << ")" << " requesting shutdown." << std::endl;
                m_acceptor.cancel();
            });
        }

        void Run() {
            Accept(); // Call ACCEPT Kicking off the first IO Call
            std::cout << "Server running..." << std::endl;
            m_io_service.run();
        }

    private:
        void Accept() {
            m_acceptor.async_accept(m_nextSocket,
            [this](boost::system::error_code err) {
                if (!err) {
                    m_connections.emplace_front(std::move(m_nextSocket));
                    auto connection = m_connections.begin();
                    
                    // Why not instead flag connections for removal internally via a time from last call?
                    // Server can force the flag to any state, triggering removal
                    // Then we don't need a handler, we just prune all connections checking for the flag.
                    
                    // CLEAN UP INACTIVE CONNECTIONS
                    int connections_cleaned = 0;
                    for (auto &connections : m_connections) {
                        if (!connection->Active()) {
                            m_connections.erase(connection);
                            connections_cleaned++;
                        }
                    }

                    if (connections_cleaned > 0) {
                        std::cout << "Total of " << connections_cleaned << " connections cleaned! Total Size is: " << m_connections.size() << "." << std::endl;
                    }

                    //connection->SetCloseHandler(
                    //[this, connection]() {
                    //    m_connections.erase(connection);
                    //});
                    std::cout << "Accepting a new connection! Total Size is: " << m_connections.size() << "." << std::endl;
                    Accept();
                }
            });
        }

        std::list<LineBasedConnection> m_connections;
        
        boost::asio::io_service m_io_service;
        boost::asio::signal_set m_signal_set;

        boost::asio::ip::tcp::acceptor m_acceptor;
        boost::asio::ip::tcp::socket m_nextSocket;
        
    };

} // End Server Namespace
} // End Mud Namespace
#endif // !SERVER_HPP


