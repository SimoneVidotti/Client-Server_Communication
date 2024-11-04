#include <iostream>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 8888
#define BUFFER_SIZE 2048

void Line() {
    cout << endl;
    for (int C = 0; C < 60; C++) {
        cout << "=";
    }
    cout << endl;
}

void handle_connection(int socket_fd) {
    char buffer[BUFFER_SIZE] = {0};
    while (true) {
        int valread = read(socket_fd, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            cout << "Connessione chiusa.\n";
            break;
        }

        // Stampa il messaggio ricevuto in modo sicuro
        cout << "(ip server): " << buffer;
        Line();

        memset(buffer, 0, BUFFER_SIZE); // Pulisci il buffer
    }
    close(socket_fd);
}

int Menu() {
    int c;
    cout << endl << "TerminalMes" << endl;
    cout << "Scegli un'opzione:\n1. Avvia come server\n2. Avvia come client\n";
    cout << endl << "[1,2]> ";
    cin >> c;
    cin.ignore(); // Ignora il newline dopo la scelta
    Line();
    return c;
}

int main() {
    int choice = Menu();

    if (choice == 1) {
        // Parte Server
        int server_fd, new_socket;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        thread t(handle_connection, new_socket);
        t.detach();

        while (true) {
            string message;
            getline(cin, message);

            if (message == "exit") {
                cout << "Chiusura del server...\n";
                break;
            }

            send(new_socket, message.c_str(), message.size(), 0);
            Line();
        }

        close(new_socket);
        close(server_fd);
    } else if (choice == 2) {
        // Parte Client
        int sock = 0;
        struct sockaddr_in serv_addr;
        char buffer[BUFFER_SIZE] = {0};

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "Socket creation error\n";
            return -1;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        const char* IPv4 = "127.0.0.1";

        if (inet_pton(AF_INET, IPv4, &serv_addr.sin_addr) <= 0) {
            cout << "Indirizzo non valido o non supportato\n";
            return -1;
        }

                if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            cout << "Connessione fallita\n";
            return -1;
        }

        thread t([sock]() {
            char buffer[BUFFER_SIZE] = {0};
            while (true) {
                int valread = read(sock, buffer, BUFFER_SIZE);
                if (valread <= 0) {
                    cout << "Connessione chiusa dal server.\n";
                    break;
                }

                // Stampa il messaggio ricevuto in modo sicuro
                cout << "(ip server): " << buffer;
                Line();

                memset(buffer, 0, BUFFER_SIZE); // Pulisci il buffer
            }
            close(sock);
        });
        t.detach();

        while (true) {
            string message;
            getline(cin, message);

            if (message == "exit") {
                cout << "Chiusura del client...\n";
                break;
            }

            send(sock, message.c_str(), message.size(), 0);
            Line();
        }

        close(sock);
    } else {
        cout << "Opzione non valida.\n";
    }

    return 0;
}