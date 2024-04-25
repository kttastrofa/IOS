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

#define skierMIN    (L > 0)
#define skierMAX    (L < 20000)
#define stopsMAX    (Z <= 10)
#define stopsMIN    (Z > 0)
#define busMIN      (K >= 10)
#define busMAX      (K <= 100)
#define WbusMIN     (0 <= TB)
#define WbusMAX     (TB <= 1000)
#define WskierMIN   (0 <= TL)
#define WskierMAX   (TL <= 10000)

//needed variables
int L, Z, K, TL, TB, *A, *idZ, *ARR, *BOARD, *waiting;
sem_t *start, *stop, *leave, *final;
FILE *file;

//main functions, also some (skier && skibus) implemented as processes
int main(int argc, char *argv[]);
void skier(int idL);
void skibus(void);


#endif