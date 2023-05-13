#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using boost::asio::ip::tcp;

void write(tcp::socket& socket)
{
	std::array<int, 3> value = { 2, 3, 4 };
	boost::asio::write(socket, boost::asio::buffer(&value, sizeof(value)));
}

void read(tcp::socket& socket)
{
	std::array<double, 4> buffer;
	boost::system::error_code error;

	size_t len = socket.read_some(boost::asio::buffer(buffer), error);

	if (error == boost::asio::error::eof)
	{
		std::cout << "Connection closed by client" << std::endl;
	}
	else if (error) 
	{
		std::cout << "Error: " << error.message() << std::endl;
	}
	else 
	{
		std::cout << "Received " << len << " bytes from client: ";
		for (int i = 0; i < buffer.size(); i++)
		{
			std::cout << buffer[i] << " ";
		}
		std::cout << std::endl;
	}
}

void handleClient(tcp::socket socket) {
	std::cout << "Client connected!" << std::endl;
	write(socket);
	read(socket);
	socket.close();
}

int main() {
	try
	{
		boost::asio::io_context io_context;

		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));

		std::cout << "Server started, waiting for client to connect..." << std::endl;

		while (true)
		{
			tcp::socket socket(io_context);
			acceptor.accept(socket);
			std::thread(handleClient, std::move(socket)).detach();
		}
	}
	catch (const char* exception)
	{
		std::cerr << "Error: " << exception << '\n';
	}

	return 0;
}