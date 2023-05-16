#include <boost/asio.hpp>
#include <iostream>
#include "request.h"
#include "matrix.h"

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

std::vector<std::vector<int>> receiveResult(tcp::socket& socket, const int size) 
{
	int status;
	boost::asio::read(socket, boost::asio::buffer(&status, sizeof(status)));

	if (status == 0) {

		std::cout << "Result is not ready yet" << std::endl;

		while (true) 
		{
			int request_type = Request::Get;
			boost::asio::write(socket, boost::asio::buffer(&request_type, sizeof(request_type)));

			int result_status;
			boost::asio::read(socket, boost::asio::buffer(&result_status, sizeof(result_status)));
			if (result_status == 1) {
				break;
			}

			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
	}

	std::vector<std::vector<int>> result;


    // for (int i = 0; i < numRows; i++) {
    //     std::vector<int> row;
    //     for (int j = 0; j < numCols; j++) {
    //         int value;
    //         std::memcpy(&value, buffer, sizeof(int));
    //         buffer += sizeof(int);
    //         row.push_back(value);
    //     }
    //     matrix.push_back(row);
    // }

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

		std::vector<std::vector<int>> matrix = Matrix::createMatrix(3);

		Matrix::print(matrix);
		sendRequest(socket, matrix);

		//std::vector<std::vector<int>> result = receiveResult(socket, matrix.size());

	}
	catch (const char* exception)
	{
		std::cerr << "Error: " << exception << '\n';
	}

	return 0;
}