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

        //Starting the time counters
        clock_gettime(CLOCK_REALTIME, &start_time1);
        clock_gettime(CLOCK_REALTIME, &start_time2);

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

    //Print statements
    printf("Total profit: %d euros\n",total_profit);
    printf("Total plain pizzas sold: %d\n",sum_of_plainpizzas);
    printf("Total special pizzas sold: %d\n",sum_of_specialpizzas);
    printf("Successfull orders: %d\n",succesfull_orders);
    printf("Unsuccessfull orders: %d\n",unsuccesfull_orders);
    printf("Average customer service time:  %d \n",order_time_sum/succesfull_orders);
    printf("Max customer service  time:  %d \n",order_max_time);
    printf("Average cold time:  %d \n",cold_time_sum/succesfull_orders);
    printf("Max cold time:  %d \n",cold_max_time);

    //Freing memory
    pthread_cancel(threads[Ncust]);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}