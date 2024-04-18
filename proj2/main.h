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

#define skierMAX    =
#define stopsMAX    =
#define busMIN      =
#define busMAX      =
#define waitMAX     =
#define tripMAX     =
//#define TODO konstatne kontroly a premenne
//#define

int main();
void skier();
void skibus();

#endif