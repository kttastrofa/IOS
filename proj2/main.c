/**
    * @name     IOS projekt vol2
    * @author  Katarina Meciarova ~ xmeciak00
    * @date    18.4.2024 (updated: 24.4.2024)
*/

#include "main.h"

/// main function
/// \param argc checks how many arguments were used
/// \param argv checks what content in arguments were used
/// \return 1 if failed, 0 if succeeded
int main(int argc, char *argv[]){
    if (argc!=6) {
        perror("Too many or too little arguments, only 1+5 accepted.\n");
        exit (1);
    }
/// convert arguments to integers
        L  = atoi(argv[1]); //skiers
        Z  = atoi(argv[2]); //stops
        K  = atoi(argv[3]); //capacity of the bus
        TL = atoi(argv[4]); //maximum time to wait for skiers
        TB = atoi(argv[5]); //maximum time to wait for the bus
/// what if someone gives incompatible values? PERROR! ~ we do warn the user :) and exit with value 1
        if (!(skierMAX && stopsMAX && stopsMIN && busMIN && busMAX && WAITskier && WAITbus)){
            perror("Wrong values used, contact the author for more information.\n");
            exit (1);
        }
/// initiate file for writing
        file = fopen("proj2_11.4.10.4.5?74.out", "w");
        if (file == NULL) {
            perror("Error opening file.\n");
            exit (1);
        }
/// initiate shared memory for needed variables as semaphores or integers meant for checking the "stav"
        int shmstart = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
        if (shmstart < 0) {
            perror("error shmget");
            fclose(file);
            exit(1);
        }
        start = shmat(shmstart, NULL, 0);
        if (leave == (sem_t*) -1) {
            perror("error shmat");
            fclose(file); // taking care of the memory, if failed
            exit(1);
        }
        sem_init(start, 1, -L); // initiate semaphore meant for proper start ~ bus after skiers

        int shmstop = shmget(IPC_PRIVATE, sizeof(sem_t)*Z, IPC_CREAT | 0666);
        if (shmstop < 0) {
            perror("error shmget");
            error1(); // taking care of the memory, if failed
            exit(1);
        }
        stop = shmat(shmstop, NULL, 0);
        if (stop == (sem_t*) -1) {
            perror("error shmat");
            error1(); // taking care of the memory, if failed
            exit(1);
        }
        // we initiate the semaphore few lines below, with idZ and ARR trough for loop

        int shmleave = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
        if (shmleave < 0) {
            perror("error shmget");
            error2(); // taking care of the memory, if failed
            exit(1);
        }
        leave = shmat(shmleave, NULL, 0);
        if (leave == (sem_t*) -1) {
            perror("error shmat");
            error2(); // taking care of the memory, if failed
            exit(1);
        }
        sem_init(leave, 1, 0); // initiate semaphore meant for proper leave of the bus

        int shmfinal = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
        if (shmfinal < 0) {
            perror("error shmget");
            error3(); // taking care of the memory, if failed
            exit(1);
        }
        final = shmat(shmfinal, NULL, 0);
        if (final == (sem_t*) -1) {
            perror("error shmat");
            error3(); // taking care of the memory, if failed
            exit(1);
        }
        sem_init(final, 1, 0); // initiate semaphore meant for proper arriving to the final

        int shmA = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmA < 0) {
            perror("error shmget");
            error4(); // taking care of the memory, if failed
            exit(1);
        }
        A = shmat(shmA, NULL, 0);
        if (A == (int*) -1) {
            perror("error shmat");
            error4(); // taking care of the memory, if failed
            exit(1);
        }
        *A=0; // variable of the number of actions

        int shmidZ = shmget(IPC_PRIVATE, sizeof(int)*Z, IPC_CREAT | 0666);
        if (shmidZ < 0) {
            perror("error shmget");
            error5(); // taking care of the memory, if failed
            exit(1);
        }
        idZ = shmat(shmidZ, NULL, 0);
        if (idZ == (int*) -1) {
            perror("error shmat");
            error5(); // taking care of the memory, if failed
            exit(1);
        }
        // we initiate the variable few lines below, with sem.stop and ARR trough for loop

    int shmARR = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmARR < 0) {
            perror("error shmget");
            error6(); // taking care of the memory, if failed
            exit(1);
        }
        ARR = shmat(shmARR, NULL, 0);
        if (ARR == (int*) -1) {
            perror("error shmat");
            error6(); // taking care of the memory, if failed
            exit(1);
        }

        for (int i=0; i<Z; i++){
            idZ[i]=i+1; // variable meant for proper locating of skiers || bus trough id of stops
            sem_init(&stop[i], 1, 0); // semaphore for stops ~ proper boarding etd
            ARR[i]=0; // variable for counting skiers waiting on specific stop, diff by id of stops
        }

        int shmBOARD = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmBOARD < 0) {
            perror("error shmget");
            error7(); // taking care of the memory, if failed
            exit(1);
        }
        BOARD = shmat(shmBOARD, NULL, 0);
        if (BOARD == (int*) -1) {
            perror("error shmat");
            error7(); // taking care of the memory, if failed
            exit(1);
        }
        *BOARD=0; // variable for counting skiers on board of the bus ~ controlled by constant variable of capacity

        int shmwaiting = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmwaiting < 0) {
            perror("error shmget");
            error8(); // taking care of the memory, if failed
            exit(1);
        }
        waiting = shmat(shmwaiting, NULL, 0);
        if (waiting == (int*) -1) {
            perror("error shmat");
            error8(); // taking care of the memory, if failed
            exit(1);
        }
        *waiting=L; // variable for counting skiers still waiting, not on their way to ski yet

