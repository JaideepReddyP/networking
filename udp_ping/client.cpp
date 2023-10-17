//
// C:\Users\pjaid>ping google.com
//
// Pinging google.com [142.250.193.110] with 32 bytes of data:
// Reply from 142.250.193.110: bytes=32 time=19ms TTL=58
// Reply from 142.250.193.110: bytes=32 time=18ms TTL=58
// Reply from 142.250.193.110: bytes=32 time=18ms TTL=58
// Reply from 142.250.193.110: bytes=32 time=18ms TTL=58
//
// Ping statistics for 142.250.193.110:
//     Packets: Sent = 4, Received = 4, Lost = 0 (0% loss),
// Approximate round trip times in milli-seconds:
//     Minimum = 18ms, Maximum = 19ms, Average = 18ms
//
#include <iostream>
#include <winsock2.h>
#include <chrono>
using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

#define SERVER "127.0.0.1"  // or "localhost" - ip address of UDP server
#define BUFLEN 512  // max length of answer
#define PORT 8888  // the port on which to listen for incoming data

int main(int argc, char** argv)
{
    // system("title UDP Client");

    // initialise winsock
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        printf("Failed to initialize winsock. Error Code: %d", WSAGetLastError());
        return 1;
    }

    // create socket
    sockaddr_in server;
    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
    {
        printf("socket() failed with error code: %d", WSAGetLastError());
        return 2;
    }

    // setup address structure
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER);

    // start communication
    char message[] = "some test msg to send for ping!";
    cout << "Pinging " << SERVER << " with " << sizeof(message) << " bytes of data:\n";
    int packet_count = 0;
    while (packet_count++ < 4)
    {
        // start counter
        auto start = std::chrono::high_resolution_clock::now();
        // send the message
        if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code: %d", WSAGetLastError());
            return 3;
        }

        // receive a reply and print it
        // clear the answer by filling null, it might have previously received data
        char answer[BUFLEN] = {};

        // try to receive some data, this is a blocking call
        int slen = sizeof(sockaddr_in);
        int answer_length;
        if (answer_length = recvfrom(client_socket, answer, BUFLEN, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code: %d", WSAGetLastError());
            exit(0);
        }
        // end timer
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        cout << "Reply from " << SERVER << ": bytes=" << sizeof(message) << " time=" << (float)duration.count()/1000 << "ms\n";
        Sleep(1000);
    }

    closesocket(client_socket);
    WSACleanup();
}