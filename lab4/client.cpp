#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

void write(tcp::socket& socket)
{
	std::array<double, 4> value = {2.22, 3.33, 4.44, 5.55};
	boost::asio::write(socket, boost::asio::buffer(&value, sizeof(value)));
}

void read(tcp::socket& socket)
{
	std::array<int, 3> buffer;
	boost::system::error_code error;

	size_t len = socket.read_some(boost::asio::buffer(buffer), error);

	if (error == boost::asio::error::eof) 
	{
		std::cout << "Connection closed by server" << std::endl;
	}
	else if (error) 
	{
		std::cout << "Error: " << error.message() << std::endl;
	}
	else 
	{
		std::cout << "Received " << len << " bytes from server: ";
		for (int i = 0; i < buffer.size(); i++)
		{
			std::cout << buffer[i] << " ";
		}
		std::cout << std::endl;
	}
}

int main() {
	try
	{
		boost::asio::io_context io_context;

		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints = resolver.resolve("localhost", "12345");

		tcp::socket socket(io_context);
		boost::asio::connect(socket, endpoints);

		std::cout << "Connected to server!" << std::endl;

		write(socket);
		read(socket);
	}
	catch (const char* exception)
	{
		std::cerr << "Error: " << exception << '\n';
	}

	return 0;
}