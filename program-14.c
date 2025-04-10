#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds buf;

    key = ftok("progfile", 65);  // Ensure "progfile" exists in current directory
    msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl - IPC_STAT");
        return 1;
    }

    printf("Message Queue Properties:\n");
    printf("----------------------------\n");
    printf("Queue ID: %d\n", msgid);
    printf("Owner UID: %d\n", buf.msg_perm.uid);
    printf("Owner GID: %d\n", buf.msg_perm.gid);
    printf("Permissions: %o\n", buf.msg_perm.mode);
    printf("Current number of bytes in queue: %lu\n", buf.__msg_cbytes);
    printf("Number of messages in queue: %lu\n", buf.msg_qnum);
    printf("Max bytes allowed in queue: %lu\n", buf.msg_qbytes);
    printf("Last message sent time: %s", ctime(&buf.msg_stime));
    printf("Last message received time: %s", ctime(&buf.msg_rtime));
    printf("Last change time: %s", ctime(&buf.msg_ctime));

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl - IPC_RMID");
        return 1;
    }

    printf("\nMessage queue with ID %d deleted successfully.\n", msgid);
    return 0;
}
