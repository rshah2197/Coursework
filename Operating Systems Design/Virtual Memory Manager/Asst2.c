// File:    my_pthread.c
// Author:  Yujie REN
// Date:    09/23/2017

// name: Xingyu Yao xy139, Ryan Shah rds216
// username of iLab:cd.cs.rutgers.edu
// iLab Server:cd.cs.rutgers.edu

#include "Asst2.h"
#include "ucontext.h"
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#define stac 40000
#define taken 1
#define untaken 0
char *memory;
char *share;            
static void *buffer;         
static void *user_space;      
int free_num;         
int free_num_SF;   
header * HEAD;
ucontext_t thr, orig,main_context,op_context;
queue run_q;
kilist* kilhim=NULL;//where threads go to be reaped
tcb *current=NULL; //One thread exevuting
ready_queue* readyQ=NULL;
//signal handler
int count = 0;
int id=0;
int SYS=0;//IF I am in OS or not used to preven sighandlers from messing up the system
int level=0;
struct itimerval it;
struct sigaction act, oact;
int lock=0;
int mutexid=1;
int firsttime=0;
int firstshare=0;
waitQueues * waitQ=NULL;
waitQueues * current_wait_Q=NULL;
page_node page_table[6144]; 
//delete later
my_pthread_mutex_t *key;
my_pthread_mutex_t *key2;
my_pthread_mutex_t *key3;
my_pthread_mutex_t *key4;
my_pthread_mutex_t *key5;



void set_block_data(header * temp, int size){
    header* new = (header*)((char *)temp + sizeof(header) + size);
    new->size = temp->size - sizeof(header) - size;
    new->free = 1;
    new->next = temp->next;
    temp->free = 0;
    temp->size = size;
    temp->next = new;
}

void Init_Block(int req, void * temp){
    mprotect( (temp), Page_Size, PROT_READ | PROT_WRITE );
    HEAD = (header *) temp;
    if (req == THREADREQ){
        HEAD->size = Page_Size-sizeof(header);
    }
    HEAD->next = NULL;
    HEAD->free = 1;
}
void Init_Share_Block(int req, void * temp){
    mprotect( (temp), Page_Size, PROT_READ | PROT_WRITE );
    HEAD = (header *) temp;
    if (req == THREADREQ){
        HEAD->size = (4*Page_Size)-sizeof(header);
    }
    HEAD->next = NULL;
    HEAD->free = 1;
}



void update_protection(int nexttid, int tid){
    int i = User_Space;
    while(i < Swap_Space) {
    
        if (page_table[i].tid == nexttid){
            mprotect( (void*)(memory + i * Page_Size), Page_Size, PROT_READ | PROT_WRITE);}
              i++;
        }
}

static void handler(int sig, siginfo_t *si, void *unused) {
    void *swapout;
    int sniped_tid, sniped_page, page_fault_page;
    page_fault_page = (si->si_addr - (void *) memory)/Page_Size;
    swapout = (char *)memory + (page_fault_page*Page_Size);
    sniped_tid = current->id;
    sniped_page = page_fault_page - User_Space;
    mprotect( (void*)(memory + page_fault_page ), Page_Size, PROT_READ | PROT_WRITE );
    //swap page
    int y; 
    void *curr_page;
    for(y = User_Space; y < 6144; y++){
        if ((page_table[y].tid == sniped_tid) && (page_table[y].page_num == sniped_page))
        {
            curr_page = memory + y * Page_Size;
            mprotect( (void*)(curr_page), Page_Size, PROT_READ | PROT_WRITE );
            mprotect( (void*)(buffer), Page_Size, PROT_READ | PROT_WRITE );
            mprotect( (void*)(swapout), Page_Size, PROT_READ | PROT_WRITE );
            int curr_page_num = (curr_page - (void *)memory)/Page_Size;
            int swapout_page_num = (swapout - (void *)memory)/Page_Size;
            page_node temp = page_table[curr_page_num];
            page_table[curr_page_num] = page_table[y];
            page_table[y] = temp;
            memcpy(buffer, curr_page, Page_Size);
            memcpy(curr_page, swapout, Page_Size);
            memcpy(swapout, buffer, Page_Size);     
            memset(buffer, 0, Page_Size);
            return;
        }
    }
    exit(0);
}
 
