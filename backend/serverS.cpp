#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

// Размер буфера для чтения данных
#define BUFFER_SIZE 1024

// Функция для инициализации Winsock
bool initWinsock() {
    setlocale(LC_ALL, "Russian");
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "Initialization error Winsock: " << result << std::endl;
        return false;
    }
    return true;
}

// Функция для создания серверного сокета
SOCKET createServerSocket() {
    setlocale(LC_ALL, "Russian");
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Creation Error сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    }
    return serverSocket;
}

// Функция для привязки и прослушивания сокета
void bindAndListen(SOCKET serverSocket, int port) {
    setlocale(LC_ALL, "Russian");
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Принимать соединения на всех интерфейсах
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Socket binding error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listening Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    std::cout << "Server listens on port " << port << std::endl;
}

// Функция для чтения содержимого HTML-файла
std::string readHtmlFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Функция для обработки GET-запроса
void handleGetRequest(SOCKET clientSocket) {
    setlocale(LC_ALL, "Russian");
    std::string htmlContent = readHtmlFile("D:\\Учеба\\Проект\\frontend\\index.html");
    if (htmlContent.empty()) {
        std::cerr << "Error reading HTML file" << std::endl;
        return;
    }

    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "\r\n" + htmlContent;

    send(clientSocket, response.c_str(), response.size(), 0);
}

// Функция для обработки POST-запроса
void handlePostRequest(SOCKET clientSocket, const std::string& request) {
    setlocale(LC_ALL, "Russian");
    std::string::size_type bodyPos = request.find("\r\n\r\n");

    if (bodyPos != std::string::npos) {
        std::string body = request.substr(bodyPos + 4);  // Тело POST-запроса
        std::cout << "Request body: \n" << body << std::endl;

        
        std::string::size_type functionPos = body.find("name=\"function\"");
        if (functionPos != std::string::npos) {
            std::string::size_type valueStart = body.find("\r\n\r\n", functionPos) + 4;
            std::string::size_type valueEnd = body.find("\r\n", valueStart);
            std::string functionValue = body.substr(valueStart, valueEnd - valueStart);
            std::cout << "Function value: " << functionValue << std::endl;

            
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                "Access-Control-Allow-Headers: Content-Type\r\n"
                "Content-Length: " + std::to_string(functionValue.size() + 28) + "\r\n"
                "\r\n"
                "{\"status\":\"success\",\"data\":\"" + functionValue + "\"}";

            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }
    else {
        std::cerr << "Error receiving data from client: " << WSAGetLastError() << std::endl;
    }
}


void handleClient(SOCKET clientSocket) {
    setlocale(LC_ALL, "Russian");
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; 

        
        std::cout << "Request received: \n" << buffer << std::endl;

        std::string request(buffer);
        if (request.find("GET") != std::string::npos) {
            handleGetRequest(clientSocket);
        } else if (request.find("POST") != std::string::npos) {
            handlePostRequest(clientSocket, request);
        }
    }
    else {
        std::cerr << "Error receiving data from client: " << WSAGetLastError() << std::endl;
    }

    closesocket(clientSocket);
}


int main() {
    setlocale(LC_ALL, "Russian");
    if (!initWinsock()) {
        return 1;
    }

    SOCKET serverSocket = createServerSocket();

    // Привязываем сокет к порту 8080 и начинаем слухать
    bindAndListen(serverSocket, 8080);

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
            continue;
        }

        // обрабатываем клиента
        handleClient(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}