/**
 * @author Dylan Duhamel {@literal duhadm19@alumni.wfu.edu}
 * @date Feb. 1, 2024
 * Filestem deamon to notify modifications to given filemonitor
 **/

#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/inotify.h>

#include <libnotify/notify.h>

#define EXT_SUCCESS 0
#define EXT_FAILURE_TOO_FEW_ARGS 1
#define EXT_ERR_INIT_INOTIFY 2
#define EXT_ERR_ADD_WATCH 3
#define EXT_ERR_BASE_PATH_NULL 4
#define EXT_ERR_READ_INOTIFY 5

/* The fd for incoming events */
int IeventQueue = -1;
int IeventStatus = -1;

int main(int argc, char** argv)
{
    char* basePath = NULL;
    char* token = NULL;
    char* notificatioMessage = NULL;

    char buffer[4096];
    int readLength;

    const struct inotify_event* watchEvent;
    /* Bitwise or mask. */
    const uint32_t watchMask = IN_CREATE | IN_DELETE | IN_ACCESS | IN_CLOSE_WRITE | IN_MODIFY | IN_MOVE_SELF;

    if (argc < 2) {
        fprintf(stderr, "USAGE: filemonitor PATH\n");
        exit(EXT_FAILURE_TOO_FEW_ARGS);
    }

    /* Allocate memory for path str. */
    basePath = (char*)malloc(sizeof(char) * (strlen(argv[1]) + 1));
    strcpy(basePath, argv[1]);

    token = strtok(basePath, "/");

    while (token != NULL) {
        basePath = token;
        token = strtok(NULL, "/");
    }

    if (basePath == NULL) {
        fprintf(stderr, "Error getting base path.\n");
        exit(EXT_ERR_BASE_PATH_NULL);
    }

    IeventQueue = inotify_init();
    if (IeventQueue == -1) {
        fprintf(stderr, "Error initializing inotify instance.\n");
        exit(EXT_ERR_INIT_INOTIFY);
    }

    IeventStatus = inotify_add_watch(IeventQueue, argv[1], watchMask);
    if (IeventStatus == -1) {
        fprintf(stderr, "Error adding file to watch instance.\n");
    }

    while (true) {
        printf("Waiting for ievent...\n");

        readLength = read(IeventQueue, buffer, sizeof(buffer));
        if (readLength == -1) {
            fprintf(stderr, "Error reading from inotify instance.\n");
            exit(EXT_ERR_READ_INOTIFY);
        }

        for (char* bufferPointer = buffer; bufferPointer < buffer + readLength; bufferPointer += sizeof(struct inotify_event) + watchEvent->len) {
            notificatioMessage = NULL;
            watchEvent = (const struct inotify_event*)bufferPointer;

            if (watchEvent->mask & IN_CREATE) {
                notificatioMessage = "File created.\n";
            }
            if (watchEvent->mask & IN_DELETE) {
                notificatioMessage = "File deleted.\n";
            }
            if (watchEvent->mask & IN_ACCESS) {
                notificatioMessage = "File accessed.\n";
            }
            if (watchEvent->mask & IN_CLOSE_WRITE) {
                notificatioMessage = "File written and closed.\n";
            }
            if (watchEvent->mask & IN_MODIFY) {
                notificatioMessage = "File modified.\n";
            }
            if (watchEvent->mask & IN_MOVE_SELF) {
                notificatioMessage = "File moved.\n";
            }

            /* If an acction occoured not in our wath mask, we can proceed */
            if (notificatioMessage == NULL) {
                continue;
            }

            printf("%s\n", notificatioMessage);
        }
    }
}
