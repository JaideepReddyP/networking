#include<stdio.h>
#include<signal.h>
#include<windows.h>
#include<unistd.h>

void handler(int s) {
    signal(SIGINT, handler);
    printf("handled %d\n", getpid());
}

int main() {
    signal(SIGINT, handler);

    while(1) {
        printf("hello \n");
        Sleep(1000);
    }
    return 0;
}