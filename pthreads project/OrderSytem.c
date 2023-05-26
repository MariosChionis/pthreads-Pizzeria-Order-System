#include <stdio.h>
#include <pthread.h>
#include "GlobalVariables.h"
#include "Functions.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


int main(int argc, char* argv[]) {

    int resources;
    //Customers,random and thread funcs initialization
    Ncust = atoi(argv[1]);
    random1 = atoi(argv[2]);
    pthread_t threads[Ncust];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond,NULL);
    int id[Ncust];


    for (int i = 0; i < Ncust; i++) {

        id[i] = i + 1;//Order number 
        printf("Created thread number:  %d\n", i + 1);

        resources = pthread_create(&threads[i], NULL, order, &id[i]);//Thread creation for each order 

        //Each thread is created in a period between [Torderlow,Toorderhigh]
        int random_sleep = rand_r(&random1) % Torderhigh + Torderlow;
        sleep(random_sleep);

        //Freeing memory of each order that has been canceled
        if(cancel_id!=0){
            pthread_cancel(threads[cancel_id-1]);
            printf("Cancelled order %d \n",cancel_id);
            cancel_id=0;
            
        }

    }

    for (int i = 0; i < Ncust; i++) {
        pthread_join(threads[i], NULL);
    }


    //Freing memory
    pthread_cancel(threads[Ncust]);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}