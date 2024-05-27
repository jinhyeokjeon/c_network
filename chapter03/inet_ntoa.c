#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(INADDR_ANY);

    str_ptr = inet_ntoa(addr1.sin_addr); // network-to-address: 네트워크 바이트 순서 -> 호스트 바이트 순서로 address 정렬. 함수 내에서 메모리 공간 할당하고 위치 반환. 호출 될 때마다 메모리 초기화.
    strcpy(str_arr, str_ptr);
    printf("Dotted-Decimal notation1: %s\n", str_ptr);

    inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notation2: %s\n", str_ptr);
    printf("Dotted-Decimal notation3: %s\n", str_arr);
    return 0;
}