void* new_empty_page(){
    int current_tid;
    if(current==NULL){
         current_tid=0;
    }
    else{
        current_tid=current->id;
    }
    void* memptr;
    int y=User_Space;
    while (y < 6144){
        if (page_table[y].allocate == 0){
            page_table[y].allocate = 1;
            if(y<Swap_Space){
            free_num--;}
            else{
                free_num_SF--;
            }
            page_table[y].tid = current_tid;
            mprotect( (void*)(memory + y * Page_Size), Page_Size, PROT_READ | PROT_WRITE );
            memptr = (void*)(memory + y * Page_Size);
            return memptr; 
        }y++;
    }
    return NULL;
}

void Init_MEM(){
    memory = (char *) memalign(Page_Size , (MEM+SWAPFILE) * sizeof(char));    
    share=(char *) memalign(Page_Size , 4*Page_Size * sizeof(char));   
    buffer = (void *) memory;
    user_space = (char *)memory + User_Space*Page_Size;
    free_num = 1548;        
    free_num_SF = 4096;
    int y=1;
    void* os=memory+Page_Size;
    Init_Block(2, os);
    while(y<6144){
        page_table[y].tid = -88;
        page_table[y].page_num = 0;
        page_table[y].allocate = 0;
        y++;
    } 
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1){
        printf("Fatal error setting up signal handler.\n");
        exit(EXIT_FAILURE);
    }
}

void* request_first_page(int sig){
    int current_tid;
    if(current==NULL){
      current_tid=0;
    }else{
    current_tid=current->id;}
    void * temp = user_space;
    int y;
    void *result;
    if (sig == LIBRARYREQ){
        return buffer+Page_Size;
    }
    else if (sig == THREADREQ){
        for (y = User_Space; y < Swap_Space; y++){
            if ((page_table[y].tid == current_tid) && (page_table[y].page_num == 0)){
                if(y == User_Space) {
                    result =temp;
                    return result;                    
                }
                page_node temp = page_table[User_Space];
                page_table[User_Space] = page_table[y];
                page_table[y] = temp;
                mprotect( (void*)user_space, Page_Size, PROT_READ | PROT_WRITE );
                memcpy( buffer, (void*)user_space, Page_Size );
                memcpy( (void*)user_space, (void*)(memory + y * Page_Size), Page_Size );
                memcpy( (void*)(memory + y * Page_Size), buffer, Page_Size );
                memset( buffer, 0, Page_Size );
                result = (void*)user_space;
                return result;
            }
        }
        for (y = User_Space; y < 6144; y++){
            if (page_table[y].allocate == 0){
                if (y == User_Space) {
                    page_table[User_Space].tid = current_tid;
                    page_table[User_Space].allocate = 1;
                    page_table[User_Space].page_num = 0;
                    free_num--;
                    result = (void*)user_space;
                    Init_Block(sig, result);
                    return result;
                }
                page_table[y] = page_table[User_Space];
                if(y>=Swap_Space){free_num_SF--;}
                    else{
                free_num--;}
                page_table[User_Space].tid = current_tid;
                page_table[User_Space].allocate = 1;
                page_table[User_Space].page_num = 0;
                mprotect( (void*)(memory + y * Page_Size), Page_Size, PROT_READ | PROT_WRITE );
                mprotect( (void*)user_space, Page_Size, PROT_READ | PROT_WRITE );
                memcpy((memory + y * Page_Size), user_space, Page_Size);
                memset(user_space, 0, Page_Size);
                result = user_space;
                Init_Block(sig, result);
                return (void*)result;
            }
        }
    }
    return NULL;
}

