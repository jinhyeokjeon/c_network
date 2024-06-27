#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void read_childproc(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Removed proc id: %d\n", id);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;
    struct sigaction act;
    act.__sigaction_u.__sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if (pid == 0) {
        puts("Hi! I'm a child process");
        sleep(10);
        return 12; // 메인함수에서의 return은 exit와 동일하게 동작한다.
    } else {
        printf("Child proc id: %d\n", pid);
        pid = fork();
        if (pid == 0) {
            puts("Hi! I'm a child process");
            sleep(10);
            exit(24); // 메인 함수 외부에서 프로그램을 종료할 때 필요. 즉 메인함수에서 다른 함수 호출 시, 해당 함수가 메인 프로세스 종료시키려면 exit 해야함.
        } else {
            int i;
            printf("Child proc id: %d\n", pid);
            for (i = 0; i < 5; i++) {
                puts("wait...");
                sleep(5);
            }
        }
    }

    return 0;
}