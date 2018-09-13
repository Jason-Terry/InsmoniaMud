#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdarg>
#include <iostream>
#include <queue>


/*
	The Connect Class contains 
	the input / output logic of a individual connection. 

	Prompt() - Displays the prompt, and caputres input into the inputBuffer
	Output() - Outputs the result of the engine tick to the connection.


*/

namespace MudServer {

	// The CONNECTION_STATUS should act as a state management tool to keep the connection doing the correct logic processing
	// given a certain state.
	enum CONNECTION_STATUS {
		NEW_CONNECTION = 0,		// request user name -> if valid user name -> request password
		USER_VERIFIED,			// user verification okay -> do user_menu()
		ACTIVE_PLAYER,			// user selected a valid character -> do game()
		CHAR_GEN_MENU,			// user selected invalid character name -> do char_gen() 
	};

	class Connection : public boost::enable_shared_from_this<Connection> {
	public:
		typedef boost::asio::ip::tcp::socket SocketType;
		explicit Connection(boost::asio::io_service &io_service)
			: m_socket(io_service),
			m_outputStream1(&m_outputBuffer1),
			m_outputStream2(&m_outputBuffer2),
			m_outputBufferPtr(&m_outputBuffer1),
			m_outputStreamPtr(&m_outputStream1),
			m_bufferBeingWrittenPtr(&m_outputBuffer2),
			m_StreamBeingWrittenPtr(&m_outputStream2),
			m_writing(false), m_moreToWrite(false) {
			
			boost::uuids::basic_random_generator<boost::mt19937> gen;
			m_connectionId = gen();
			std::cout << "Created connection waiting for session start\r\n";

			// Wat
		}

		SocketType &Socket() {
			return m_socket;
		}
		void Read() {
			ReadFromSocket();
		}

		// Write should output to the client, until it's done, then flush the buffer, and swap it with the inputBuffer
		void Write(const std::string &message) {
			*m_outputStreamPtr << message << "\r\n";
			WriteToSocket();
		}

		void Write(std::vector<std::string> args) {
			for (auto i = 0; i < args.size(); ++i) {
				*m_outputStreamPtr << args.at(i) << "\r\n";
			}
			WriteToSocket();
		}

		void Start() {
			
			// std::cout << "new tcp_connection session assigned id: " << to_string(m_connectionId) << "\r\n";
			Prompt();
		}

		std::string ParseInputs() {
			std::stringstream ss;
			// Some Input Parsing
			// ss << m_inputStream1.getline();
			// Some Input Parsing
			return ss.str();
		}

	private:


		void Prompt() {
			Write("WELCOME USER! ");
			Read();
		};

		

		void ReadFromSocket();
		void WriteToSocket();

		SocketType m_socket;

		// OUTPUT DOUBLE BUFFER
		boost::asio::streambuf m_outputBuffer1, m_outputBuffer2;
		std::ostream m_outputStream1, m_outputStream2;
		
		// POINTERS
		boost::asio::streambuf *m_outputBufferPtr, *m_bufferBeingWrittenPtr;
		std::ostream *m_outputStreamPtr, *m_StreamBeingWrittenPtr;

		boost::asio::streambuf m_inputBuffer1;

		boost::uuids::uuid m_connectionId;

		boost::system::error_code m_error_code;
		std::size_t m_data_size;


		bool m_writing, m_moreToWrite;

	};

} // end Namespace MudServer
#endif // !_CONNECTION_HPP_
