#include <iostream>
#include <winsock2.h>
#include <string>
#include <regex>
#include <sstream>
#include <fstream> // Необходим для работы с файлами
#include "parser.h"

#pragma comment(lib, "ws2_32.lib")

const int PORT = 8080;

bool isValidFunction(const std::string& function) {
    return function.find("x") != std::string::npos;  // Проверка на наличие переменной x
}

// Функция для конвертации массива координат в JSON
std::string coordinatesToJson(const std::vector<std::vector<double>>& coordinate_arr) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < coordinate_arr.size(); ++i) {
        oss << "[" << coordinate_arr[i][0] << ", " << coordinate_arr[i][1] << "]";
        if (i < coordinate_arr.size() - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}


std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка при открытии файла: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void handleClient(SOCKET clientSocket) {
    setlocale(LC_ALL, "Russian");
    char buffer[1024] = { 0 };
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Получен HTTP запрос:\n" << buffer << std::endl;
    }

    std::string request(buffer);
    std::string method;
    std::string path;
    std::string body;


    // Простой парсинг HTTP-запроса
    std::istringstream requestStream(request);
    requestStream >> method >> path;

    if (method == "GET") {
        if (path == "/") {
            // Читаем HTML файл
            std::string htmlContent = readFile("D:\\learning\\project\\fullstack\\frontend\\index.html");
            if (htmlContent.empty()) {
                std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nФайл не найден";
                send(clientSocket, response.c_str(), response.size(), 0);
            }
            else {
                std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
                    std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
                send(clientSocket, response.c_str(), response.size(), 0);
            }
        }
        else if (path == "/main.css") {
            std::string cssContent = readFile("D:\\learning\\project\\fullstack\\frontend\\main.css");
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: " +
                std::to_string(cssContent.size()) + "\r\n\r\n" + cssContent;
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else if (path == "/index.js") {
            std::string jsContent = readFile("D:\\learning\\project\\fullstack\\frontend\\index.js");
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\nContent-Length: " +
                std::to_string(jsContent.size()) + "\r\n\r\n" + jsContent;
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else {
            std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nРесурс не найден";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }
    else if (method == "POST") {
    // Получение тела запроса
    std::string contentLengthHeader = "Content-Length: ";
    size_t contentLengthPos = request.find(contentLengthHeader);
    if (contentLengthPos != std::string::npos) {
        size_t start = contentLengthPos + contentLengthHeader.length();
        size_t end = request.find("\r\n", start);
        int contentLength = std::stoi(request.substr(start, end - start));
        body = request.substr(request.find("\r\n\r\n") + 4, contentLength);
    }

    // Проверяем, что Content-Type: application/json
    std::string contentTypeHeader = "Content-Type: ";
    size_t contentTypePos = request.find(contentTypeHeader);
    if (contentTypePos != std::string::npos) {
        size_t start = contentTypePos + contentTypeHeader.length();
        size_t end = request.find("\r\n", start);
        std::string contentType = request.substr(start, end - start);

        if (contentType == "application/json") {
            // Обрабатываем JSON тело
            std::string function;
            size_t functionPos = body.find("\"function\":\"");
            if (functionPos != std::string::npos) {
                size_t start = functionPos + std::string("\"function\":\"").length();
                size_t end = body.find("\"", start);
                function = body.substr(start, end - start);
            }

            std::cout << "Функция: " << function << std::endl;

             // Проверяем валидность математического выражения
                if (validateMathExpression(function)) {
                    // Подготовка выходного массива
                    std::vector<std::vector<double>> output_arr;
                    unsigned int i_out_arr = 0;
                    output_arr.resize(BUFFER);
                    for (int i = 0; i < BUFFER; i++) {
                        output_arr[i].resize(2);
                    }

                    // Парсим выражение
                    parser(output_arr, i_out_arr, function);

                    // Расчёт координат
                    std::vector<std::vector<double>> coordinate_arr;
                    int size = RIGHT_BORDER * 2 * 1 / SHIFT + 1;
                    coordinate_arr.resize(size);
                    for (int i = 0; i < size; i++) {
                        coordinate_arr[i].resize(2);
                    }
                    calculating_coordinate(output_arr, i_out_arr, coordinate_arr);

                    // Преобразуем массив координат в JSON
                    std::string jsonResponse = coordinatesToJson(coordinate_arr);

                    // Отправляем ответ
                    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " +
                                           std::to_string(jsonResponse.size()) + "\r\n\r\n" + jsonResponse;
                    send(clientSocket, response.c_str(), response.size(), 0);
                }
            else {
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nНекорректная функция";
                send(clientSocket, response.c_str(), response.size(), 0);
                
            }
        }
    }
}
    else {
        std::string response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\n\r\nМетод не поддерживается";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Сервер запущен на порту " << PORT << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            handleClient(clientSocket);
        }
        else {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}