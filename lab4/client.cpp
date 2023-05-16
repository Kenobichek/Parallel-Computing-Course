#include <boost/asio.hpp>
#include <iostream>
#include "request.h"
#include "matrix.h"
#include "status.h"

using boost::asio::ip::tcp;

void sendDataToServer(tcp::socket& socket, const std::vector<std::vector<int>>& matrix) 
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

	std::cout << "Send data to server" << std::endl;
}

void sendRequestToStartComputing(tcp::socket& socket)
{
	Request request_type = Request::StartComputing;
	
	boost::asio::write(socket, boost::asio::buffer(&request_type, sizeof(int)));

	std::cout << "Send request to start computing" << std::endl;
}

void receiveResult(tcp::socket& socket, const int matrix_size) 
{
	while (true) 
	{
		std::cout << "Send GET request" << std::endl;

		Request request_type = Request::Get;
		boost::asio::write(socket, boost::asio::buffer(&request_type, sizeof(int)));
		
		Status result_status;
		boost::asio::read(socket, boost::asio::buffer(&result_status, sizeof(int)));

		if (result_status == Status::DataProcessed) {
			break;
		}

		std::cout << "Result is not ready yet" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(5));
	}

	std::this_thread::sleep_for(std::chrono::seconds(4));

	std::cout << "The result is ready" << std::endl;

	std::vector<std::vector<int>> result;

	for(int i = 0; i < matrix_size; i++)
	{
		result.push_back({});
		for(int j = 0; j < matrix_size; j++)
		{
			int value;
			boost::asio::read(socket, boost::asio::buffer(&value, sizeof(value)));
			result[i].push_back(value);
		}
	}
}

void sendRequestToEndConnection(tcp::socket& socket)
{
	Request request_type = Request::Exit;
	
	boost::asio::write(socket, boost::asio::buffer(&request_type, sizeof(int)));

	std::cout << "Send request to end connection" << std::endl;
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

		std::vector<std::vector<int>> matrix = Matrix::createMatrix(1000);

		sendDataToServer(socket, matrix);
		sendRequestToStartComputing(socket);
		receiveResult(socket, matrix.size());
		sendRequestToEndConnection(socket);
	}
	catch (const char* exception)
	{
		std::cerr << "Error: " << exception << '\n';
	}

	return 0;
}