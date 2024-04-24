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

        L  = atoi(argv[1]);
        Z  = atoi(argv[2]);
        K  = atoi(argv[3]);
        TL = atoi(argv[4]);
        TB = atoi(argv[5]);


        if (!(skierMAX && stopsMAX && stopsMIN && busMIN && busMAX && WAITskier && WAITbus)){
            perror("Wrong values used, contact the author for more information.\n");
            exit (1);
        }

        file = fopen("proj2.out", "w");
        if (file == NULL) {
            perror("Error opening file.\n");
            THEEND(0);
            exit (1);
        }

        int shmstart = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
        if (shmstart < 0) {
            perror("error shmget");
            THEEND(90);
            exit(1);
        }
        start = shmat(shmstart, NULL, 0);
        if (leave == (sem_t*) -1) {
            perror("error shmat");
            THEEND(91);
            exit(1);
        }
        sem_init(start, 1, (0-(L+1)));


        int shmstop = shmget(IPC_PRIVATE, sizeof(sem_t)*Z, IPC_CREAT | 0666);
        if (shmstop < 0) {
            perror("error shmget");
            THEEND(80);
            exit(1);
        }
        stop = shmat(shmstop, NULL, 0);
        if (stop == (sem_t*) -1) {
            perror("error shmat");
            THEEND(81);
            exit(1);
        }
        //inicializujeme o par riadkov nizsie spolu so shm of stops

        int shmleave = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
        if (shmleave < 0) {
            perror("error shmget");
            THEEND(70);
            exit(1);
        }
        leave = shmat(shmleave, NULL, 0);
        if (leave == (sem_t*) -1) {
            perror("error shmat");
            THEEND(71);
            exit(1);
        }
        sem_init(leave, 1, 0);

        int shmfinal = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
        if (shmfinal < 0) {
            perror("error shmget");
            THEEND(60);
            exit(1);
        }
        final = shmat(shmfinal, NULL, 0);
        if (final == (sem_t*) -1) {
            perror("error shmat");
            THEEND(61);
            exit(1);
        }
        sem_init(final, 1, 0);

        
        int shmA = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmA < 0) {
            perror("error shmget");
            THEEND(50);
            exit(1);
        }
        A = shmat(shmA, NULL, 0);
        if (A == (int*) -1) {
            perror("error shmat");
            THEEND(51);
            exit(1);
        }
        *A=0;

        int shmidZ = shmget(IPC_PRIVATE, sizeof(int)*Z, IPC_CREAT | 0666);
        if (shmidZ < 0) {
            perror("error shmget");
            THEEND(40);
            exit(1);
        }
        idZ = shmat(shmidZ, NULL, 0);
        if (idZ == (int*) -1) {
            perror("error shmat");
            THEEND(41);
            exit(1);
        }


        int shmBOARD = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmBOARD < 0) {
            perror("error shmget");
            THEEND(30);
            exit(1);
        }
        BOARD = shmat(shmBOARD, NULL, 0);
        if (BOARD == (int*) -1) {
            perror("error shmat");
            THEEND(31);
            exit(1);
        }
        *BOARD=0;

        int shmARR = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmARR < 0) {
            perror("error shmget");
            THEEND(20);
            exit(1);
        }
        ARR = shmat(shmARR, NULL, 0);
        if (ARR == (int*) -1) {
            perror("error shmat");
            THEEND(21);
            exit(1);
        }

        for (int i=0; i<Z; i++){
            idZ[i]=i+1;
            sem_init(&stop[i], 1, 0);
            ARR[i]=0;
        }

        int shmwaiting = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmwaiting < 0) {
            perror("error shmget");
            THEEND(10);
            exit(1);
        }
        waiting = shmat(shmwaiting, NULL, 0);
        if (waiting == (int*) -1) {
            perror("error shmat");
            THEEND(11);
            exit(1);
        }
        *waiting=L;


    for (int i = 1; i <= L; i++) {
        pid_t skierP = fork();
        if (skierP == 0) {
            skier(i);
            THEEND(11);
            exit(0);
        }
    }

    pid_t skibusP = fork();
    if (skibusP == 0) {
        skibus();
        THEEND(11);
        exit(0);
    }


    for (int i = 0; i < L; i++) {
        wait(NULL);
    }
    wait(NULL);


    if (!(*waiting==0 || *BOARD==0)){
        perror("Processes failed.\n");
        THEEND(1);
        exit (1);
    }


    THEEND(1);

    exit (0);

}


