#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <functional>

#include "request.h"
#include "matrix.h"

using boost::asio::ip::tcp;

void receiveData(tcp::socket& socket) 
{
		int matrix_size;
		boost::asio::read(socket, boost::asio::buffer(&matrix_size, sizeof(matrix_size)));

		std::cout << "Matrix size = " << matrix_size << std::endl;

		std::vector<std::vector<int>> matrix;

		for(int i = 0; i < matrix_size; i++)
		{
			matrix.push_back({});
			for(int j = 0; j < matrix_size; j++)
			{
				int value;
				boost::asio::read(socket, boost::asio::buffer(&value, sizeof(value)));
				matrix[i].push_back(value);
				std::cout << "value: " << value << "\n";
			}
		}
		Matrix::print(matrix);
}

void startComputing(tcp::socket& socket) 
{

}

void get(tcp::socket& socket) 
{

}

void handleRequest(tcp::socket socket) 
{

	Request request_type;
	boost::asio::read(socket, boost::asio::buffer(&request_type, sizeof(int)));

	std::cout << "Request type = " << request_type << std::endl;
	
	std::unordered_map<Request, std::function<void(tcp::socket&)>> request_handlers = 
	{
		{ Request::SendData, receiveData },
		{ Request::StartComputing, startComputing },
		{ Request::Get, get }
	};

	request_handlers[request_type](socket);
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
			std::thread(handleRequest, std::move(socket)).detach();
		}
	}
	catch (const char* exception)
	{
		std::cerr << "Error: " << exception << '\n';
	}

	return 0;
}