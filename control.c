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

union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };

int main(int argc, char* argv[]) {

  int shmid;
  int semid;
  int fd;
  char* line = malloc(256);

  if (strcmp(argv[1],"-c") == 0) {
    shmid = shmget(ftok("mem",'S'), 256, IPC_CREAT | 0644);
    semid = semget(ftok("sem",'R'), 1, IPC_CREAT | 0644);
    fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
    line = shmat(shmid,0,0);
    line[0] = 0;
    union semun data;
    data.val = 1;
    semctl(semid, 0, SETVAL, data);
    printf("Shared memory, semaphore, and file were all created!\n");
    return 1;
  }

  if (strcmp(argv[1],"-r") == 0) {
    shmid = shmget(ftok("mem",'S'), 256, 0);
    semid = semget(ftok("sem",'R'), 1, 0);
    fd = open("story.txt", O_RDONLY);
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = -1;
    semop(semid, &operation, 1);
    operation.sem_op = 1;
    semop(semid, &operation, 1);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID, 0);
    fd = open("story.txt", O_RDONLY);
    char* story = malloc(8192);
    read(fd, story, 8192);
    printf("The Enitre Story: %s\n", story);
    remove("story.txt");
    printf("Everything has now been removed!\n");
    return 1;
  }

  if (strcmp(argv[1],"-v") == 0) {
    fd = open("story.txt", O_RDONLY);
    char* story = malloc(8192);
    read(fd, story, 8192);
    printf("The Enitre Story: %s\n", story);
    return 1;
  }

  printf("Please input a proper argument!\n");
  return 0;
}
