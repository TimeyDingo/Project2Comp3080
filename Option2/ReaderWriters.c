//Benjamin Evangelista, Wes Cole, Kelvin Kee, Riley Stevens
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#define RANGE 1000000000
#define BASE 500000000
int rICrange = RANGE;
int rICbase = BASE;
int rOOCrange = RANGE;
int rOOCbase = BASE;
int wICrange = RANGE;
int wICbase = BASE;
int wOOCrange = RANGE;
int wOOCbase = BASE;
int keepgoing = 1;
int totalReads = 0;
int totalWrites = 0;


sem_t mutex; // for read count?
sem_t countLock; 
sem_t rw_mutex; // controls access to shared resource
int ReadCount = 0;
//! The global area must include semaphore declarations and
//! declarations of any state variables (reader counts,
//! total number of readers and writers).
//! Use this function to sleep within the threads

void threadSleep(int range, int base)
{
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = (rand() % range) + base;
    nanosleep(&t, 0);
}

void *readers(void *args)
{
    int id = *((int *)args);
    threadSleep(rOOCrange, rOOCbase);
    while (keepgoing)
    {

        sem_wait(&mutex);
        ReadCount++;
        if (ReadCount == 1){
            sem_wait(&rw_mutex);
        } 
        sem_post(&mutex);
        sem_wait(&countLock);
        totalReads++;
        sem_post(&countLock);
        
        printf("Reader %d starting to read\n", id);
        threadSleep(rICrange, rICbase);
        printf("Reader %d finishing reading\n", id);
        
        sem_wait(&mutex);
        ReadCount--;
        if (ReadCount == 0){
            sem_post(&rw_mutex);
        }
        sem_post(&mutex);
    }
    printf("Reader %d quitting\n", id);
    return NULL;
}

void *writers(void *args)
{
    int id = *((int *)args);
    threadSleep(wOOCrange, wOOCbase);
    while (keepgoing)
    {
        //! Add code for each writer to enter
        //! the writing area.
        sem_wait(&rw_mutex);
        sem_wait(&countLock);
        totalWrites++;
        sem_post(&countLock);   
        printf("Writer %d starting to write\n", id);
        threadSleep(wICrange, wICbase);
        printf("Writer %d finishing writing\n", id);

        sem_post(&rw_mutex); //! Add code for each writer to leave //! the writing area.
        
        threadSleep(wOOCrange, wOOCbase);
    }
    printf("Writer %d quitting\n", id);
    return NULL;
}

int main(int argc, char **argv)
{
    int numRThreads = 0;
    int numWThreads = 0;
    if (argc == 11)
    {
        rICrange = atoi(argv[1]);
        rICbase = atoi(argv[2]);
        rOOCrange = atoi(argv[3]);
        rOOCbase = atoi(argv[4]);
        wICrange = atoi(argv[5]);
        wICbase = atoi(argv[6]);
        wOOCrange = atoi(argv[7]);
        wOOCbase = atoi(argv[8]);
        numRThreads = atoi(argv[9]);
        numWThreads = atoi(argv[10]);
    }
    else
    {
        printf("Usage: %s <reader in critical section sleep range> <reader in critical section sleep base> \n\t <reader out of critical section sleep range> <reader out of critical section sleep base> \n\t <writer in critical section sleep range> <writer in critical section sleep base> \n\t <writer out of critical section sleep range> <writer out of critical section sleep base> \n\t <number of readers> <number of writers>\n", argv[0]);
        exit(-1);
    }
    //! Add declarations for pthread arrays, one for reader threads and
    //! one for writer threads.
    pthread_t readerArr[numRThreads];
    pthread_t writerArr[numWThreads];
    //! Add declarations for arrays for reader and writer thread identities. As in the
    //! dining philosopher problem, arrays of int are used.
    int readersID[numRThreads];
    int writersID[numWThreads];
    //! Add code to initialize the binary semaphores used by the readers and writers.
    sem_init(&mutex, 0, 1);
    sem_init(&countLock, 0, 1);
    sem_init(&rw_mutex, 0, 1);
    //! Add a for loop to create numRThread reader threads.
    for (int i = 0; i < numRThreads; i++) {
        readersID[i] = i;
        pthread_create(&readerArr[i], NULL, readers, &readersID[i]);
    }
    //! Add a for loop to create numWThread writer threads.
    for (int i = 0; i < numWThreads; i++) {
        writersID[i] = i;
        pthread_create(&writerArr[i], NULL, writers, &writersID[i]);
    }
    //! These statements wait for the user to type a character and press
    //! the Enter key. Then, keepgoing will be set to 0, which will cause
    //! the reader and writer threads to quit.
    char buf[256];
    scanf("%s", buf);
    keepgoing = 0;
    
    //! Add two for loops using pthread_join in order to wait for the reader
    for (int i = 0; i < numRThreads; i++) {
        pthread_join(readerArr[i],NULL);
    }
    for (int i = 0; i < numWThreads; i++) {
        pthread_join(writerArr[i],NULL);
    }
    //! and writer threads to quit.
    printf("Total number of reads: %d\nTotal number of writes: %d\n", totalReads, totalWrites);

    return 0;
}
