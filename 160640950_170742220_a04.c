#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>

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

time_t programClock;//this line represents the clock for the program

typedef struct thread 
{
        char tid[4];
        unsigned int startTime;
        int state;
        pthread_t handle;
        int retVal;
} Thread;



void Run();
int readFile(char* fileName);//this function reads the file content and builds array of threads
void rq(char* c);
void rl(char* c);
void Ast();
int safety();

char rff[5][8];//resources from file
int r[5][4];//max resources
int ar[5][4];// allocated
int nr[5][4];//needed
int numOfR[4];
int ex = 0;

int main(int argc, char *argv[])
{
        if(argc<2)
        {
                printf("Input file name could not be found...exiting with error code -1\n");
                return -1;
        }
        char *p;
        long conv = strtol(argv[1], &p, 10);

        numOfR[0] = conv;
        conv = strtol(argv[2], &p, 10);
        numOfR[1] = conv;
        conv = strtol(argv[3], &p, 10);
        numOfR[2] = conv;
        conv = strtol(argv[4], &p, 10);
        numOfR[3] = conv;

    

        Thread* threads = NULL;
        //argv[1]
        int threadCount = readFile("sample4_in.txt");
        int numCus;
        printf("Number of Customers: ");
        scanf(" %d", &numCus);
//      printf("\n");
        printf("Presently Available resources: %d %d %d %d\n",numOfR[0],numOfR[1],numOfR[2],numOfR[3]);
        printf("Maximum resources from file: \n");
        int x = 0;
        while(x<5){
                printf("%s\n", rff[x]);
                x++;
        }
//      startClock();

        char cmd[16];
        char zero[2] = "0";
        char c[3];

        printf("Enter Command (Press 0 to exit): ");
        scanf(" %[^\n]", cmd);

        while(strcmp(cmd,zero) != 0 && ex == 0){
                strncpy(c,cmd,3);
//              printf("%s\n",c);

                if(strcmp(c,"RQ ") == 0){
                        rq(cmd);
                }
                else if(strcmp(c,"RL ") == 0){
                        rl(cmd);
                }
                else if(strcmp(c,"*") == 0){
                        Ast();
                }
                else if(strcmp(c,"Run") == 0){
                        Run();
                }
                else{
                        printf("Invalid entry\n");
                }

                if(ex == 0){
                        printf("Enter Command (Press 0 to exit): ");
                        scanf(" %[^\n]", cmd);
                }
        }

        return 0;
}

int readFile(char* fileName)//do not modify this method
{
        FILE *in = fopen(fileName, "r");
        if(!in)
        {
                printf("Child A: Error in opening input file...exiting with error code -1\n");
                return -1;
        }

        struct stat st;
        fstat(fileno(in), &st);
        char* fileContent = (char*)malloc(((int)st.st_size+1)* sizeof(char));
        fileContent[0]='\0';    
        while(!feof(in))
        {
                char line[100];
                if(fgets(line,100,in)!=NULL)
                {
                        strncat(fileContent,line,strlen(line));
                }
        }
        fclose(in);

        char* command = NULL;
        int threadCount = 0;
        char* fileCopy = (char*)malloc((strlen(fileContent)+1)*sizeof(char));
        strcpy(fileCopy,fileContent);
        command = strtok(fileCopy,"\r\n");
        while(command!=NULL)
        {
                threadCount++;
                command = strtok(NULL,"\r\n");
        }
//      *threads = (Thread*) malloc(sizeof(Thread)*threadCount);

        char* lines[threadCount];
        command = NULL;
        int i=0;
        command = strtok(fileContent,"\r\n");
        while(command!=NULL)
        {
               lines[i] = malloc(sizeof(command)*sizeof(char));
                strcpy(lines[i],command);
                i++;
                command = strtok(NULL,"\r\n");
        }

        char rffClone[5][8];//resources from file
        for(int k=0; k<threadCount; k++)
        {
                char* token = NULL;
                int j = 0;
                token =  strtok(lines[k],";");
                while(token!=NULL)
                {

//                      printf("%s\n",token);

                        strcpy(rff[k], token);
                        strcpy(rffClone[k], token);
                        ar[k][j] =  0;
                        j++;
                        token = strtok(NULL," ");

                }
        }

        char * token;
        // loops through the string to extract all other tokens
        for(int j =0;j<5;j++){
                token = strtok(rffClone[j], ",");
                i = 0;
                while( token != NULL ) {
                        //printf( " %s\n", token ); //prints each token
                        r[j][i] = atoi(token);
                        nr[j][i] = atoi(token);
                        token = strtok(NULL, ",");
                         i++;
                }
        }


        return threadCount;
}