void THEEND(int in){
    switch(in){
        case 11:
            shmdt(waiting);
        case 10:
            shmctl(shmwaiting, IPC_RMID, NULL);

        case 21:
            shmdt(ARR);
        case 20:
            shmctl(shmARR, IPC_RMID, NULL);

        case 31:
            shmdt(BOARD);
        case 30:
            shmctl(shmBOARD, IPC_RMID, NULL);

        case 41:
            shmdt(idZ);
        case 40:
            shmctl(shmidZ, IPC_RMID, NULL);

        case 51:
            shmdt(A);
        case 50:
            shmctl(shmA, IPC_RMID, NULL);



        case 62:
            sem_destroy(final);
        case 61:
            shmdt(final);
        case 60:
            shmctl(shmfinal, IPC_RMID, NULL);



        case 72:
            sem_destroy(leave);
        case 71:
            shmdt(leave);
        case 70:
            shmctl(shmleave, IPC_RMID, NULL);


        case 82:
            sem_destroy(stop);
        case 81:
            shmdt(stop);
        case 80:
            shmctl(shmstop, IPC_RMID, NULL);


        case 92:
            sem_destroy(start);
        case 91:
            shmdt(start);
        case 90:
            shmctl(shmstart, IPC_RMID, NULL);

        default:
            fclose(file);
    }
}

void skibus(){

    sem_wait(start);

    //printf("%d: BUS: started \n", ++*A);
    fprintf(file,"%d: BUS: started \n", ++*A);
    fflush(file);
    int id;
    while (*waiting>0){

        id=0;
        while (id < Z){
            srand(time(NULL) ^ (getpid()<<16));
            usleep(rand() % TB);

            //printf("%d: BUS: arrived to %d \n", ++*A, idZ[id]);
            fprintf(file, "%d: BUS: arrived to %d \n", ++*A, idZ[id]);
            fflush(file);
            while (*BOARD<K && ARR[id]>0){
                sem_post(&stop[id]);
                sem_wait(leave);
            }

            //printf("%d: BUS: leaving %d \n", ++*A, idZ[id++]);
            fprintf(file,"%d: BUS: leaving %d \n", ++*A, idZ[id++]);
            fflush(file);
        }
        usleep(rand() % TB);

        //printf("%d: BUS: arrived to final \n", ++*A);
        fprintf(file, "%d: BUS: arrived to final \n", ++*A);
        fflush(file);
        
        while (*BOARD>0){
            sem_post(final);
            sem_wait(leave);
        }

        //printf("%d: BUS: leaving final \n", ++*A);
        fprintf(file, "%d: BUS: leaving final \n", ++*A);
        fflush(file);
    }

    //printf("%d: BUS: finish \n", ++*A);
    fprintf(file, "%d: BUS: finish \n", ++*A);
    fflush(file);
}


void skier(int idL){
    srand(time(NULL) ^ (getpid()<<16));
    int id = rand() % Z;

    //printf("%d: L %d: started \n", ++*A, idL);
    fprintf(file, "%d: L %d: started \n", ++*A, idL);
    fflush(file);

    
    usleep(rand() % TL+0);
    sem_post(start);

    //printf("%d: L %d: arrived to %d \n", ++*A, idL, idZ[id]);
    fprintf(file, "%d: L %d: arrived to %d \n", ++*A, idL, idZ[id]);
    fflush(file);
    ARR[id]+=1;
   
    sem_wait(&stop[id]);
    ARR[id]-=1;

    //printf("%d: L %d: boarding \n", ++*A, idL);
    fprintf(file, "%d: L %d: boarding \n", ++*A, idL);
    fflush(file);
    ++*BOARD;
    sem_post(leave);


    sem_wait(final);
    --*BOARD;
    sem_post(leave);

    //printf("%d: L %d: going to ski \n", ++*A, idL);
    fprintf(file, "%d: L %d: going to ski \n", ++*A, idL);
    fflush(file);
    --*waiting;
}