/**
 * @author Dylan Duhamel {@literal duhadm19@alumni.wfu.edu}
 * @date Feb. 1, 2024
 * Filestem deamon to notify modifications to given filemonitor
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

#include <sys/inotify.h>

#define EXT_SUCCESS 0
#define EXT_FAILURE_TOO_FEW_ARGS 1
#define EXT_ERR_INIT_INOTIFY 2

int IeventQueue = -1;

int main(int argc, char **argv)
{
  char *basePath = NULL;
  char *token = NULL;

  /* Bitwise or mask. */
  const uint32_t watchMask = IN_CREATE | IN_DELETE | IN_ACCESS | IN_CLOSE_WRITE | IN_MODIFY | IN_MOVE_SELF;

  if (argc < 2)
  {
    fprintf(stderr, "USAGE: filemonitor PATH\n");
    exit(EXT_FAILURE_TOO_FEW_ARGS);
  }

  /* Allocate memory for path str. */
  basePath = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));
  strcpy(basePath, argv[1]);

  token = strtok(basePath, "/");

  while (token != NULL)
  {
    basePath = token;
    token = strtok(NULL, "/");
  }
  
  IeventQueue = inotify_init();
  if (IeventQueue == -1)
  {
    fprintf(stderr, "Error initializing inotify instance.");
    exit(EXT_ERR_INIT_INOTIFY);
  }

  while (true)
  {

  }
}
