////////////////////////////////////////////////
///  by:     Dan Ben Ami - 316333079
///          Tom kessous - 206018749
///////////////////////////////////////////////
///===========   includes  ==========================
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


///===================   defines  ==========================
/// Probabilities
#define HIT_RATE 					0.5
#define WR_RATE 					0.5

/// System properties
#define N 5
#define USED_SLOTS_TH 3

/// Times
#define SIM_TIME 					1	//in seconds
#define TIME_BETWEEN_SNAPSHOTS  100000000 			//in ns
#define MEM_WR_T 				1000		//in ns
#define HD_ACCS_T 				100000  	//in ns
#define INTER_MEM_ACCS_T 			10000	    	//in ns


/// msg types
#define WR_req 1
#define RD_req 2
#define MMU_Ack 3
#define HD_req 4
#define HD_Ack 5



///=====================   structures  ==========================
typedef struct message {		
    long type;
    char chr;  // 1=proc1, 2=proc2, 3=mmu, 4=HD
} msg;

typedef struct memory {
    int valid[N];
    int dirty[N];
    int valid_cnt;		//Count the amount of valid pages.
    int dirty_cnt;		//Count the amount of dirty pages.
    int clock_ptr;		//Clock ptr indicate which page will evacuate.
} memory;
///====================  global variables  =================
int msgs_Qs[4];         //Queues = [proc1, proc2, MMU, HD]
pid_t pids[4];
int run_system;	
memory mem;
pthread_mutex_t mem_lock;
pthread_cond_t mmu_cond;
pthread_cond_t evicter_cond;
pthread_t tids[3];

///=======================   prototypes  ==========================
void process(char proc_num);
void init();
int open_queue(key_t keyval);
void my_msgsnd(int queue,const char proc_num, long type);
msg my_msgrcv(int qid,int type);
pid_t my_fork();
void create_processes();
void terminate_mmu_from_mmu();
void hit_case(msg* rcv_msg_ptr);
void mmu_main();
void my_lock(pthread_mutex_t *mutex);
void my_unlock(pthread_mutex_t *mutex);
void my_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
void my_cond_signal(pthread_cond_t *cond);
void printer();
void HD();
void evicter();
void my_ptherad_create(pthread_t *tid, void* func);
void kill_them_all();
void mmu();
void terminate_mmu_from_father();
///========================   functions     ============================

int main() {
    init();
    create_processes();
    return 0;
}


void process(char proc_num){
///This function simulate a process that invoke memory accesses every INTER_MEM_ACCS_T ns by requesting from the MMU.
    int iter_cnt=0;
    srand(time(0));
    int max_iters = (int)((SIM_TIME*1e10)/INTER_MEM_ACCS_T);   //Wait 10 times the SIM_TIME (endless loop for our purposes)
    while(iter_cnt < max_iters) {
        nanosleep((const struct timespec[]) {{INTER_MEM_ACCS_T / (int) (1e9),
                                                     ((long) (INTER_MEM_ACCS_T % ((int) 1e9)))}}, NULL);
        iter_cnt++;
        if ((double) rand() / (double) RAND_MAX <= WR_RATE) {
            //Invoke write operation
            my_msgsnd(msgs_Qs[2], proc_num, WR_req);
        } else {
            //Invoke read operation
            my_msgsnd(msgs_Qs[2], proc_num, RD_req);
        }
        my_msgrcv(msgs_Qs[(int)(proc_num-'1')],MMU_Ack);	//Wait for Ack from the MMU.
    }
}


int open_queue( key_t keyval ){
//This function create queue with read and write permissions.
    int qid;
    if((qid = msgget( keyval, IPC_CREAT | 0666 )) == -1){
        printf("ERROR while open Queue.");
        perror("IPC error: msgget"); exit(1);    }
    return qid;
}


