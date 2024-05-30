#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void error_handling(char *message);

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected..............."); // stdout에 문자열을 출력하고, 출력 후에 자동으로 새 줄 문자 추가.

    fputs("Operand count: ", stdout); // 지정된 파일 스트림에 문자열을 출력하고, 출력 후에 자동으로 새 줄 문자 추가하지 않음.
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;

    for (i = 0; i < opnd_cnt; i++) {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int *)&opmsg[1 + i * OPSZ]);
    }
    fgetc(stdin); // 지정된 파일 스트림에서 문자 하나 읽어옴.
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[1 + opnd_cnt * OPSZ]);
    write(sock, opmsg, opnd_cnt * OPSZ + 2);
    read(sock, &result, RLT_SIZE); // 수신할 데이터의 크기가 4바이트이기 때문에 한번의 read 함수 호출으로 충분히 수신 가능.

    printf("Operation result: %d\n", result);
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}