void Run()
{

        int safe = safety();


        if(safe == 1){
                printf("Safe Sequence is: <0 1 2 3 4>");
        for(int i  = 0;i<5;i++){

                printf("\n");
                printf("\n");
                printf("--> Customer/Thread %d\n",i);
                printf("--> Customer/Thread %d\n",i);
                printf("        Allocated resources:    %d %d %d %d\n", ar[i][0], ar[i][1], ar[i][2], ar[i][3]);
                printf("        Needed:    %d %d %d %d\n", nr[i][0], nr[i][1], nr[i][2], nr[i][3]);
                printf("        Available:    %d %d %d %d\n", numOfR[0], numOfR[1], numOfR[2], numOfR[3]);
                printf("        Thread has started\n");
                printf("        Thread has finished\n");
                printf("        Thread is releasing resources\n");
                for(int j =0;j<4;j++){
                        numOfR[j] = numOfR[j] +ar[i][j];
                }
                printf("        New Available:   %d %d %d %d\n", numOfR[0], numOfR[1], numOfR[2], numOfR[3]);
                for(int j =0;j<4;j++){
                        nr[i][j] = 0;
                        ar[i][j] = r[i][j];
                }
                ex = 1;
        }
        }
        else{
                printf("Not safe\n");
        }
}

void rq(char* c){
        char  res[6][3];
        char * token = strtok(c, " ");
        int i = 0;
        
        while( token != NULL ) {
                //printf( " %s\n", token ); //printing each token
                strcpy(res[i],token);
                token = strtok(NULL, " ");
                i++;
        }
        int cus  = atoi(res[1]);
        int x = 2;
        int re[4];
        //printf("%d\n",cus);
        while(x<6){
                re[x-2] = atoi(res[x]);
//              printf("%d\n",re[x]);
                x++;
        }

        int s = 0;
        if(re[0] < numOfR[0]){
                if(re[1] < numOfR[1]){
                        if(re[2] < numOfR[2]){
                                if(re[3] < numOfR[3]){
                                        s  = 1;
                                }
                        }
                }
        }

        if(s==1){
                for(int i = 0;i<4;i++){
                        numOfR[i] = numOfR[i] - re[i];
                }

                for(int i = 0;i<4;i++){
                        ar[cus][i] = ar[cus][i] + re[i];
                }

                for(int i = 0;i<4;i++){
                        nr[cus][i] = nr[cus][i] - re[i];
                }
                printf("successful 0\n");
        }
        else{
                printf("unsuccessful -1\n");
        }

}

void rl(char* c){
        char  res[6][3];
        char * token = strtok(c, " ");
        int i = 0;
        // loops through the string to extract all other tokens
        while( token != NULL ) {
                //printf( " %s\n", token ); //printing each token
                strcpy(res[i],token);
                token = strtok(NULL, " ");
                i++;
        }
        int cus  = atoi(res[1]);
        int x = 2;
        int re[4];
        //printf("%d\n",cus);
        while(x<6){
                re[x-2] = atoi(res[x]);
//              printf("%d\n",re[x]);
                x++;
        }

        int s = 0;
        if(re[0] <= ar[cus][0]){
                if(re[1] <= ar[cus][1]){
                        if(re[2] <= ar[cus][2]){
                                if(re[3] <= ar[cus][3]){
                                        s  = 1;
                                }
                        }
                }
        }
        
        if(s==1){
        for(int i = 0;i<4;i++){
                numOfR[i] = numOfR[i] + re[i];
        }

        for(int i = 0;i<4;i++){
                ar[cus][i] = ar[cus][i] - re[i];
        }

        for(int i = 0;i<4;i++){
               nr[cus][i] = nr[cus][i] + re[i];
        }
        printf("successful 0\n");
        }
        else{
                printf("unsuccessful -1\n");
        }
}

void Ast(){
        printf("Presently Available resources: %d %d %d %d\n",numOfR[0],numOfR[1],numOfR[2],numOfR[3]);

        printf("Maximum resources: \n");
        for(int i = 0; i<5; i++){
                for(int j = 0;j<4;j++){
                        printf("%d ",r[i][j] );
                }
                printf("\n");
        }

        printf("Allocated resources: \n");
        for(int i = 0; i<5; i++){
                for(int j = 0;j<4;j++){
                        printf("%d ",ar[i][j] );
                }
                printf("\n");
        }

        printf("Required resources: \n");
        for(int i = 0; i<5; i++){
                for(int j = 0;j<4;j++){
                        printf("%d ",nr[i][j] );
                }
                printf("\n");
        }
}

int safety(){

        int i = 0;
        int j = 0;
        int safe = 1;
        int num;
        while(i < 5  && safe ==1){
                while(j < 4  && safe ==1){
                        num = 0;
                        for(int a = i-1; a >0; a--){
                                num = num +ar[i-a][j];
                        }
                        if(nr[i][j] > (numOfR[j]+num)){
                                safe = 0;
                        }
                        j++;
                }
                i++;
        }

        return safe;
}