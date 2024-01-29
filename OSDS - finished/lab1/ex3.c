#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif


void handler(int signal, siginfo_t *info, void *ucontext) {
    printf("Illegal trial of memory access at the address: %p\n", info->si_addr);
    mprotect(info->si_addr, 4096, PROT_READ | PROT_WRITE);
}

int main() {
    int *ptr = mmap(NULL, 4096, PROT_READ , MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap Error");
        return EXIT_FAILURE;
    }

    struct sigaction s;
    s.sa_flags = SA_SIGINFO;
    s.sa_sigaction = handler;

    if (sigaction(SIGSEGV, &s, NULL) == -1) {
        perror("Sigaction Error");
        return EXIT_FAILURE;
    }

    ptr[0] = 5;
    printf("Now the program works: %d\n", ptr[0]);

    return 0;
}