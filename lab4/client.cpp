#include <boost/asio.hpp>
#include <iostream>
#include "request.h"
#include "matrix.h"
#include "status.h"

using boost::asio::ip::tcp;

void sendRequest(tcp::socket& socket, const std::vector<std::vector<int>>& matrix) 
{
	boost::asio::streambuf request_buf;
	std::ostream request_stream(&request_buf);

	Request request_type = Request::SendData;
	request_stream.write((char*)&request_type, sizeof(int));

	int size = static_cast<int>(matrix.size());
	request_stream.write((char*)&size, sizeof(size));

	for (int i = 0; i < matrix.size(); i++) 
	{
		request_stream.write((char*)matrix[i].data(), matrix[i].size() * sizeof(int));
	}

	boost::asio::write(socket, request_buf);
}

void receiveResult(tcp::socket& socket) 
{
	while (true) 
	{
		Request request_type = Request::Get;
		boost::asio::write(socket, boost::asio::buffer(&request_type, sizeof(int)));
		
		std::this_thread::sleep_for(std::chrono::seconds(2));

		Status result_status;
		boost::asio::read(socket, boost::asio::buffer(&result_status, sizeof(int)));

		if (result_status == Status::DataProcessed) {
			break;
		}

		std::cout << "Result is not ready yet" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(5));
	}

	std::cout << "The result is ready" << std::endl;
	std::vector<std::vector<int>> result;
	boost::asio::read(socket, boost::asio::buffer(result));
	Matrix::print(result);
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

		std::vector<std::vector<int>> matrix = Matrix::createMatrix(3);

		sendRequest(socket, matrix);

		receiveResult(socket);
	}
	catch (const char* exception)
	{
		std::cerr << "Error: " << exception << '\n';
	}

	return 0;
}