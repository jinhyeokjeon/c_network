#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void error_handling(char *message);

int main(int argc, char *argv[]) {
    int i;
    struct hostent *host;
    if (argc != 2) {
        printf("Usage: %s <addr>\n", argv[0]);
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if (!host)
        error_handling("gethost... error");

    printf("Official name: %s\n", host->h_name);
    for (i = 0; host->h_aliases[i]; i++)
        printf("Aliases %d: %s\n", i + 1, host->h_aliases[i]);
    printf("Address type: %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_ITNER6");

    // host->h_addr_list[i]가 가리키는 값은 struct in_addr* 이다. 구조체 hostent는 IPv4만을 위해 정의된 구조체가 아니기 때문에 char형 포인터 배열로 선언한 것이다.
    // 소켓관련 함수들은 void형 포인터 변수가 표준화되기 이전에 정의되어 있다. 당시에는 참조의 대상이 일정하지 않은 경우 char형 포인터 변수를 활용하였다.
    for (i = 0; host->h_addr_list[i]; i++)
        printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}