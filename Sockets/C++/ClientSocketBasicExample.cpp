#include <iostream>
#include <cstring> // For strlen and memset
#include <unistd.h> // For close
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h> // For inet_addr

using namespace std;

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    const char *message = "Hello, Server!";
    char buffer[1024];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cout << "Socket creation failed" << endl;
        return -1;
    }

    // Server address setup
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); // Example port
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Example IP

    // Server connection
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection failed" << endl;
        close(sock);
        return -1;
    }

    // Message -> Server
    send(sock, message, strlen(message), 0);
    cout << "Message sent." << endl;

    // Clear buffer before reading
    memset(buffer, 0, sizeof(buffer));

    // Read response from server
    int bytesRead = 0;
    while (true) { 
        bytesRead = read(sock, buffer, sizeof(buffer) - 1); // Read into buffer

        if (bytesRead < 0) {
            break; // Exit loop if an error occurs
        }

        // Only print if buffer is not empty
        if (bytesRead > 0) {
            cout << "Received: " << buffer << endl;
	    break;
        }
    }

    // Socket close
    close(sock);
    return 0;
}