void* myallocate(size_t size, int req){
     int current_tid;
    if(current==NULL){
        current_tid=0;
    }
    else{
        current_tid=current->id;
    }
    if (size <= 0){
        return NULL; }
    if (firsttime == 0){
        Init_MEM();
        firsttime=1; }        
    header *temp= (header *) request_first_page(req);
    if (HEAD == NULL) {
        return NULL;
    }
    void * result;
    while (temp != NULL && temp->next != NULL) {
        if (temp->free == 1 && temp->size >= size){
            break;}        
        temp = temp->next;
    }
//Case:1
    if (temp->free == 1 && temp->size <= size + sizeof(header)){
        temp->free = 0;
        result = (char *)temp + sizeof(header);
        return (void*)result;
    }
//Case: 2
    if (temp->free == 1 && temp->size >= (size + sizeof(header))){
        set_block_data(temp, size);
        result = (char *)temp + sizeof(header);
        return (void*)result;
    }
    if ((temp->free == 0)||(temp->size < size)) {
        if((free_num ==0)&& (free_num_SF ==0)) {
            return NULL;
        }
    }
//Case : 3
    if ((temp->free == 0) || (temp->size < size + sizeof(header))){
            void *freePage = new_empty_page();
            if (!freePage){
                return NULL;
            }
            int current_pagenum = ((void *)temp - (void *)memory)/Page_Size;
            int freepage_num = ((void *)freePage - (void *)memory)/Page_Size;
            int new_pagenum = page_table[current_pagenum].page_num;
                page_table[freepage_num].tid = current_tid;
                page_table[freepage_num].allocate = 1;
                page_table[freepage_num].page_num = new_pagenum + 1;
                mprotect( (void*)(memory + freepage_num * Page_Size), Page_Size, PROT_READ | PROT_WRITE );
            if (temp->free == 0){                
                ((header*)freePage)->next = NULL;
                ((header*)freePage)->free = 1;
                ((header*)freePage)->size = Page_Size-sizeof(header);
                temp->next = ((header*)freePage);
                set_block_data( temp->next, size );
                temp->next->free = 0;                
                result = (char*)temp->next + sizeof(header);
                return (void*)result;
            }
            else {
                temp->size = temp->size+Page_Size;
                if (temp->size > size + sizeof(header)) {
                    set_block_data( temp, size );
                }
                temp->free = 0;
                result = (char*)temp + sizeof(header);
                return (void*)result;
            }
    }
    else{
        return NULL;
    }
}


void mydeallocate(void* space, char* file, int line, int req){
    if(space==NULL) {
        return;
    }
    header *target;
    target = (header *)(space - sizeof(header));
    if(target->free){
        if(target->free==0){
                target->free = 1;}
                else{
        fprintf(stderr,"ERROR: double free.");
        return;
    }
    }
    else{
        fprintf(stderr,"ERROR: double free.");
        return;
    }
}



void* shalloc(size_t size){
 header *temp=(header*)share;
 if(firstshare==0){
    Init_Share_Block(1,share);
    firstshare=1;
 }
void * result;
    while (temp != NULL && temp->next != NULL) {

        if (temp->free == 1 && temp->size >= size){
            break;}        
        temp = temp->next;
    }

//Case:1
    if (temp->free == 1 && temp->size >= size && temp->size <= size + sizeof(header)){

        temp->free = 0;
        result = (char *)temp + sizeof(header);

        return (void*)result;
    }
//Case: 2
    if (temp->free == 1 && temp->size >= (size + sizeof(header))){

        set_block_data(temp, size);
        result = (char *)temp + sizeof(header);

        return (void*)result;
    }
    return NULL;
   }


void multilevelQueue(tcb * main){

    readyQ = myallocate(sizeof(ready_queue),LIBRARYREQ);
    int i;
    for (i = 0; i < 5; i++){
        readyQ->queues[i] = (queue* )myallocate(sizeof(queue),LIBRARYREQ);
        readyQ->queues[i]->head = NULL;
        readyQ->queues[i]->back = NULL;
    readyQ->queues[i]->size = 0;
        //readyQ->queues[i]=NULL;
        //readyQ->queues[i]->level = i;
    }
//multipliers/threads_done for queues
    //readyQ->queues[0]->multiplier = 1;
   // readyQ->queues[0]->threads_done= 0;
    readyQ->queues[0]->multiplier = 10;
    readyQ->queues[0]->threads_done= 0;
    readyQ->queues[1]->multiplier = 8;
    readyQ->queues[1]->threads_done= 0;
    readyQ->queues[2]->multiplier = 6;
    readyQ->queues[2]->threads_done= 0;
    readyQ->queues[3]->multiplier = 4;
    readyQ->queues[3]->threads_done= 0;
    readyQ->queues[4]->multiplier = 2; 
    readyQ->queues[4]->threads_done= 0;  
 
    //node *High_Node = NULL;
//insert main context
    /*readyQ->queues[0]->head=(node*)malloc(sizeof(node));
    readyQ->queues[0]->head->thread=(tcb*)malloc(sizeof(tcb));  
    readyQ->queues[0]->head->thread=current;
    readyQ->queues[0]->head->next=NULL; 
    readyQ->queues[0]->back=readyQ->queues[0]->head;*/
    //find highest node in queue
   // for (i = 0; i < 6; i++){
     //   High_Node = readyQ->queues[i]->head;
      //  if (High_Node != NULL){
       //     break;
       // }
    //}
  
  /*  
    //if High_Node is currently running thread
    if (High_Node->thread->id == current->id){
        if (High_Node->next != NULL){
            High_Node = High_Node->next;
        }
        else {
            for (i = i + 1; i < 6; i++){
                High_Node = readyQ->queues[i]->head;
                if (High_Node != NULL){
                    break;
                }
            }
        }
        if (High_Node == NULL){
            High_Node->thread = current;
        }
    }
   
    int multiplier = readyQ->queues[High_Node->level]->multiplier;
*/
}


