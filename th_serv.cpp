
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "/home/pavel/pasha_db_server/Database.h"
#include "/home/pavel/pasha_db_server/gen.h"
#include "/home/pavel/pasha_db_server/Query.h"
#include "/home/pavel/pasha_db_server/Record.h"
#include <string>
#include <pthread.h>


// Определимся с номером порта и другими константами.
#define PORT    5555
#define BUFLEN  512
#define ERROR_CREATE_THREAD -1
#define ERROR_JOIN_THREAD -2
#define BAD_MESSAGE -3
#define SUCCESS 0
#define NUM_CONNECTIONS 5

// Две вспомогательные функции для чтения/записи (см. ниже)
int readFromClient(int fd, char *buf);

void writeToClient(int fd, const char *buf);

typedef struct Args_t
{
    int id;
    std::string query;
    int out;
};

int main() {

    int i, err, opt = 1;
    int sock, new_sock;
    fd_set active_set, read_set;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    char buf[BUFLEN];
    Database db("/home/pavel/pasha_db_server/db.txt");
    std::string query;
    socklen_t size;
    pthread_t threads[NUM_CONNECTIONS];
    int th_count;
    int status;
    int l;
    int status_addr;

    // Создаем TCP сокет для приема запросов на соединение
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        perror("Server: cannot create socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // Заполняем адресную структуру и
    // связываем сокет с любым адресом
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0) {
        perror("Server: cannot bind socket");
        exit(EXIT_FAILURE);
    }

    // Создаем очередь на 3 входящих запроса соединения
    err = listen(sock, NUM_CONNECTIONS);
    if (err < 0) {
        perror("Server: listen queue failure");
        exit(EXIT_FAILURE);
    }

    // Подготавливаем множества дескрипторов каналов ввода-вывода.
    // Для простоты не вычисляем максимальное значение дескриптора,
    // а далее будем проверять все дескрипторы вплоть до максимально
    // возможного значения FD_SETSIZE.
    FD_ZERO(&active_set);
    FD_SET(sock, &active_set);

    // Основной бесконечный цикл проверки состояния сокетов
    while (1) {
        // Проверим, не появились ли данные в каком-либо сокете.
        // В нашем варианте ждем до фактического появления данных.
        read_set = active_set;
        if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) {
            perror("Server: select  failure");
            exit(EXIT_FAILURE);
        }
        // Данные появились. Проверим в каком сокете.
        for (i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &read_set)) {
                if (i == sock) {
                    // пришел запрос на новое соединение
                    size = sizeof(client);
                    new_sock = accept(sock, (struct sockaddr *) &client, &size);
                    if (new_sock < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    fprintf(stdout, "Server: connect from host %s, port %hu.\n",
                            inet_ntoa(client.sin_addr),
                            ntohs(client.sin_port));
                    FD_SET(new_sock, &active_set);
                    status = pthread_create(&threads[th_count],NULL,funk,(void *), &args[i]);
                } else {
                    // пришли данные в уже существующем соединени
                    err = readFromClient(i, buf);
                    query = buf;
                    Query q(query);
                    std::vector<Record> s = db.select_recs(q);
                    std::cout << query << " found " << s.size() << "\n";
                    if (err < 0) {
                        // ошибка или конец данных
                        close(i);
                        FD_CLR(i, &active_set);
                    } else {
                        // данные прочитаны нормально
                        std::string result;
                        for (int j = 0; j < s.size(); ++j) {
                            result +=s[j].print();
                        }
                        if (!strcmp(buf, "STOP")) {
                            close(sock);
                            return 0;
                        }
                        writeToClient(i, result.c_str());
//                        close(i);
//                        FD_CLR (i, &active_set);
                        // а если это команда закончить работу?
                    }
                }
            }
        }
    }
}


int readFromClient(int fd, char *buf) {
    int nbytes;

    nbytes = read(fd, buf, BUFLEN);
    if (nbytes < 0) {
        // ошибка чтения
        perror("Server: read failure");
        return -1;
    } else if (nbytes == 0) {
        // больше нет данных
        return -1;
    } else {
        // есть данные
        fprintf(stdout, "Server got message: %s\n", buf);

        return 0;
    }
}


void writeToClient(int fd, const char *buf) {
    int nbytes;
//    unsigned char *s;

    //for (s = (unsigned char *) buf; *s; s++) *s = toupper(*s);
    nbytes = write(fd, buf, strlen(buf) + 1);
    fprintf(stdout, "Write back: %s\nnbytes=%d\n", buf, nbytes);

    if (nbytes < 0) {
        perror("Server: write failure");
    }
}

void* funk(void *args)
{
    // пришли данные в уже существующем соединени
    err = readFromClient(i, buf);
    query = buf;
    Query q(query);
    std::vector<Record> s = db.select_recs(q);
    std::cout << query << " found " << s.size() << "\n";
    if (err < 0) {
        // ошибка или конец данных
        close(i);
        FD_CLR(i, &active_set);
    } else {
        // данные прочитаны нормально
        std::string result;
        for (int j = 0; j < s.size(); ++j) {
            result +=s[j].print();
        }
        if (!strcmp(buf, "STOP")) {
            close(sock);
            return 0;
        }
        writeToClient(i, result.c_str());
//                        close(i);
//                        FD_CLR (i, &active_set);
        // а если это команда закончить работу?
    }