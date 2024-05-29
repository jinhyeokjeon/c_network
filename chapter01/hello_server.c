#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // 유닉스 기반 파일 입출력. 리눅스에서는 소켓도 파일로 취급되어, 각 소켓은 파일 디스크립터를 부여받는다.
void error_handling(char *message);

int main(int argc, char *argv[]) {
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr; // IPv4 주소표현을 위한 구조체
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello World!";

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // PF_INET: IPv4 인터넷 프로토콜 체계. SOCK_STREAM: 연결지향형 데이터 전송 소켓. IPPROTO_TCP: 프로토콜 최종 선택. (앞 두 조건을 만족하는 프로토콜은 TCP만 존재함)
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;                // IPv4 인터넷 프로토콜에 적용하는 주소체계.
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 32비트 IP주소정보 저장. host-to-network: long data big-endian 방식으로 변환. (네트워크에서는 big-endian 통일). INADDR_ANY: IP주소 자동으로 할당.
    serv_addr.sin_port = htons(atoi(argv[1]));     // 16비트 포트번호 저장. host-to-network: short data big-endian 방식으로 변환.

    // bind함수는 sockaddr 구조체의 주소 정보를 원한다.
    // bind함수는 sockaddr의 값을 채우고 남은 부분은 0으로 채울 것을 요구한다.
    // 이러한 조건에 맞추어 IPv4 주소를 sockaddr의 값을 채우는 것은 어렵기 때문에,
    // sockaddr_in이 등장하였고, sockaddr에는 IPv4의 주소정보만 담기 위한것이 아니기 때문에 sin_family에 AF_INET 을 저장해야 한다.
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    // 연결요청 대기상태. (연결요청 대기상태에 두고자 하는 소켓의 파일 디스크립터, 연결요청 대기 큐의 크기)
    // 5가 전달되면 클라이언트의 연결요청을 5개까지 대기시킬 수 있다.
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // (서버 소켓의 파일 디스크립터, 연결요청한 클라이언트의 주소정보 담을 변수의 주솟값, 두번째 인자로 전달된 주소의  변수 크기. 함수호출이 완료되면 클라이언트의 주소정보길이가 계산되어 채워짐.)
    // accept 함수는 '연결요청 대기 큐'에서 대기중인 클라이언트의 연결 요청을 수락하는 기능.
    // accept 함수는 호출 성공 시 내부적으로 데이터 입출력에 사용할 소켓을 생성하고, 그 소켓의 파일 디스크립터 반환.
    // accept 함수는 대기 큐가 찰 때까지, 즉 클라이언트의 연결요청이 들어올 때까지 반환하지 않는다.
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");

    write(clnt_sock, message, sizeof(message)); // (전송 대상 파일 디스크립터, 전송할 데이터의 버퍼 주소값, 전송할 데이터의 바이트 수)
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}