
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "/home/pavel/pasha_db_server/Database.h"
#include "/home/pavel/pasha_db_server/Query.h"
#include "/home/pavel/pasha_db_server/Record.h"
#include <chrono>
#include <pthread.h>
#include <functional>
#include <mutex>
#include <sstream>
// Определимся с номером порта и другими константами.
#define PORT    5555
#define BUFLEN  1024

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
Database db("/home/pavel/pasha_db_server/db.txt");


void urldecode2(char *dst, const char *src)
{
    char a, b;
    while (*src) {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a')
                a -= 'a'-'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a'-'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst++ = 16*a+b;
            src+=3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}

void *threadFunction(void *arg) {
    int new_sock = *(int *) arg;
    std::cout << "Подключение клиента и создание потока. sock : " << new_sock << ". Дата: ";
    std::time_t t;
    t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::ctime(&t);

    int err;
    char buf[BUFLEN];
    err = recv(new_sock, buf, sizeof(buf), 0);
    buf[err] = '\0';
    char *output = static_cast<char *>(malloc(strlen(buf) + 1));
    urldecode2(output, buf); //функция для декода строки из браузера
    //printf("Decoded string: %s\n", output);

    if (err == 0) {

        std::cout << "закрытие потока и отключение клиента. sock : " << new_sock << std::endl;
        close(new_sock);

        pthread_exit(nullptr);
    }

    std::string query = output;
    free(output);
    int pos = query.find("recv=");
    if (pos != std::string::npos) {
        int pos1 = query.find(" HTTP", pos + 1);
        unsigned int value_index = pos + 5;
        if (pos1 != std::string::npos) {
            query = query.substr(value_index, pos1 - value_index);
        } else {
            query = query.substr(value_index);
        }
    }
    std::cout << "query:" << query << std::endl;

    std::stringstream response_body;
    std::stringstream response;

    if ((query=="=EXIT") or (query=="EXIT"))
    {
        pthread_mutex_lock(&lock);

//            db.add_rec(q);
        response_body
                << R"(<body><div align="center">
                                <form action = />
                                <input type="text" name="recv"></br>
                                <input type="submit" value="Отправить запрос">
                                </form>)";
        response_body << R"(<table cellspacing="5" cellpadding="5", ><tr>)"
                      << "<pre>" << "Завершение работы с клиентом" << "</pre>";;
        // Формируем весь ответ вместе с заголовками
        response << "HTTP/1.1 200 OK\r\n"
                 << "Version: HTTP/1.1\r\n"
                 << "Content-Type: text/html; charset=utf-8\r\n"
                 << "Content-Length: " << response_body.str().length()
                 << "\r\n\r\n"
                 << response_body.str();

        send(new_sock, response.str().c_str(), strlen(response.str().c_str()), 0);
        close(new_sock);
        pthread_mutex_unlock(&lock);
    }

    Query q(query);
    if (q.style == "add") {

        pthread_mutex_lock(&lock);

//            db.add_rec(q);
        response_body << "<pre>" << "Запись была успешно добавлена" << "</pre>\n";

        // Формируем весь ответ вместе с заголовками
        response << "HTTP/1.1 200 OK\r\n"
                 << "Version: HTTP/1.1\r\n"
                 << "Content-Type: text/html; charset=utf-8\r\n"
                 << "Content-Length: " << response_body.str().length()
                 << "\r\n\r\n"
                 << response_body.str();

        send(new_sock, response.str().c_str(), strlen(response.str().c_str()), 0);

        pthread_mutex_unlock(&lock);
    }
    if (q.style == "select") {
        pthread_mutex_lock(&lock);

        std::cout << "Выборка на основании запроса. sock : " << new_sock << std::endl;
        std::vector<Record> s = db.select_recs(q);

        pthread_mutex_unlock(&lock);
        t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string result = std::ctime(&t);
        for (auto &j : s) {
            result += j.print();
        }
        response_body
                << R"(<body><div align="center">
                                <form action = />
                                <input type="text" name="recv="></br>
                                <input type="submit" value="Отправить запрос">
                                </form>)";
        response_body << R"(<table cellspacing="5" cellpadding="5", ><tr>)";
        for (auto &j : s) {
            response_body << "<tr>";
            response_body << "<td>" << std::ctime(&t) << "</td>";
            response_body << "<td>" << j.fio.print() << "</td>";
            response_body << "<td>" << j.ip.print() << "</td>";
            response_body << "<td>" << j.date.print() << "</td>";
            response_body << "</tr>";
        }
        response_body << "</table>";
        response_body << "</div></body>";
        // Формируем весь ответ вместе с заголовками
        response << "HTTP/1.1 200 OK\r\n"
                 << "Version: HTTP/1.1\r\n"
                 << "Content-Type: text/html; charset=utf-8\r\n"
                 << "Content-Length: " << response_body.str().length()
                 << "\r\n\r\n"
                 << response_body.str();

        send(new_sock, response.str().c_str(), strlen(response.str().c_str()), 0);
        result = "";
    }
    if (q.style == "delete") {
        pthread_mutex_lock(&lock);
        db.delete_recs(q);
        response_body
                << R"(<body><div align="center">
                                <form action = />
                                <input type="text" name="recv"></br>
                                <input type="submit" value="Отправить запрос">
                                </form>)";
        response_body << R"(<table cellspacing="5" cellpadding="5", ><tr>)"
                      << "<pre>" << "Записи были успешно удалены" << "</pre>";;
        response << "HTTP/1.1 200 OK\r\n"
                 << "Version: HTTP/1.1\r\n"
                 << "Content-Type: text/html; charset=utf-8\r\n"
                 << "Content-Length: " << response_body.str().length()
                 << "\r\n\r\n"
                 << response_body.str();

        send(new_sock, response.str().c_str(), strlen(response.str().c_str()), 0);

        pthread_mutex_unlock(&lock);

    }

//    response_body << "<pre>" << query << "</pre>\n";
//
//    // Формируем весь ответ вместе с заголовками
//    response << "HTTP/1.1 200 OK\r\n"
//             << "Version: HTTP/1.1\r\n"
//             << "Content-Type: text/html; charset=utf-8\r\n"
//             << "Content-Length: " << response_body.str().length()
//             << "\r\n\r\n"
//             << response_body.str();
//
//    send(new_sock, response.str().c_str(), strlen(response.str().c_str()), 0);
    close(new_sock);
    pthread_exit(nullptr);
}


int main() {

    int err;
    int sock, new_sock;
    struct sockaddr_in addr;
    struct sockaddr_storage servStorage;
    pthread_t thr;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        perror("Server: cannot create socket");
        exit(EXIT_FAILURE);
    }


    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0) {
        perror("Server: cannot bind socket");
        exit(EXIT_FAILURE);
    }


    if (listen(sock, 5) == -1) {
        perror("ошибка вызова listen()");
        exit(1);
    }

    fprintf(stderr, "Сервер готов: %s\n",
            inet_ntoa(addr.sin_addr));

    // Основной бесконечный цикл проверки состояния сокетов
    socklen_t addr_len;

    int i = 0;
    while (true) {

        addr_len = sizeof(servStorage);

        if ((new_sock = accept(sock, (struct sockaddr *) &servStorage, &addr_len)) < 0) {
            perror("Ошибка вызова accept()");
            exit(1);
        }
        if (pthread_create(&thr, nullptr, threadFunction, &new_sock) != 0) {
            std::cout << "Не получается создать поток!" << std::endl;
        }

        pthread_join(thr, nullptr);

    }
    close(sock);
    return 0;
}

