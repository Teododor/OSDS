/*

#include <stdio.h>

int main(int argc, char *argv[])
{
    while(1)
    {
        printf("Hello World");
    }
    
    return 0;
}

*/


#include <stdio.h>
#include <sys/mman.h> // for mmap, protect -> man mmap in terminal for accessing manual page
#include <signal.h> // for sigaction -> SIGSEGV
#include <unistd.h> // for _exit 
#include <stdlib.h> // for error codes
#include <bits/mman-linux.h>

void handler(int sig, siginfo_t *info, void *ucontext)
{
    printf("Illegal accessing for memory at address: %p\n", info->si_addr);
    mprotect(info->si_addr, 4096, PROT_WRITE);
}

int main(int argc, char *argv[])
{
    int *ptr = mmap(NULL, 4096, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    
    struct sigaction s;

    s.sa_flags = SA_SIGINFO;
    s.sa_sigaction = handler;

    if(sigaction(SIGSEGV, &s, NULL) == -1)
    {
        perror("Sigaction Error");
        _exit(EXIT_FAILURE);
    }

    // now we try to write (READ_ONLY)
    ptr[0] = 5;
    printf("Now it works: %d\n", ptr[0]);
    
    return 0;
}