/// here we call functions as processes. "fork" takes care of having separated processes running on the "same" time
    pid_t skibusP = fork();
    if (skibusP == 0) {
        skibus();
        theEND();  // taking care of the memory before end, tbs
        exit(0);
    }
    for (int i = 1; i <= L; i++) {
        pid_t skierP = fork();
        if (skierP == 0) {
            skier(i);
            theEND(); // taking care of the memory before end, tbs
            exit(0);
        }
    }

    for (int i = 0; i < L; i++) {
        wait(NULL);
    }
    wait(NULL);

    theEND(); // taking care of the memory once again, to be sure :)
    exit (0);
}

/// skibus function takes care of the process of skibus's process. It is operated by semaphores ~ synchronized with skiers
void skibus(void){

    sem_wait(start); // waits 'till all the skiers starts

    fprintf(file,"%d: BUS: started \n", ++*A);
    fflush(file);
    int id; // initiation of local variable meant for identification of stops
/// this process~while operates until all of the skiers are not skiing already
    while (*waiting>0){
        id=0; // takes care of proper order of stops
/// this while operates until the last stop (before final) is not reached
        while (id < Z){
            srand(time(NULL) ^ (getpid()<<16)); // takes care of randomness
            usleep(rand() % TB); // random choosing of the time bus needs to get to the stop[id]

            fprintf(file, "%d: BUS: arrived to %d \n", ++*A, idZ[id]);
            fflush(file);
/// if arrived, wait 'till all skiers on current stop boarded on bus
            while (*BOARD<K && ARR[id]>0){
                sem_post(&stop[id]); // gives signal to board
                sem_wait(leave); // waits for signal to leave
            }

            fprintf(file,"%d: BUS: leaving %d \n", ++*A, idZ[id++]);
            fflush(file);
        }
        usleep(rand() % TB);

        fprintf(file, "%d: BUS: arrived to final \n", ++*A);
        fflush(file);
/// if on final, wait until every skier on board leaves
        while (*BOARD>0){
            sem_post(final); // gives signal to leave the bus
            sem_wait(leave); // waits for all the signal to leave the final stop and get on another way
        }

        fprintf(file, "%d: BUS: leaving final \n", ++*A);
        fflush(file);
    }
/// if all af the skiers are on the hill skiing already
    fprintf(file, "%d: BUS: finish \n", ++*A);
    fflush(file);
}

/// skier function takes care of the process of skiers's processes. By semaphores ~ synchronized with skibus's process
/// \param idL differentiate and identifies skiers
void skier(int idL){
    srand(time(NULL) ^ (getpid()<<16)); // randomness
    int id = rand() % Z; // random choosing of the stop for the skier idL

    fprintf(file, "%d: L %d: started \n", ++*A, idL);
    fflush(file);

/// if started, signal the bus
    sem_post(start);

    usleep(rand() % TL+0); // random choosing of the time to walk to stop[id]

    fprintf(file, "%d: L %d: arrived to %d \n", ++*A, idL, idZ[id]);
    fflush(file);
    ARR[id]+=1; // if arrived, increment the ARR[id], based on stop[id], to know, how many will board on stop[id]
///   wait for the signal to board from the bus
    sem_wait(&stop[id]);
    ARR[id]-=1; // if signaled, decrement ARR[id]

    fprintf(file, "%d: L %d: boarding \n", ++*A, idL);
    fflush(file);
    ++*BOARD; // increment BORAD to know, how many will leave on the final stop
/// signal the bus to leave
    sem_post(leave);
/// wait for the signal to leave the board
    sem_wait(final);
    --*BOARD; // if signaled, decrement BOARD
    --*waiting; // also decrement waiting, so we know, if is someone still waiting for skiing
/// if left the board, signal bus for leave
    sem_post(leave);

    fprintf(file, "%d: L %d: going to ski \n", ++*A, idL);
    fflush(file);
}

/// if shm_stop failed
void error1(void){
    sem_destroy(start);
    shmdt(start);
    shmctl(shmstart, IPC_RMID, NULL);
    fclose(file);
}

/// if shm_leave failed
void error2(void){
    error1();
    sem_destroy(stop);
    shmdt(stop);
    shmctl(shmstop, IPC_RMID, NULL);
}

/// if shm_final failed
void error3(void){
    error2();
    sem_destroy(leave);
    shmdt(leave);
    shmctl(shmleave, IPC_RMID, NULL);
}

/// if shm_A failed
void error4(void){
    error3();
    sem_destroy(final);
    shmdt(final);
    shmctl(shmfinal, IPC_RMID, NULL);
}

/// if shm_idZ failed
void error5(void){
    error4();
    shmdt(A);
    shmctl(shmA, IPC_RMID, NULL);
}

/// if shm_ARR failed
void error6(void){
    error5();
    shmdt(idZ);
    shmctl(shmidZ, IPC_RMID, NULL);
}

/// if shm_BOARD failed
void error7(void){
    error6();
    shmdt(ARR);
    shmctl(shmARR, IPC_RMID, NULL);
}

/// if shm_waiting failed
void error8(void){
    error7();
    shmdt(BOARD);
    shmctl(shmBOARD, IPC_RMID, NULL);
}

/// if the whole programs ends (success || fail)
void theEND(){
    error8();
    shmdt(waiting);
    shmctl(shmwaiting, IPC_RMID, NULL);
}

/// the END