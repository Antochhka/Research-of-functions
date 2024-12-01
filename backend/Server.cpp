#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
#include "parser.h"      
#include "validator.h"     
#include "calculation.h"   
#include "analysis.h"
#include "derivatives.h"

const int PORT = 8080;

// Функция для конвертации массива координат в JSON
std::string coordinatesToJson(const std::vector<std::vector<double>>& coordinate_arr)
{
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < coordinate_arr.size(); ++i)
    {
        oss << "[" << coordinate_arr[i][0] << ", " << coordinate_arr[i][1] << "]";
        if (i < coordinate_arr.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

// Функция для конвертации интервалов в JSON
std::string intervalsToJson(const std::vector<std::pair<double, double>>& intervals)
{
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < intervals.size(); ++i)
    {
        oss << "[" << intervals[i].first << ", " << intervals[i].second << "]";
        if (i < intervals.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

// Функция для конвертации точек разрыва в JSON
std::string breakPointsToJson(const std::vector<std::pair<double, int>>& break_points)
{
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < break_points.size(); ++i)
    {
        oss << "[" << break_points[i].first << ", " << break_points[i].second << "]";
        if (i < break_points.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

// Функция для конвертации асимптот в JSON
std::string asymptotesToJson(const std::vector<double>& vertical_asymptote, const std::pair<double, double>& horizontal_asymptote)
{
    std::ostringstream oss;
    oss << "{\"vertical_asymptotes\": [";
    for (size_t i = 0; i < vertical_asymptote.size(); ++i)
    {
        oss << vertical_asymptote[i];
        if (i < vertical_asymptote.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "], \"horizontal_asymptote\": [" << horizontal_asymptote.first << ", " << horizontal_asymptote.second << "]}";
    return oss.str();
}

// Функция для конвертации кортежей в JSON
std::string tuplesToJson(const std::vector<std::tuple<double, double, int>>& tuples)
{
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < tuples.size(); ++i)
    {
        oss << "[" << std::get<0>(tuples[i]) << ", " << std::get<1>(tuples[i]) << ", " << std::get<2>(tuples[i]) << "]";
        if (i < tuples.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

std::string readFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Ошибка при открытии файла: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void handleClient(int clientSocket)
{
    setlocale(LC_ALL, "ru_RU.UTF-8");  // Устанавливаем локализацию для поддержки русского языка

    char buffer[1024] = { 0 };
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        std::cout << "Получен HTTP запрос:\n"
            << buffer << std::endl;
    }

    std::string request(buffer);
    std::string method;
    std::string path;
    std::string body;

    // Простой парсинг HTTP-запроса
    std::istringstream requestStream(request);
    requestStream >> method >> path;

    if (method == "GET")
    {
        if (path == "/")
        {
            // Читаем HTML файл
            std::string htmlContent = readFile("/home/suhare/Research-of-functions/frontend/index.html");
            if (htmlContent.empty())
            {
                std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nФайл не найден";
                send(clientSocket, response.c_str(), response.size(), 0);
            }
            else
            {
                std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
                    std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
                send(clientSocket, response.c_str(), response.size(), 0);
            }
        }
        else if (path == "/main.css")
        {
            std::string cssContent = readFile("/home/suhare/Research-of-functions/frontend/main.css");
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: " +
                std::to_string(cssContent.size()) + "\r\n\r\n" + cssContent;
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else if (path == "/index.js")
        {
            std::string jsContent = readFile("/home/suhare/Research-of-functions/frontend/index.js");
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\nContent-Length: " +
                std::to_string(jsContent.size()) + "\r\n\r\n" + jsContent;
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else
        {
            std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nРесурс не найден";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }
    else if (method == "POST")
    {
        // Получение тела запроса
        std::string contentLengthHeader = "Content-Length: ";
        size_t contentLengthPos = request.find(contentLengthHeader);
        if (contentLengthPos != std::string::npos)
        {
            size_t start = contentLengthPos + contentLengthHeader.length();
            size_t end = request.find("\r\n", start);
            int contentLength = std::stoi(request.substr(start, end - start));
            body = request.substr(request.find("\r\n\r\n") + 4, contentLength);
        }

        // Проверяем, что Content-Type: application/json
        std::string contentTypeHeader = "Content-Type: ";
        size_t contentTypePos = request.find(contentTypeHeader);
        if (contentTypePos != std::string::npos)
        {
            size_t start = contentTypePos + contentTypeHeader.length();
            size_t end = request.find("\r\n", start);
            std::string contentType = request.substr(start, end - start);

            if (contentType == "application/json")
            {
                // Обрабатываем JSON тело
                std::string function;
                size_t functionPos = body.find("\"function\":\"");
                if (functionPos != std::string::npos)
                {
                    size_t start = functionPos + std::string("\"function\":\"").length();
                    size_t end = body.find("\"", start);
                    function = body.substr(start, end - start);
                }

                std::cout << "Функция: " << function << std::endl;

                if (validateMathExpression(function)) {
                    // Инициализация переменных
                    std::vector<std::vector<double>> output_arr(BUFFER, std::vector<double>(2));
                    std::vector<bool> nan_arr;
                    std::vector<std::pair<double, double>> intervals;
                    std::vector<std::pair<double, int>> break_points;
                    std::vector<std::vector<double>> coordinate_arr;
                    std::vector<double> vertical_asymptote;
                    std::pair<double, double> horizontal_asymptote = {10000000000 , 10000000000};
                    unsigned int i_out_arr = 0;
                    std::vector<std::vector<double>> first_critical_points;
                    std::vector<std::vector<double>> second_critical_points;
                    RCP<const Basic> first_derivative_expression;
                    RCP<const Basic> second_derivative_expression;
                    std::vector<std::tuple<double, double, int>> monotony_intervals;
                    std::vector<std::tuple<double, double, int>> convexity_concavity_intervals;
                    std::string break_point_for_tan;
                    std::vector<double> k_b_index = {0, 0, 0};

                    // Парсинг выражения
                    parser(output_arr, i_out_arr, function);

                    // Инициализация массива NaN значений
                    int size = static_cast<int>(RIGHT_BORDER * 2 / SHIFT + 1);
                    nan_arr.resize(size);

                    // Проверка диапазонов и создание интервалов
                    check_nan_by_range(output_arr, i_out_arr, nan_arr, function);
                    create_intervals(nan_arr, intervals);

                    // Вычисление координат
                    calculating_coordinate(output_arr, i_out_arr, coordinate_arr, intervals);

                    // Поиск точек разрыва
                    search_break_points(intervals, break_points, output_arr, i_out_arr);

                    // Поиск вертикальных асимптот
                    search_vertical_asymptote(intervals, output_arr, i_out_arr, vertical_asymptote);

                    // Поиск горизонтальных асимптот
                    search_horizontal_asymptote(intervals, output_arr, i_out_arr, k_b_index);
                    if (k_b_index[2] == 1) {
                        horizontal_asymptote = std::make_pair(k_b_index[0], k_b_index[1]);
                    }

                    // Проверка симметрии функции
                    std::string symmetryResult = checkFunctionSymmetry(output_arr, i_out_arr);

                    // Проверка и поиск периода функции
                    bool is_periodic = isFunctionPeriodic(output_arr, i_out_arr);
                    std::string periodResult = is_periodic ? findFunctionPeriod(output_arr, i_out_arr) : "Функция не является периодической на заданном интервале";

                    // Поиск критических точек первой и второй производных
                    search_critical_points(first_critical_points, second_critical_points, first_derivative_expression,
                                           second_derivative_expression, function);

                    // Области возрастания и убывания
                    search_monotony_intervals(intervals, first_critical_points, monotony_intervals,
                                              first_derivative_expression);

                    // Области выпуклости и вогнутости
                    search_convexity_concavity_intervals(intervals, second_critical_points, convexity_concavity_intervals,
                                                         second_derivative_expression);

                    // Преобразование данных в JSON
                    std::string jsonCoordinates = coordinatesToJson(coordinate_arr);
                    std::string jsonDomain = intervalsToJson(intervals);
                    std::string jsonBreakPoints = breakPointsToJson(break_points);
                    std::string jsonAsymptotes = asymptotesToJson(vertical_asymptote, horizontal_asymptote);
                    std::string jsonMonotonyIntervals = tuplesToJson(monotony_intervals);
                    std::string jsonConvexityConcavityIntervals = tuplesToJson(convexity_concavity_intervals);
                    std::string jsonResponse = "{\"coordinates\": " + jsonCoordinates +
                        ", \"domain\": " + jsonDomain +
                        ", \"breakPoints\": " + jsonBreakPoints +
                        ", \"asymptotes\": " + jsonAsymptotes +
                        ", \"symmetry\": \"" + symmetryResult + "\"" +
                        ", \"periodicity\": \"" + periodResult + "\"" +
                        ", \"monotonyIntervals\": " + jsonMonotonyIntervals +
                        ", \"convexityConcavityIntervals\": " + jsonConvexityConcavityIntervals +
                        "}";

                    // Формирование HTTP-ответа
                    int contentLength = jsonResponse.size();
                    std::ostringstream responseStream;
                    responseStream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Type: application/json\r\n"
                        << "Content-Length: " << contentLength << "\r\n\r\n"
                        << jsonResponse;

                    // Отправка ответа клиенту
                    std::string response = responseStream.str();
                    send(clientSocket, response.c_str(), response.size(), 0);
                }

                else
                {
                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nНекорректная функция";
                    send(clientSocket, response.c_str(), response.size(), 0);
                }
            }
        }
    }
    else
    {
        std::string response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\n\r\nМетод не поддерживается";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(clientSocket);
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Bind failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == -1)
    {
        std::cerr << "Listen failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Сервер запущен на порту " << PORT << std::endl;

    while (true)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket != -1)
        {
            handleClient(clientSocket);
        }
        else
        {
            std::cerr << "Accept failed" << std::endl;
        }
    }

    close(serverSocket);
    return 0;
}