void init() {
//This function initialize the queues.
    int queue = 0;
    key_t key;

    /// create queues
    for (queue = 0; queue < 4; queue++) {
        if ((key = ftok("/tmp", 'a'+queue)) == (key_t) -1) {
            perror("IPC error: ftok");
            exit(1);
        }
        msgs_Qs[queue] = open_queue(key);
    }
}


void create_processes(){
//This fumction open process: process1, process2, mmu and HD.
//all the above process are childs of the father process (who create them).
    int i;
    pid_t pid;
    for(i=0;i<4;i++){
        pid = my_fork();
        if(!pid){
            switch (i) {
                case 0:
                    process('1');
                    break;
                case 1:
                    process('2');
                    break;
                case 2:
                    signal(SIGKILL, terminate_mmu_from_father);	//Define signal handler for SIGKILL in the mmu process.
                    mmu();
                    break;
                case 3:
                    HD();
                    break;
                default:
                    break;
            }
            break;
        }
        else if(i==3){                   //Father process
            pids[i] = pid;
            signal(SIGKILL,kill_them_all);				//Define signal handler for SIGKILL in the father process.
            sleep(SIM_TIME);						//sleep SIM_TIME for the duration of the simulation.
            kill_them_all();						//In the end of the simulation we kill all the process.
            printf("Successfully finished sim\n");
        }
        else pids[i] = pid;
    }
}


void kill_them_all() {
//This function kill all the process that we created and destroy the queues.
    int j=0;
    for(j=0;j<4;j++) {
        kill(pids[j],SIGKILL);
        wait(NULL);
        msgctl(msgs_Qs[j],IPC_RMID,NULL);
    }
}


void my_msgsnd(int queue,const char proc_num, long type){
//This fumction send a message to the specific queue.
    msg send_msg;
    send_msg.chr = proc_num;
    send_msg.type = type;
    if(msgsnd(queue, &send_msg, sizeof(send_msg.chr), 0) == -1){
        perror("IPC error in msgsnd");
        kill_them_all();
    }
}


msg my_msgrcv(int qid,int type){
//This function receive a message from qid.
    msg recieved_msg;
    if(msgrcv(qid,&recieved_msg,sizeof(recieved_msg.chr),type,0) == -1){
        perror("IPC error in msgrcv");
        kill_them_all();
    }
    return recieved_msg;
}


pid_t my_fork(){
//This fumction fork a process and check if the fork succeeded.
    pid_t pid;
    if((pid = fork())== -1 ){
        perror("fork failed");
        kill_them_all();
    }
    return pid;
}


void mmu(){
//This is the mme process, first we initialize the memory.
    mem.clock_ptr = 0;
    mem.valid_cnt = 0;
    mem.dirty_cnt =0;
    for(int i =0; i<N;i++) {
        mem.valid[i] = 0;
        mem.dirty[i] = 0;
    }
    //initialize the mutex and cond variable.
    if(pthread_mutex_init(&mem_lock,NULL)){
        perror("Mutex init failed");
        terminate_mmu_from_mmu();
    }
    if(pthread_cond_init(&evicter_cond,NULL)){
        perror("Cond init failed");
        terminate_mmu_from_mmu();
    }
    if(pthread_cond_init(&mmu_cond,NULL)){
        perror("Cond init failed");
        terminate_mmu_from_mmu();
    }
    run_system = 1;		//start the simulation
    //Create mmu_main, evicter and printer threads.
    my_ptherad_create(&tids[0],(void*) mmu_main);
    my_ptherad_create(&tids[1],(void*) evicter);
    my_ptherad_create(&tids[2],(void*) printer);
    sleep(SIM_TIME);
    run_system = 0;		//When the simulation finish set run_system to 0.
}


