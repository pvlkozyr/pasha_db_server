
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
#define BUFLEN  512

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
Database db("/home/pavel/pasha_db_server/db.txt");

void* threadFunction(void* arg) {
    int new_sock = *(int *) arg;
    std::cout << "Подключение клиента и создание потока. sock : " <<new_sock << ". Дата: ";
    std::time_t t;
    t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::ctime(&t);
    int err;
    char buf[BUFLEN];
    while (true) {
        pthread_mutex_lock(&lock);

        err = recv(new_sock, buf, sizeof(buf), 0);

        pthread_mutex_unlock(&lock);
        if (err == 0)
        {
            pthread_mutex_lock(&lock);
            std::cout << "закрытие потока и отключение клиента. sock : " <<new_sock << std::endl;
            close(new_sock);

            pthread_mutex_unlock(&lock);
            pthread_exit(nullptr);
        }
        std::string query = buf;

        if (query == "EXIT") {
            pthread_mutex_lock(&lock);
            std::cout << "закрытие потока и отключение клиента командой EXIT. sock : " <<new_sock << ". Дата: ";
            t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << std::ctime(&t);
            close(new_sock);
            pthread_mutex_unlock(&lock);
            pthread_exit(nullptr);
        }
        Query q(query);

        if (q.style == "add") {

            pthread_mutex_lock(&lock);

//            db.add_rec(q);
            send(new_sock, "Запись была успешно добавлена", 512, 0);

            pthread_mutex_unlock(&lock);
        }
        if (q.style == "select") {
            pthread_mutex_lock(&lock);

            std::cout << "Выборка на основании запроса. sock : "<< new_sock << std::endl;
            std::vector<Record> s = db.select_recs(q);

            pthread_mutex_unlock(&lock);
            t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::string result = std::ctime(&t);
            for (auto &j : s) {
                result += j.print();
            }
            send(new_sock, result.c_str(), 512, 0);
            result = "";
        }
        if (q.style == "delete") {
            pthread_mutex_lock(&lock);
            db.delete_recs(q);
            send(new_sock, "Записи были успешно удалены", 512, 0);

            pthread_mutex_unlock(&lock);

        }
    }
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
        if (pthread_create(&thr,nullptr,threadFunction,&new_sock) != 0)
        {
            std::cout << "Не получается создать поток!" << std::endl;
        }

    }
}
