#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
    key_t key = ftok("semfile", 1);
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    semctl(semid, 0, SETVAL, 3);
    int value = semctl(semid, 0, GETVAL);
    printf("Semaphore value: %d\n", value);

    return 0;
}