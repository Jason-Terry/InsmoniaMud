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
            Accept();
            std::cout << "Server running..." << std::endl;
            m_io_service.run();
        }

        void DeleteConnection(std::list<LineBasedConnection>::iterator connection) {
            m_connections.erase(connection);
        }

    private:
        void Accept() {
            m_acceptor.async_accept(m_nextSocket,
            [this](boost::system::error_code err) {
                if (!err) {
                    m_connections.emplace_front(std::move(m_nextSocket));
                    auto connection = m_connections.begin();
                    connection->SetCloseHandler(
                    [this, connection]() {
                        m_connections.erase(connection);
                        std::cout << "Deleting a old connection!\n\rTotal Size is: " << m_connections.size() << std::endl;
                    });
                    std::cout << "Accepting a new connection!\n\rTotal Size is: " << m_connections.size() << std::endl;
                    Accept();
                }
            });
        }

        boost::asio::io_service m_io_service;
        boost::asio::signal_set m_signal_set;

        boost::asio::ip::tcp::acceptor m_acceptor;
        boost::asio::ip::tcp::socket m_nextSocket;
        
        std::list<LineBasedConnection> m_connections;
    };

} // End Server Namespace
} // End Mud Namespace
#endif // !SERVER_HPP