//Certain things became one for no reason please go about fixing yield it is busted



void start_itime(){ //later put up a way to tell how much time using multileveled queue
    //setup Itimer
    int x=level+1;
    act.sa_handler = sighandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGPROF, &act, &oact); 
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = 25000*x;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 25000*x;
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = 25000*x;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 25000*x;
    setitimer(ITIMER_PROF, &it, NULL);
}
//Stop Itimer
void stop_itime(){
    setitimer(ITIMER_PROF, 0, 0);
    
}

void sighandler(int sig)
{   //must be fixed this is for changing priority
 if(SYS==0){
        __sync_bool_compare_and_swap(&SYS,untaken,taken);
        stop_itime();
    int j=0;
        tcb *swap_tcb = (tcb *) myallocate(sizeof(tcb),LIBRARYREQ);
        swap_tcb=current;
    int z=swap_tcb->id;
        if(current==NULL){
        current==swap_tcb;
        return;
        }    
    if(readyQ->queues[level]->head==NULL){
        if(level!=4){
            level=level+1;
            enqueue(swap_tcb);
            level=level-1;
        }else{
            j=4;
        }
        readyQ->queues[level]->threads_done=0;
        int a=level+1;
        if(level!=4){
            if(readyQ->queues[level]->head==NULL&&readyQ->queues[a]->head->thread->id==z){
                if(level<3){
                level=level+2;
                }else{
                level=0;
                }
            }
        }
        while(readyQ->queues[level]->head==NULL){
            level=level+1;
            if(level%5==0){
            level=0;
            }
        }
        if(j=4){
            int x=level;
            level=4;    
            enqueue(swap_tcb);
            level=x;
        }
       current=dequeue();
    }else{          //case head is not null meaning yielding with sig does not have a priority switch
        readyQ->queues[level]->threads_done=readyQ->queues[level]->threads_done+1;
        if(level!=4){
            level=level+1;
            enqueue(swap_tcb);
            level=level-1;
        }else{
            enqueue(swap_tcb);
        }   
        if(readyQ->queues[level]->multiplier==readyQ->queues[level]->threads_done){
            readyQ->queues[level]->threads_done=0;
            int a=level+1;
        if(level<4){
            if(readyQ->queues[a]->head->thread->id==z){
                level=level+1;
            }
        }
            level=level+1;
            if(level%5==0){
            level=0;
            }
            while(readyQ->queues[level]->head==NULL){
                level=level+1;
                if(level%5==0){
                level=0;
                }
            }
        }
    current=dequeue();
        }   
        start_itime();
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
        update_protection(current->id,swap_tcb->id);
        swapcontext(&swap_tcb->uc,&current->uc);
        start_itime();
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
    }
        return;    
}


