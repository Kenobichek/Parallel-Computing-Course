#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <functional>
#include "request.h"
#include "matrix.h"
#include "status.h"

using boost::asio::ip::tcp;

std::unordered_map<tcp::socket*, Status> server_status_map;
std::unordered_map<tcp::socket*, std::vector<std::vector<int>>> matrix_map;

bool receiveData(tcp::socket& socket) 
{
    int matrix_size;
    boost::asio::read(socket, boost::asio::buffer(&matrix_size, sizeof(matrix_size)));

    std::cout << "Matrix size = " << matrix_size << std::endl;

    std::vector<std::vector<int>> matrix;

    for (int i = 0; i < matrix_size; i++)
    {
        matrix.push_back({});
        for (int j = 0; j < matrix_size; j++)
        {
            int value;
            boost::asio::read(socket, boost::asio::buffer(&value, sizeof(value)));
            matrix[i].push_back(value);
        }
    }

    server_status_map[&socket] = Status::DataNotYetProcessed;
    matrix_map[&socket] = std::move(matrix);

    std::cout << "Data received on the server from the client" << std::endl;

    return true;
}

bool startComputing(tcp::socket& socket) 
{
    while (server_status_map[&socket] != Status::DataNotYetProcessed) {}

    server_status_map[&socket] = Status::DataProcessing;
    std::cout << "Start computing" << std::endl;

    Matrix::placeMaxRowElementsOnMainDiagonal(matrix_map[&socket], 1);

    server_status_map[&socket] = Status::DataProcessed;
    std::cout << "End computing" << std::endl;

    return true;
}

bool get(tcp::socket& socket) 
{
    boost::asio::streambuf request_buf;
    std::ostream request_stream(&request_buf);

    request_stream.write((char*)&server_status_map[&socket], sizeof(int));

    if (server_status_map[&socket] == Status::DataProcessed)
    {
        std::vector<std::vector<int>>& matrix = matrix_map[&socket];

        for (int i = 0; i < matrix.size(); i++)
        {
            request_stream.write((char*)matrix[i].data(), matrix[i].size() * sizeof(int));
        }

        std::cout << "Sent data from server to client" << std::endl;
    }

    boost::asio::write(socket, request_buf);

    return true;
}

bool endConnection(tcp::socket& socket)
{
    std::cout << "End connection" << std::endl;

    server_status_map.erase(&socket);
    matrix_map.erase(&socket);
	
    return false;
}

bool handleRequest(tcp::socket& socket) 
{
    Request request_type;
    boost::asio::read(socket, boost::asio::buffer(&request_type, sizeof(int)));

    std::cout << "Request type = " << request_type << std::endl;

    std::unordered_map<Request, std::function<bool(tcp::socket&)>> request_handlers = 
    {
        { Request::SendData, receiveData },
        { Request::StartComputing, startComputing },
        { Request::Get, get },
        { Request::Exit, endConnection }
    };

    return request_handlers[request_type](socket);
}

void handleClient(tcp::socket&& socket) 
{
    std::cout << "Client connected!" << std::endl;

    while (handleRequest(socket)) {}
    
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
