#include <stdio.h>
#include <pthread.h>
#include "p3210218-p3210272-pizzeria.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//Holds the id of an order the has been canceled
void cancelation_id(int id){
    cancel_id=id;
}

//Calculates the total profit,the number of plain pizzas as well as the number of special pizzas
void sales(int pizzas[],int pizza){

    for (int i=0; i<pizza; i++) {
        if(pizzas[i]==0){
            sum_of_plainpizzas++;
            total_profit +=Cplain;
        }else{
            sum_of_specialpizzas++;
            total_profit +=Cspecial;
        }
        
    }
}


//Gives the seed,imported from the user, for the random numbers genarators
unsigned int* random_number_seed(void) {
    return &random1;
}

//Checks for order failure or submission
void* order(void* x) {

    pthread_mutex_lock(&lock);
    int id_thread = *(int*)x;
    printf("H paraggelia %d xekinhse\n", id_thread);

    //Finds the random number of pizzas by order
    int random_number_pizza = rand_r(random_number_seed()) % Norderhigh + Norderlow;

    //Initialization of pizzas atrray
    int pizzas[random_number_pizza];

    //Determines if a pizza is special or plain based on the given possibility
    for (int i = 0; i < random_number_pizza; i++) {

        float possibility_plain = (float)rand_r(random_number_seed()) / RAND_MAX;
        if (possibility_plain <= 0.6) {
            pizzas[i] = 0;//plain
        }
        else {
            pizzas[i] = 1;//special
        }
    }

    int paying_time=rand_r(random_number_seed())%Torderlow+Torderhigh;//Random paying time
    sleep(paying_time);
    float possibility_failed=(float)rand_r(random_number_seed())/RAND_MAX;
    
    //Checks if the order has failed or submitted based on the given possibility
    if(possibility_failed<=0.1){
        printf("Order with number %d failed\n",id_nhma);
        unsuccesfull_orders++;
        cancelation_id(id_nhma);
        pthread_mutex_unlock(&lock);
    }else{
        printf("Order with number %d submitted\n",id_nhma);
        succesfull_orders++;
        sales(pizzas,random_number_pizza);
        
    }

    pthread_mutex_unlock(&lock);
    free(pizzas);

    return 0;
}