//start Itimer
void my_pthread_exit(void *value_ptr) {
__sync_bool_compare_and_swap(&SYS,untaken,taken);     //do a seperate case for main context in that the whole program will end.
stop_itime();
//when kill list is made to put for reapable threads is created, then edit code
if(current->id==0){
exit(0);
}
if(kilhim==NULL){
kilhim= (kilist *) myallocate(sizeof(kilist),LIBRARYREQ);
node *temp = (node *) myallocate(sizeof(node),LIBRARYREQ);
temp->thread=current;
temp->next=NULL;
//int * a=(int*)malloc(sizeof(int));
//a=(int*)value_ptr;
temp->thread->return_value=value_ptr;
kilhim->begin=temp;
}else{
node *temp = (node *) myallocate(sizeof(node),LIBRARYREQ);
temp->thread=current;
temp->next=NULL;
//int * a=(int*)malloc(sizeof(int));
//a=(int*)value_ptr;
temp->thread->return_value=value_ptr;
temp->next=kilhim->begin;
kilhim->begin=temp;
}           //do it here
tcb *n_tcb = (tcb *) myallocate(sizeof(tcb),LIBRARYREQ);
n_tcb=current;
    // printf("yea\n" );
if(readyQ->queues[level]->head==NULL){  

    readyQ->queues[level]->threads_done=0;
    while(readyQ->queues[level]->head==NULL){
        level=level+1;
        if(level%5==0){
        level=0;
        }
    }
   current=dequeue();
}else{
    readyQ->queues[level]->threads_done=readyQ->queues[level]->threads_done+1;    
    if(readyQ->queues[level]->multiplier==readyQ->queues[level]->threads_done){ //Checked logic it is for, when exiting a queue reaches priority limit due to the thread being elminiated counting as a thread done. This leads to switching to the next. 
        readyQ->queues[level]->threads_done=0;
        level=level+1;
        if(level%5==0){
        level=0;
        }
        while(readyQ->queues[level]->head==NULL){
            level=level+1;
            if(level%5==0){
                level=0;
            }
        }
    }
tcb *nswap_tcb = (tcb *) myallocate(sizeof(tcb),LIBRARYREQ);// might want to include kill
nswap_tcb=dequeue(); 
current=nswap_tcb;
}
start_itime();
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
update_protection(current->id,n_tcb->id);
swapcontext(&n_tcb->uc,&current->uc);
__sync_bool_compare_and_swap(&SYS,untaken,taken);
stop_itime();
while(1){
__sync_bool_compare_and_swap(&SYS,untaken,taken);
stop_itime();
while(readyQ->queues[level]->head==NULL){
            level=level+1;
            if(level%5==0){
                level=0;
            }
        }
n_tcb=current;
tcb *nswap_tcb = (tcb *)myallocate(sizeof(tcb),LIBRARYREQ);// might want to include kill
nswap_tcb=dequeue(); 
current=nswap_tcb;
start_itime();
__sync_bool_compare_and_swap(&SYS,taken,untaken);
update_protection(current->id,n_tcb->id);
swapcontext(&n_tcb->uc,&current->uc);

}
};


//fix join and yield, they are broken.
/* create a new thread */
int my_pthread_create(my_pthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg) {
//start Itimer
            //replace with dequeue regarding getting thread, orig is not meant to be permanent.
//get context
    __sync_bool_compare_and_swap(&SYS,untaken,taken);
            
    if(current==NULL){

    if(getcontext(&main_context)!=0){
        printf("error\n");
        exit(EXIT_FAILURE);
    }
      //  run_q.head = run_q.back = NULL;
      //  run_q.size = 0;
    //get main context
        tcb *main_tcb = (tcb *) myallocate(sizeof(tcb),LIBRARYREQ);
            main_tcb->id=id;//(IMPORTANT!!! main ID??????)
        id=id+1;
        if(getcontext(&main_tcb->uc)!=0){
        printf("%s\n",strerror(EINVAL));
        exit(EXIT_FAILURE);
        }

    //set main as current thread context
        current = main_tcb;
    //Init multilevel
        multilevelQueue(current);
    }else{
    stop_itime();
    }

    if(getcontext(&thr)!=0){
    printf("%s\n",strerror(EINVAL));
    exit(EXIT_FAILURE);
    }
    if(getcontext(&op_context)!=0){
    printf("%s\n",strerror(EINVAL));
    exit(EXIT_FAILURE);
    }   
    op_context.uc_link=0;   //later change instead of 0 some other type of link to exit
    op_context.uc_stack.ss_sp=(int*)myallocate(stac,LIBRARYREQ);
    op_context.uc_stack.ss_size=stac;
    op_context.uc_stack.ss_flags=0;
    if(arg==NULL){
    makecontext(&op_context,(void*)(*my_pthread_exit),1,NULL);
    }else{
    int* zam=(int*)&arg;
    makecontext(&op_context,(void*)(*my_pthread_exit),1,zam);
    }   
    thr.uc_link=&op_context;    // some other type of link to exit so no problem is encountered
    thr.uc_stack.ss_sp=(int*)myallocate(stac,LIBRARYREQ);
    thr.uc_stack.ss_size=stac;
    thr.uc_stack.ss_flags=0;    
    if(arg==NULL){
    makecontext(&thr,(void*)(*function),1,NULL);
    }else{
    int* tam=(int*)&arg;
    makecontext(&thr,(void*)(*function),1,tam);
    }
    tcb *new_tcb = (tcb *) myallocate(sizeof(tcb),LIBRARYREQ);
    new_tcb->id=id;
    *thread=id;
    id=id+1;        //all thar is occuring is a context switch with our new thread.
    new_tcb->uc=thr;
    tcb *temp_tcb = (tcb *) myallocate(sizeof(tcb),LIBRARYREQ);
    temp_tcb=current;
    int d=level;
    level=0;
    enqueue(new_tcb);
    level=d;   


  /*  enqueue(current);
    //current=new_tcb; //now it is that our new tcb is going to run
    readyQ->queues[level]->threads_done=readyQ->queues[level]->threads_done+1;    
    if(readyQ->queues[level]->multiplier==readyQ->queues[level]->threads_done){
        readyQ->queues[level]->threads_done=0;
        level=level+1;
        if(level%5==0){
        level=0;
        }
        while(readyQ->queues[level]->head==NULL){
        level=level+1;
            if(level%5==0){
            level=0;
            }
        }
    }
    start_itime();
    SYS=0;
    swapcontext(&temp_tcb->uc,&current->uc);*/

    my_pthread_yield(); 
    start_itime();
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
    return 0;
};