void mmu_main(){
//This is the mmu_main thread.
    msg rcv_msg;
    int hit_miss;   //hit=0, miss=1
    srand(time(0));
    while(run_system){
        rcv_msg = my_msgrcv(msgs_Qs[2],-2);  //recieve all messages except HD_Ack type.
        if(!mem.valid_cnt) {         //Page fault - empty memory
            my_msgsnd(msgs_Qs[3],'m',HD_req);	//Send HD request
            my_msgrcv(msgs_Qs[2], HD_Ack);	//Wait to receive HD Ack.
            if(rcv_msg.type == WR_req) {                    //write when mem empty
                nanosleep((const struct timespec[]) {{MEM_WR_T / (int) (1e9),
                                                             ((long) (MEM_WR_T % ((int) 1e9)))}}, NULL);
            }
            my_lock(&mem_lock);		//lock the mutex
            if(rcv_msg.type == WR_req) {
                mem.dirty_cnt++;
                mem.dirty[0] = 1;
            }
            mem.valid_cnt++;
            mem.valid[0] = 1;
            mem.clock_ptr = 0;
            my_unlock(&mem_lock);		//unlock the mutex
            my_msgsnd(msgs_Qs[(int)(rcv_msg.chr - '1')], 'm', MMU_Ack);	//Send Ack back to the process who sent the reqest.
        }
        else{           //memory is not empty
            hit_miss = (float)rand()/(float)RAND_MAX < HIT_RATE ? 0:1;		
            if(!hit_miss) hit_case(&rcv_msg);       //hit
            else{       //miss
                if(mem.valid_cnt == N) {        //memory is full
                    my_lock(&mem_lock);
                    my_cond_signal(&evicter_cond);		//wake up the evictor
                    my_cond_wait(&mmu_cond, &mem_lock);	//wait to the evicter
                    my_unlock(&mem_lock);
                }
                my_msgsnd(msgs_Qs[3],'m', HD_req);		//Send HD request	
                my_msgrcv(msgs_Qs[2], HD_Ack);			//Wait to receive HD Ack.
                my_lock(&mem_lock);
                if(rcv_msg.type == WR_req) {
                    mem.dirty_cnt++;
                    mem.dirty[(mem.clock_ptr + mem.valid_cnt) % N] = 1;
                }
                mem.valid[(mem.clock_ptr + mem.valid_cnt) % N] = 1;
                mem.valid_cnt++;
                my_unlock(&mem_lock);
                my_msgsnd(msgs_Qs[(int) (rcv_msg.chr - '1')], 'm', MMU_Ack);		
                //Send Ack back to the process who sent the reqest.
            }
        }
    }
}


void hit_case(msg* rcv_msg_ptr){
//This function handle hit operation.
    int idx;
    if ((*rcv_msg_ptr).type == RD_req) {		
        my_msgsnd(msgs_Qs[(int) ((*rcv_msg_ptr).chr - '1')], 'm', MMU_Ack);  //In case of hit read
    }
    else {          ////In case of hit write
        nanosleep((const struct timespec[]) {{MEM_WR_T / (int) (1e9),
                                                     ((long) (MEM_WR_T % ((int) 1e9)))}}, NULL);
        my_lock(&mem_lock);
        idx = (mem.clock_ptr+rand() % mem.valid_cnt) % N;
        mem.dirty_cnt = (mem.dirty[idx] == 1 )? mem.dirty_cnt : mem.dirty_cnt+1;
        mem.dirty[idx] = 1;
        my_unlock(&mem_lock);
        my_msgsnd(msgs_Qs[(int) ((*rcv_msg_ptr).chr - '1')], 'm', MMU_Ack);

    }
}


void evicter(){
//This function evict pages from the memory in FIFO manner.
    while(run_system) {
        my_lock(&mem_lock);
        my_cond_wait(&evicter_cond,&mem_lock);		//Wait for the mmu_main signal
        my_unlock(&mem_lock);
        while(mem.valid_cnt >= USED_SLOTS_TH){
            if(!mem.valid_cnt) break;   //relevant just in case USED_SLOTS_TH=1
            if(mem.dirty[mem.clock_ptr]) {
                my_msgsnd(msgs_Qs[3],'m', HD_req);
                my_msgrcv(msgs_Qs[2], HD_Ack);
            }
            my_lock(&mem_lock);
            mem.dirty_cnt = (mem.dirty[mem.clock_ptr]== 1) ? mem.dirty_cnt-1 : mem.dirty_cnt;
            mem.dirty[mem.clock_ptr] = 0;
            mem.valid[mem.clock_ptr] = 0;
            mem.valid_cnt--;
            mem.clock_ptr = (mem.clock_ptr + 1) % N;
            my_cond_signal(&mmu_cond);
            my_unlock(&mem_lock);
        }
    }
}


