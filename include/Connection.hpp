#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdarg>

namespace MudServer {
	class Connection {
	public:
		typedef boost::asio::ip::tcp::socket SocketType;
		explicit Connection(boost::asio::io_service &io_service) 
			: m_socket(io_service), 
			m_outputStream1(&m_outputBuffer1), 
			m_outputStream2(&m_outputBuffer2),
			m_outputBuffer(&m_outputBuffer1),
			m_outputStream(&m_outputStream1),
			m_bufferBeingWritten(&m_outputBuffer2),
			m_StreamBeingWritten(&m_outputStream2),
			m_writing(false), m_moreToWrite(false) {

			// Wat
		}
		
		SocketType &Socket() {
			return m_socket;
		}

		// Write should output to the client, until it's done, then flush the buffer, and swap it with the inputBuffer
		void Write(const std::string &message) {
			*m_outputStream << message;
			WriteToSocket();
		}

		void Write(std::vector<std::string> args) {
			for (auto i = 0; i < args.size(); ++i) {
				*m_outputStream << args.at(i) << std::endl;
			}
			WriteToSocket();
		}

		void Start() {
			boost::uuids::basic_random_generator<boost::mt19937> gen;
			m_connectionId = gen();
			
			Write({ "Test ", "two ", "three ", "four " });
			Write("Welcome User!");
		}

	private:

		void WriteToSocket();

		SocketType m_socket;
		boost::asio::streambuf m_outputBuffer1, m_outputBuffer2;
		std::ostream m_outputStream1, m_outputStream2;
		boost::asio::streambuf *m_outputBuffer, *m_bufferBeingWritten;
		std::ostream *m_outputStream, *m_StreamBeingWritten;

		boost::uuids::uuid m_connectionId;

		bool m_writing, m_moreToWrite;

	};

} // end Namespace MudServer
#endif // !_CONNECTION_HPP_