//add a node to the rear of queue
int enqueue (tcb *thread) {
    node *temp = (node *) myallocate(sizeof(node),LIBRARYREQ);
    if (temp == NULL) {
    return 0;
        }
    temp->thread= thread;
    temp->next = NULL;
    if (readyQ->queues[level]->back) {
       readyQ->queues[level]->back->next = temp;
        readyQ->queues[level]->back= temp;
        }
    else
        {
        readyQ->queues[level]->back= temp;
        readyQ->queues[level]->head = temp;
        }
    readyQ->queues[level]->size++;
 //   if(readyQ->queues[0]->size>1){printf("%dyes its here:%d\n", readyQ->queues[0]->head->thread->id,readyQ->queues[0]->head->next->thread->id);}
        return 1;
    
}





//Initial thead queue by managing main context, this must be eliminated effective immediately as we cannot have an init later on.
/*void thread_init(){
    //initialized queue
    run_q.head = run_q.back = NULL;
    run_q.size = 0;
    //get main context
    tcb *main_tcb = (tcb *) malloc(sizeof(tcb));
    main_tcb->id=id;//(IMPORTANT!!! main ID??????)
    id=id+1;
    if(getcontext(&main_tcb->uc)!=0){
    printf("error\n");
    exit(EXIT_FAILURE);
    }
    //set main as current thread context
    current = main_tcb;
    return;
}*/

int my_pthread_yield() {
    __sync_bool_compare_and_swap(&SYS,untaken,taken);
    stop_itime();
    tcb* temp_tcb=NULL;
    //worry about null case next
    
    int j=level;
    if(readyQ->queues[level]->head==NULL){
    temp_tcb=current;
    enqueue(current);
    readyQ->queues[level]->threads_done=0;
        level=level+1;
        if(level%5==0){
        level=0;
        }
    while(readyQ->queues[level]->head==NULL){
        level=level+1;
        if(level%5==0){
        level=0;
        }
    }
    current=dequeue();
     }

    else{
        readyQ->queues[level]->threads_done=readyQ->queues[level]->threads_done+1;
        temp_tcb=current;
        enqueue(current);
        //current=dequeue();    
        if(readyQ->queues[level]->multiplier==readyQ->queues[level]->threads_done){
            readyQ->queues[level]->threads_done=0;
            level=level+1;
            if(level%5==0){
            level=0;
            }
            while(readyQ->queues[level]->head==NULL){ //if next is null
                level=level+1;
                if(level%5==0){
                level=0;
                }
            }
        }
    current=dequeue(); 
}
    start_itime();
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
    swapcontext(&temp_tcb->uc,&current->uc);
    start_itime();
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
    return 0;
};

