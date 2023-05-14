#include <boost/asio.hpp>
#include <iostream>
#include "request.h"

using boost::asio::ip::tcp;

void sendRequest(tcp::socket& socket, const std::vector<std::vector<int>>& matrix) 
{

	boost::asio::streambuf request_buf;
	std::ostream request_stream(&request_buf);

	Request request_type = Request::SendData;
	request_stream.write((char*)&request_type, 4);

	int size = static_cast<int>(matrix.size());
	request_stream.write((char*)&size, 4);

	for (int i = 0; i < matrix.size(); i++) 
	{
		request_stream.write((char*)matrix[i].data(), matrix[i].size() * sizeof(int));
	}

	boost::asio::write(socket, request_buf);
}

std::vector<int> receiveResult(tcp::socket& socket) 
{

	int status;
	boost::asio::read(socket, boost::asio::buffer(&status, sizeof(status)));

	if (status == 0) {
	std::cout << "Result is not ready yet" << std::endl;

		while (true) 
		{
			int request_type = 2; // Request result
			boost::asio::write(socket, boost::asio::buffer(&request_type, sizeof(request_type)));

			int result_status;
			boost::asio::read(socket, boost::asio::buffer(&result_status, sizeof(result_status)));
			if (result_status == 1) {
				break;
			}

			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
	}


	std::vector<int> result;
	boost::asio::read(socket, boost::asio::buffer(result));

	return result;
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

		std::vector<std::vector<int>> matrix = {
			{ 1, 2, 3 },
			{ 4, 5, 6 },
			{ 7, 8, 9 },
		};

		sendRequest(socket, matrix);

		// std::vector<int> result = receiveResult(socket);

		// int size = static_cast<int>(std::sqrt(matrix.size()));

		// for (int i = 0; i < size; i++) {
		// 	for (int j = 0; j < size; j++) {
		// 		std::cout << result[i * size + j] << " ";
		// 	}
		// 	std::cout << std::endl;
		// }
	}
	catch (const char* exception)
	{
		std::cerr << "Error: " << exception << '\n';
	}

	return 0;
}