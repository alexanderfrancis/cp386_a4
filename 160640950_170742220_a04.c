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
    int available[10];
    int max[10][10];
    int allocation[10][10];
    int need[10][10];
} resources;

void init(resources* r, int n, int m, char* args) {
    
    return;
}

int main(int argc, char *argv[]) {
    FILE *f = fopen("sample4_in.txt", "r");

    //find number of threads from file
    int n = 0;
    char s[100];
    while (fgets(s, 100, f) != NULL) {
        n++;
    }

    //number of resource types
    int m = argc - 1;

    //initialize resources
    resources* r =  (resources*) malloc(sizeof(resources));
    init(r, n, m, argv);

    return 0;
}