int my_pthread_join(my_pthread_t thread, void **value_ptr) {
    
    __sync_bool_compare_and_swap(&SYS,untaken,taken);
    stop_itime();
    if(readyQ==NULL){
    printf("%s\n",strerror(EINVAL));
    exit(EXIT_FAILURE);
    }
    while(kilhim==NULL){
    my_pthread_yield();
    __sync_bool_compare_and_swap(&SYS,untaken,taken);
    stop_itime();
    }
    node* look=kilhim->begin;
    int check=-1;
    int x=(int)thread;
    while(check!=x){
        if(look!=NULL){
            while(look->next!=NULL){    
                    if(look->thread->id==x||look->next->thread->id==x){     
                    break;
                 }
        look=look->next;
            }
            if(look->next!=NULL){
          if(look->next->thread->id==x){

                break;
        }
            }
            if(look->thread->id==x){

                break;
            }
        
        }
        my_pthread_yield();
        __sync_bool_compare_and_swap(&SYS,untaken,taken);
        stop_itime();
        look=kilhim->begin;     
    }
    if(look->thread->id==x){
    kilhim->begin=look->next;
    if(value_ptr!=NULL){
    *value_ptr=look->thread->return_value;
    //*value_ptr=j;
//value_ptr=j;
    }
    free(look);
    }else{
    node* temp=look->next;
    if(value_ptr!=NULL){
       *value_ptr=temp->thread->return_value;
   // *value_ptr=j;
  //double* j=(double*)value_ptr;
//j=look->thread->return_value;
//*value_ptr=look->thread->return_value;
//value_ptr=j;
    }
    look->next=look->next->next;
    free(temp);
    }
    my_pthread_yield();
    start_itime();
            __sync_bool_compare_and_swap(&SYS,taken,untaken);

    return 0;
};


void waitQinit(){
    waitQueues *waitQ=myallocate(sizeof(waitQueues),LIBRARYREQ);
}


void waitQadd(waitQueues * new_wait_Q){
    struct waitQueues * temp;
    if(waitQ){
    }
    else{
        waitQ=new_wait_Q;
        return;
    }
    if(waitQ->next){
    temp=waitQ->next;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next=new_wait_Q;
    return;}
    else{
        waitQ->next=new_wait_Q;
        return;
    }
}


int my_pthread_mutex_init(my_pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr){
    //initialize mutex
    __sync_bool_compare_and_swap(&SYS,untaken,taken);
    stop_itime();
    if(mutex==NULL){
    printf("%s\n",strerror(EINVAL));
    exit(EXIT_FAILURE);
    }
    if(mutex->lock==1){
    printf("%s\n",strerror(EBUSY));
    exit(EXIT_FAILURE);
    }
    mutex->mid=mutexid;
    mutex->lock=0;
    //initialize waitQ
    if(waitQ==NULL){
        waitQinit();
    } 
    
    //Add new Q into wait Q
    waitQueues * newQ=myallocate(sizeof(waitQueues),LIBRARYREQ);
    if(newQ==NULL){
    printf("%s\n",strerror(ENOMEM));
    exit(EXIT_FAILURE);
    }
    newQ->head=NULL;
    newQ->id=mutex->mid; 
    mutexid=mutexid+1;
    waitQadd(newQ);
   // my_pthread_yield();   You get extra time should we allow that??? Probably.
   if(readyQ!=NULL){
    my_pthread_yield(); 
    start_itime();
   }    
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
    return 0;
}


