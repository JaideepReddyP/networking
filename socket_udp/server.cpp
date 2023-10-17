#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main(int argc, char** argv) {
    const char* pkt = "Message to be sent";
    const char* srcIP = < source IP address >;
    const char* destIP = < destination IP address >;
    sockaddr_in dest;
    sockaddr_in local;
    WSAData data;
    WSAStartup(MAKEWORD(2, 2), &data);

    local.sin_family = AF_INET;
    inet_pton(AF_INET, srcIP, &local.sin_addr.s_addr);
    local.sin_port = htons(0);

    dest.sin_family = AF_INET;
    inet_pton(AF_INET, destIP, &dest.sin_addr.s_addr);
    dest.sin_port = htons(< destination port number >);

    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(s, (sockaddr *)&local, sizeof(local));

    sendto(s, pkt, strlen(pkt), 0, (sockaddr *)&dest, sizeof(dest));

    closesocket(s);
    WSACleanup();

    return 0;
}