void my_lock(pthread_mutex_t *mutex){
//Lock the mutex
    if(pthread_mutex_lock(mutex)){
        perror("Mutex lock failed");
        terminate_mmu_from_mmu();
    }
}


void my_unlock(pthread_mutex_t *mutex){
//Unlock the mutex
    if(pthread_mutex_unlock(mutex)){
        perror("Mutex unlock failed");
        terminate_mmu_from_mmu();
    }
}


void my_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex){
//Condition variable wait function.
    if(pthread_cond_wait(cond, mutex)){
        perror("cond wait failed");
        terminate_mmu_from_mmu();
    }
}


void my_cond_signal(pthread_cond_t *cond){
//Condition variable signal function.
    if(pthread_cond_signal(cond)){
        perror("cond signal failed");
        terminate_mmu_from_mmu();
    }
}


void printer(){
//This function print the memory every TIME_BETWEEN_SNAPSHOTS ns.
    int i,mem_state;
    int valid[N], dirty[N];
    while(run_system) {
        my_lock(&mem_lock);
        memcpy(dirty, mem.dirty, sizeof(int) * N);
        memcpy(valid, mem.valid, sizeof(int) * N);
        my_unlock(&mem_lock);
        for (i = 0; i < N; i++) {
            if ((valid[i]) && (!dirty[i])) mem_state = '0';
            else if (valid[i]) mem_state = '1';
            else mem_state = '-';
            printf("%d|%c\n", i, mem_state);
        }
        printf("\n\n");
        nanosleep((const struct timespec[]) {{TIME_BETWEEN_SNAPSHOTS / (int) (1e9),
                                                     ((long) (TIME_BETWEEN_SNAPSHOTS % ((int) 1e9)))}}, NULL);
    }
}


void HD(){
//This is HD process.
    msg rcv_msg;
    int iter_cnt=0;
    int max_iters = (int)((SIM_TIME*1e10)/HD_ACCS_T);   //Wait 10 times the SIM_TIME (endless loop for our purposes)
    while(iter_cnt < max_iters){
        rcv_msg = my_msgrcv(msgs_Qs[3],HD_req);	//Wait for Hd reqests.
        nanosleep((const struct timespec[]) {{HD_ACCS_T / (int) (1e9),
                                                     ((long) (HD_ACCS_T % ((int) 1e9)))}}, NULL);
        iter_cnt++;
        my_msgsnd(msgs_Qs[2],'h',HD_Ack);		//Send HD Ack.
    }
}


void my_ptherad_create(pthread_t *tid, void* func){
//Create threads.
    int error;
    error = pthread_create(tid, NULL, func, NULL);
    if (error != 0) {
        printf("\nThread can't be created :[%s]", strerror(error));
        terminate_mmu_from_mmu();
    }
}


void terminate_mmu_from_mmu(){
//In case somthing went wrong the simulation terminate from the inside of the mmu.
    run_system = 0;
    pthread_mutex_destroy(&mem_lock);
    pthread_cond_destroy(&evicter_cond);
    pthread_cond_destroy(&mmu_cond);
    kill(getppid(),SIGKILL);
}


void terminate_mmu_from_father(){
//Signal handler for the SIGKILL in mmu process.
    if(run_system) {
        run_system = 0;
        pthread_mutex_destroy(&mem_lock);
        pthread_cond_destroy(&evicter_cond);
        pthread_cond_destroy(&mmu_cond);
    }
    kill(getpid(),SIGKILL);
}