int my_pthread_mutex_lock (my_pthread_mutex_t *mutex){
  // printf("%d:%d\n", current->id,readyQ->queues[0]->head->thread->id);
    __sync_bool_compare_and_swap(&SYS,untaken,taken);
    stop_itime();
    if(mutex==NULL){
    printf("%s\n",strerror(EINVAL));
    exit(EXIT_FAILURE);
    }
    int d=0;
    my_pthread_mutex_search(mutex);
    waitQueues * wait=current_wait_Q;
    if(wait==NULL){
        printf("%s\n",strerror(EINVAL));
        exit(EXIT_FAILURE);
    }
    if (mutex->lock == 1) {
        stop_itime();
        node * temp;

//Putting current thread into wait queue
//storing into current wait Q
    //if current wait Q is empty
    node * target =(node*)myallocate(sizeof(node),LIBRARYREQ);
    target->thread=current;
    target->next=NULL;
    target->level=level;
    if(wait->head==NULL){
        wait->head=target;
    }
    //traverse the current wait Q to enqueue new node into the rear of wait Q
    else{
    temp=wait->head;
    while(temp->next!=NULL){
        temp=temp->next;
        }
        //Push new node
    temp->next=target;
    }

//Need context switching
    //............
readyQ->queues[level]->threads_done=readyQ->queues[level]->threads_done+1;
tcb* temporary=NULL;
temporary=current;
       if(readyQ->queues[level]->multiplier==readyQ->queues[level]->threads_done){
            readyQ->queues[level]->threads_done=0;
            level=level+1;
            if(level%5==0){
            level=0;
            }
            while(readyQ->queues[level]->head==NULL){ //if next is null
                level=level+1;
                if(level%5==0){
                level=0;
                }
            }
        }
    while(readyQ->queues[level]->head==NULL){ //if next is null
                level=level+1;
                if(level%5==0){
                level=0;
                }
            }
    current=dequeue();
    d=1;
        start_itime();
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
    swapcontext(&temporary->uc,&current->uc);
    __sync_bool_compare_and_swap(&SYS,untaken,taken);
    stop_itime();
    }
    mutex->lock = 1;
    if(readyQ!=NULL&&d==0){
    my_pthread_yield();
    start_itime();  
    }
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
    return 0;
}



int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex) { //what are you doing
    __sync_bool_compare_and_swap(&SYS,untaken,taken);
    stop_itime();
    if(mutex==NULL){
    printf("%s\n",strerror(EINVAL));
    exit(EXIT_FAILURE);
    }
    my_pthread_mutex_search(mutex);
    if(current_wait_Q==NULL){
    printf("%s\n",strerror(EINVAL));
    exit(EXIT_FAILURE);
    }
    if(current_wait_Q->head!=NULL){
     
    
    node *temp=current_wait_Q->head;
    if(temp->next==NULL){
        //putting it back
        int d=level;
        level=temp->level;
        enqueue(temp->thread);
        level=d;
        //pop
        current_wait_Q->head=NULL;
    }
    else{
        while(temp->next->next){
            temp=temp->next;
        }
        //putting it back
        int d=level;
        level=temp->next->level;
        enqueue(temp->next->thread);
        level=d;
        //pop
        temp->next=temp->next->next;

    }}
    mutex->lock = 0;
    if(readyQ!=NULL){
    my_pthread_yield();
    start_itime();
    }
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
    return 0;
}





int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex){
    __sync_bool_compare_and_swap(&SYS,untaken,taken);
    stop_itime();
    if(mutex==NULL){
    printf("%s\n",strerror(EINVAL));
    exit(EXIT_FAILURE);
    }
    my_pthread_mutex_search(mutex);
    waitQueues * target=current_wait_Q;
    if(target==NULL){
    printf("%s\n",strerror(EINVAL));
    exit(EXIT_FAILURE);
    }   
    if(target->head||mutex->lock==1){
        printf("%s\n",strerror(EBUSY));
        exit(EXIT_FAILURE);
    }
    else{
        free(target->head);
    }
    my_pthread_yield();
    start_itime();
        __sync_bool_compare_and_swap(&SYS,taken,untaken);
    return 0;
}



tcb* dequeue (queue q){
tcb *ret = NULL;
if (readyQ->queues[level]->head==NULL){
ret = NULL;
}
else if (readyQ->queues[level]->head->next == NULL){
    ret = readyQ->queues[level]->head->thread;
    readyQ->queues[level]->head = NULL;
    readyQ->queues[level]->back = NULL;
    readyQ->queues[level]->size--;
}
else {
node *temp = (node *) myallocate(sizeof(node),LIBRARYREQ);
    temp = readyQ->queues[level]->head;
    readyQ->queues[level]->head = readyQ->queues[level]->head->next;
    ret = temp->thread;
                readyQ->queues[level]->size--;
    }
return ret;
}
void  my_pthread_mutex_search(my_pthread_mutex_t *mutex){
    int lid=mutex->mid;
    waitQueues * temp=waitQ;
    while(temp->next){
        if(temp->id==lid){
            current_wait_Q=temp;
            return;
    }
    temp=temp->next;
  }
    if(temp!=NULL){
    if(temp->id==lid){
            current_wait_Q=temp;
            return;
        }
    }
  current_wait_Q=NULL;
  return;
}




