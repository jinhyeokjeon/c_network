#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;

    struct sockaddr_in serv_adr, from_adr;
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    while (1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 아래 방식은 unconnected UDP 소켓으로 매번 UDP 소켓에 목적지의 IP와 PORT번호 등록, 데이터 전송, 목적지 정보 삭제의 과정을 거친다.
        // sendto 함수 호출 시 소켓(sock)에 내 주소정보가 할당되지 않았다면, 해당 소켓에 내 IP와 PORT번호가 자동으로 할당되고 유지된다.
        // 소켓: (내 IP, PORT / 목적지 IP, PORT)
        sendto(sock, message, strlen(message) + 1, 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
        adr_sz = sizeof(from_adr);
        // UDP 소켓은 데이터의 경계가 존재한다. 즉 한번의 sendto 함수를 통해 보낸 데이터는 한번의 recvfrom 함수를 통해 수신한다.
        // TCP 소켓은 그러한 경계가 존재하지 않는다.
        // 예를 들어 메시지 3개가 이미 보낸진 상황에서 수신을 시작하면, TCP 소켓은 한번의 입력함수 호출로 데이터를 전부 수신할 수도 있지만, UDP 소켓은 정확히 3번의 입력함수 호출이 요구된다.
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&from_adr, &adr_sz);
        printf("Message from server: %s", message);
    }
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}