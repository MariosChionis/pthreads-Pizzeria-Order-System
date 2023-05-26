#include <stdio.h>
#include <pthread.h>
#include "GlobalaVariables.h"
#include <stdlib.h>
#include <unistd.h>
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

//Order Delivery Stage
void delivery(int id){

    //Condition to ensure that there is at least one delverer to deliver the order 
    while(Ndeliverer<1){
        printf("No delivery guy available\n");
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);
    Ndeliverer--;//Delivery started
    printf("Delivering order %d \n",id);
    unsigned int delivery_time=rand_r(random_number_seed())%Tdelhigh+Tdellow;
    sleep(delivery_time*2);//It is doubled because it is delivery_time delivering the order and delivery_time returning to the store
    pthread_mutex_lock(&lock);
    
    Ndeliverer++;//Delivery finished
    
    
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    
    printf("Order with number %d delivered in %f minutes \n",id,convertSecondsToMinutes(z));

}

//Order Packing stage
void packing(int id,int pizzas){
    
    //Condition to ensure that there is a packer to pack the order's pizzas
    while(Npacker<1){
        printf("Not enough packers..please wait \n");
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);
    Npacker--;
    
    for(int i=0;i<pizzas;i++){//For each pizza
        sleep(Tpack);//Tpack=Packing time
    }
    pthread_mutex_lock(&lock);
    
    Npacker++;//Packing stage finished
    
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    delivery(id);
    
}

//Baking stage
void baking(int pizzas,int id){

    //Condition to ensure that tehere are enough ovens to bake every pizza of an order at tha same time
    while(Noven<pizzas){
        printf("Not enough ovens..please wait \n");
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);
    Noven = Noven-pizzas;
    printf("Baking started for order %d,available ovens %d\n",id,Noven);
    sleep(Tbake);//Tbake=baking time
    pthread_mutex_lock(&lock);

    Noven = Noven+pizzas;//Baking stage finished for the order
   
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    packing(id,pizzas);
}

//Preparation stage
void preparation(int pizzas,int id){

    //Condition to ensure that there is a cook to start preparing the order
    while(Ncook<1){
        printf("Not enough cooks..please wait \n");
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);
    
    //There is a cook at least
    Ncook --;
    printf("Preparation started for the order\n");

    //Preparation for each pizza
    for(int i=0;i<pizzas;i++){
        sleep(Tprep);//Tprep*60 (because Tprep is in seconds)
        printf("Number %d pizza prepared for order %d \n",i+1,id);
    }
    pthread_mutex_lock(&lock);
    
    //End of an order's preparation so it is ready for baking
    Ncook++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    baking(pizzas,id);

}

//Checks for order failure or submission
void* order(void* x) {

    pthread_mutex_lock(&lock);
    int id_thread = *(int*)x;
    printf("Order with number %d started\n", id_thread);

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
    preparation(random_number_pizza,id_nhma);

    return 0;
}