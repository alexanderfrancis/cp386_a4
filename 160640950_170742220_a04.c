#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <semaphore.h>

/*
    CP386 Assignment 4: Multithreaded Banker's Algorithm

    Authors:
        Alexander Francis
        fran0950@mylaurier.ca
        160640950

        Prit Patel
        pate2220@mylaurier.ca
        170742220

    June 30, 2021

    Repository URL:
        https://github.com/alexanderfrancis/cp386_a4.git
    
    GitHub Usernames:
        alexanderfrancis
        pritpatel27

*/

typedef struct resources
{
    int *available;    //[numtypes]
    int *max;          //[numthreads][numtypes] 
    int *allocation;   //[numthreads][numtypes] 
    int *need;         //[numthreads][numtypes] 
} resources;

//initialize all resource structures
void init(resources* r, int numthreads, int numtypes, char *args[]) {
    
    //available
    r->available = malloc(sizeof(int) * numtypes);
    for (int i = 1; i < numtypes + 1; i++) {
        char *p;
        int v = strtol(args[i], &p, 10);
        r->available[i - 1] = v;
    }

    //max

    //allocation

    //need

    return;
}

int main(int argc, char *argv[]) {
    FILE *f = fopen("sample4_in.txt", "r");

    //find number of threads from file
    int numthreads = 0;
    char s[100];
    while (fgets(s, 100, f) != NULL) {
        numthreads++;
    }

    //number of resource types
    int numtypes = argc - 1;

    //initialize resources
    resources* r = malloc(sizeof(resources));
    init(r, numthreads, numtypes, argv);

    return 0;
}