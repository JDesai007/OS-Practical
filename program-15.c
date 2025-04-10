#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>

int main() {
    key_t key;
    int msgid[10];
    char command[50];
    struct msqid_ds buf;

    printf("Creating 10 message queues...\n");

    for (int i = 0; i < 10; i++) {
        key = ftok(".", i + 1);
        msgid[i] = msgget(key, IPC_CREAT | 0666);
        if (msgid[i] == -1) {
            perror("msgget failed");
            return 1;
        }
        printf("Queue %d created with ID: %d\n", i + 1, msgid[i]);
    }

    printf("\nDeleting message queues using system(ipcrm)...\n");
    for (int i = 0; i < 10; i++) {
        sprintf(command, "ipcrm -q %d", msgid[i]);
        system(command);
        printf("Queue with ID %d deleted.\n", msgid[i]);
    }

    printf("\nChecking status of each message queue after deletion:\n");
    for (int i = 0; i < 10; i++) {
        if (msgctl(msgid[i], IPC_STAT, &buf) == -1) {
            printf("Queue ID %d: Does not exist (deleted successfully).\n", msgid[i]);
        } else {
            printf("Queue ID %d: Still exists (deletion failed).\n", msgid[i]);
        }
    }

    return 0;
}
