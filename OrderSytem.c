#include <stdio.h>
#include <pthread.h>
#include "p3210218-p3210272-pizzeria.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


//Gives the seed,imported from the user, for the random numbers genarators
unsigned int* random_number_seed(void) {
    return &random1;
}


//Checks for order failure or submission
void* order(void* x) {

    pthread_mutex_lock(&lock);
    int id_thread = *(int*)x;
    printf("H paraggelia %d xekinhse\n", id_thread);

    //vriskoume ton tuxaio arithmo pitswn ths paraggelias
    int random_number_pizza = rand_r(random_number_seed()) % Norderhigh + Norderlow;

    int* pizzas = (int*)malloc(random_number_pizza * sizeof(int));
    //analoga me to poses pitses thelei,briskoume poies einai aples kai poies special
    for (int i = 0; i < random_number_pizza; i++) {

        float possibility_plain = (float)rand_r(random_number_seed()) / RAND_MAX;
        if (possibility_plain <= 0.6) {
            pizzas[i] = 0;//plain
        }
        else {
            pizzas[i] = 1;//special
        }


    }

    pthread_mutex_unlock(&lock);
    free(pizzas);

    return 0;
}


int main(int argc, char* argv[]) {

    int resources;
    //Customers,random and thread funcs initialization
    Ncust = atoi(argv[1]);
    random1 = atoi(argv[2]);
    pthread_t threads[Ncust];
    pthread_mutex_init(&lock, NULL);
    
    int id[Ncust];


    for (int i = 0; i < Ncust; i++) {

        id[i] = i + 1;//Order number 
        printf("Created thread number:  %d\n", i + 1);

        resources = pthread_create(&threads[i], NULL, order, &id[i]);//Thread creation for each order 

        //Each thread is created in a period between [Torderlow,Toorderhigh]
        int random_sleep = rand_r(&random1) % Torderhigh + Torderlow;
        sleep(random_sleep);

    }

    for (int i = 0; i < Ncust; i++) {
        pthread_join(threads[i], NULL);
    }


    //Freing memory
    pthread_cancel(threads[Ncust]);
    pthread_mutex_destroy(&lock);
    return 0;
}