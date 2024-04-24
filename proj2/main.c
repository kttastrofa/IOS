/**
* @name     IOS projekt vol2
 * @author  Katarina Meciarova ~ xmeciak00
 * @date    18.4.2024
*/

#include "main.h"



int main(int argc, char *argv[]){
    if (argc!=6) {
        perror("Too many or too little arguments, only 1+5 accepted.\n");
        exit (1);
    }
    else {
        L  = atoi(argv[1]);
        Z  = atoi(argv[2]);
        K  = atoi(argv[3]);
        TL = atoi(argv[4]);
        TB = atoi(argv[5]);

        if (!(skierMAX || stopsMAX || stopsMIN || busMIN || busMAX || WAITskier || WAITbus)){
            perror("Wrong values used, contact the author for more information.\n");
            exit (1);
        }

        file = fopen("proj2.out", "w");
        if (file == NULL) {
            perror("Error opening file.\n");
            exit (1);
        }

        int shmstart = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
        if (shmstart < 0) {
            perror("error shmget");
            exit(1);
        }
        leave = shmat(shmstart, NULL, 0);
        if (leave == (sem_t*) -1) {
            perror("error shmat");
            exit(1);
        }
        sem_init(start, 1, L);


        int shmstop = shmget(IPC_PRIVATE, sizeof(sem_t)*Z, IPC_CREAT | 0666);
        if (shmstop < 0) {
            perror("error shmget");
            exit(1);
        }
        stop = shmat(shmstop, NULL, 0);
        if (stop == (sem_t*) -1) {
            perror("error shmat");
            exit(1);
        }
        //inicializujeme o par riadkov nizsie spolu s shm of stops

        int shmleave = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
        if (shmleave < 0) {
            perror("error shmget");
            exit(1);
        }
        leave = shmat(shmleave, NULL, 0);
        if (leave == (sem_t*) -1) {
            perror("error shmat");
            exit(1);
        }
        sem_init(leave, 1, 0);

        int shmfinal = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
        if (shmfinal < 0) {
            perror("error shmget");
            exit(1);
        }
        final = shmat(shmfinal, NULL, 0);
        if (final == (sem_t*) -1) {
            perror("error shmat");
            exit(1);
        }
        sem_init(final, 1, 1);

        
        int shmA = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmA < 0) {
            perror("error shmget");
            exit(1);
        }
        A = shmat(shmA, NULL, 0);
        if (A == (int*) -1) {
            perror("error shmat");
            exit(1);
        }
        *A=0;

        int shmidZ = shmget(IPC_PRIVATE, sizeof(int)*Z, IPC_CREAT | 0666);
        if (shmidZ < 0) {
            perror("error shmget");
            exit(1);
        }
        idZ = shmat(shmidZ, NULL, 0);
        if (idZ == (int*) -1) {
            perror("error shmat");
            exit(1);
        }


        int shmBOARD = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmBOARD < 0) {
            perror("error shmget");
            exit(1);
        }
        BOARD = shmat(shmBOARD, NULL, 0);
        if (BOARD == (int*) -1) {
            perror("error shmat");
            exit(1);
        }
        *BOARD=0;

        int shmARR = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmARR < 0) {
            perror("error shmget");
            exit(1);
        }
        ARR = shmat(shmARR, NULL, 0);
        if (ARR == (int*) -1) {
            perror("error shmat");
            exit(1);
        }

        for (int i=0; i<Z; i++){
            idZ[i]=i;
            sem_init(&stop[i], 1, 1);
            ARR[i]=0;
        }

        int shmwaiting = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmwaiting < 0) {
            perror("error shmget");
            exit(1);
        }
        waiting = shmat(shmwaiting, NULL, 0);
        if (waiting == (int*) -1) {
            perror("error shmat");
            exit(1);
        }
        *waiting=L;


    }


    for (int i = 1; i <= L; i++) {
        pid_t skierP = fork();
        if (skierP == 0) {
            skier(i);
            exit(0);
        }
    }

    pid_t skibusP = fork();
    if (skibusP == 0) {
        skibus();
        exit(0);
    }


    for (int i = 0; i < L; i++) {
        wait(NULL);
    }
    wait(NULL);


    if (!(*waiting==0 || *BOARD==0)){
        perror("Processes failed.\n");
        exit (1);
    }


    sem_destroy(start);
    shmdt(start);
    shmctl(shmstart, IPC_RMID, NULL);
    sem_destroy(stop);
    shmdt(stop);
    shmctl(shmstop, IPC_RMID, NULL);
    sem_destroy(leave);
    shmdt(leave);
    shmctl(shmleave, IPC_RMID, NULL);
    sem_destroy(final);
    shmdt(final);
    shmctl(shmfinal, IPC_RMID, NULL);

    shmdt(A);
    shmctl(shmA, IPC_RMID, NULL);
    shmdt(idZ);
    shmctl(shmidZ, IPC_RMID, NULL);
    shmdt(BOARD);
    shmctl(shmBOARD, IPC_RMID, NULL);
    shmdt(ARR);
    shmctl(shmARR, IPC_RMID, NULL);
    shmdt(waiting);
    shmctl(shmwaiting, IPC_RMID, NULL);

    fclose(file);

    exit (0);

}


void skibus(){

    sem_wait(start);

    fprintf(file,"%d: BUS: started \n", ++*A);
    fflush(file);
    int id=0;
    while (*waiting!=0){

        while (idZ[id] != Z){
            srand(time(NULL) ^ (getpid()<<16));
            usleep(rand() % TB+0);

            fprintf(file, "%d: BUS: arrived to %d \n", ++*A, idZ[id]);
            fflush(file);

            while (*BOARD<K && ARR[id]>0){
                sem_post(&stop[id]);
                sem_wait(leave);
            }
            
            fprintf(file,"%d: BUS: leaving %d \n", ++*A, idZ[id++]);
            fflush(file);
        }
        usleep(rand() % TB+0);
        
        fprintf(file, "%d: BUS: arrived to final \n", ++*A);
        fflush(file);
        sem_post(final);
        
        while (*BOARD>0){
            sem_wait(leave);
            *waiting-=1;
        }
        
        fprintf(file, "%d: BUS: leaving final \n", ++*A);
        fflush(file);
    }

    fprintf(file, "%d: BUS: finish \n", ++*A);
    fflush(file);
}


void skier(int idL){
    srand(time(NULL) ^ (getpid()<<16));
    int id = rand() % Z+0;

    fprintf(file, "%d: L %d: started \n", ++*A, idL);
    fflush(file);
    sem_post(start);
    
    usleep(rand() % TL+0);
    
    fprintf(file, "%d: L %d: arrived to %d \n", ++*A, idL, idZ[id]);
    fflush(file);
    ARR[id]+=1;
   
    sem_wait(&stop[id]);
    ARR[id]-=1;
    fprintf(file, "%d: L %d: boardnig \n", ++*A, idL);
    fflush(file);
    ++*BOARD;
    sem_post(leave);


    sem_wait(final);
    *BOARD-=1;
    fprintf(file, "%d: L %d: going to ski \n", ++*A, idL);
    fflush(file);
}