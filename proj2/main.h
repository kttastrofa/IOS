/**
* @name     IOS projekt vol2
 * @author  Katarina Meciarova ~ xmeciak00
 * @date    18.4.2024
*/

#ifndef HEADofMAIN
#define HEADofMAIN

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


#define skierMAX    (L < 20000)
#define stopsMAX    (Z <= 10)
#define stopsMIN    (Z > 0)
#define busMIN      (K >= 10)
#define busMAX      (K <= 100)
#define WAITskier   ((0 <= TB)&&(TB <= 10000))
#define WAITbus     ((0 <= TL)&&(TL <= 1000))

int L, Z, K, TL, TB, *A, *idZ, *ARR, *BOARD, *waiting;
//int *idL; no need

sem_t *start, *stop, *leave, *final;

FILE *file;

int shmstart, shmstop, shmleave, shmfinal, shmA, shmidZ, shmBOARD, shmARR, shmwaiting;


int main(int argc, char *argv[]);
void skier(int idL);
void skibus();
void THEEND(int in);

#endif