#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void error_handling(char *message);

int main(int argc, char *argv[]) {
    int i;
    struct hostent *host;
    struct in_addr addr;
    if (argc != 2) {
        printf("Usage: %s <addr>\n", argv[0]);
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.s_addr = inet_addr(argv[1]);
    // (in_addr 구조체 변수 포인터, 첫 번째 인자로 전달된 주소정보의 길이(IPv4: 4), 주소체계 정보 전달(IPv4: AF_INET))
    host = gethostbyaddr((char *)&addr.s_addr, 4, AF_INET);
    if (!host)
        error_handling("gethost... error");

    printf("Official name: %s\n", host->h_name);
    for (i = 0; host->h_aliases[i]; i++)
        printf("Aliases %d: %s\n", i + 1, host->h_aliases[i]);
    printf("Address type: %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_ITNER6");
    for (i = 0; host->h_addr_list[i]; i++)
        printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}