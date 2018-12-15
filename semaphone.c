#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main() {

  int shmid = shmget(ftok("mem",'S'), 256, 0);
  int semid = semget(ftok("sem",'R'), 1, 0);
  int fd = open("story.txt", O_RDONLY);
  char* line = malloc(256);

  printf("Making sure resources are available\n");
  struct sembuf operation;
  operation.sem_op = -1;
  operation.sem_flg = SEM_UNDO;
  operation.sem_num = 0;
  semop(semid, &operation, 1);

  line = shmat(shmid,0,0);
  printf("Last Line: %s\n", line);
  printf("Input a Line: ");
  char new[256];
  fgets(new, 256, stdin);
  new[strlen(new)-1] = 0;
  strcpy(line, new);
  shmdt(line);

  fd = open("story.txt", O_WRONLY | O_APPEND);
  strcat(new, " ");
  write(fd, new, strlen(new));
  close(fd);
  operation.sem_op = 1;
  semop(semid,&operation,1);

  return 1;
}
