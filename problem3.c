#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_CHEFS 3
#define DISHES_PER_CHEF 10

// Semaphores and mutex declarations
sem_t semaphoreChefs[NUM_CHEFS];
sem_t semaphoreFinish;
sem_t providerReady;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Shared variables
int chefCookCount[NUM_CHEFS];
int allCooked = 0;
double totalCookingTime = 0.0;

const char* providerOffers[NUM_CHEFS] = {"Vegetables & Meat (A+B)", "Meat & Spices (B+C)", "Vegetables & Spices (A+C)"};
const int cookingTimes[NUM_CHEFS] = {5, 3, 1};
const int providerPrepTime = 2;

struct timespec startTime, endTime;

void *chef(void *pVoid)
{
    int chefNumber = *(int*)(pVoid);

    for (int i = 0; i < DISHES_PER_CHEF; ++i)
    {
        /////////////////////////////////////////////////
        // Implement your code for chef actions here.
        // Remember to include:
        // - Waiting for ingredients
        // - Printing received ingredients
        // - Simulating preparation and cooking time
        // - Updating cook count and total cooking time
        // - Signaling finish
        /////////////////////////////////////////////////
    }
    
    pthread_exit(NULL);
}

void *provider(void *pVoid)
{
    /////////////////////////////////////////////////
    // Implement your code for provider initialization here.
    /////////////////////////////////////////////////

    while (1)
    {
        /////////////////////////////////////////////////
        // Implement your code for provider actions here.
        // Remember to include:
        // - Checking if all chefs are done
        // - Selecting the next chef
        // - Offering ingredients
        // - Simulating preparation time
        // - Signaling chef to start cooking
        // - Waiting for chef to finish
        /////////////////////////////////////////////////
    }

    return NULL;
}

int main(void)
{
    srand(time(NULL));

    // Initialize shared variables
    /////////////////////////////////////////////////
    // Implement your code to initialize shared variables here.
    /////////////////////////////////////////////////

    // Initialize semaphores
    /////////////////////////////////////////////////
    // Implement your code to initialize semaphores here.
    /////////////////////////////////////////////////

    pthread_t chefThreads[NUM_CHEFS], providerThread;
    int chefNumbers[NUM_CHEFS] = {1, 2, 3};

    // Start timing
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    // Create chef and provider threads
    /////////////////////////////////////////////////
    // Implement your code to create threads here.
    /////////////////////////////////////////////////

    // Join threads
    /////////////////////////////////////////////////
    // Implement your code to join threads here.
    /////////////////////////////////////////////////

    // End timing
    clock_gettime(CLOCK_MONOTONIC, &endTime);

    double totalTime = (endTime.tv_sec - startTime.tv_sec) + 
                       (endTime.tv_nsec - startTime.tv_nsec) / 1e9;

    printf("All chefs have finished cooking.\n");
    printf("Total running time: %.2f seconds\n", totalTime);
    printf("Total cumulative cooking time (including ingredient wait time): %.2f seconds\n", totalCookingTime);

    // Clean up
    /////////////////////////////////////////////////
    // Implement your code to destroy semaphores and mutex here.
    /////////////////////////////////////////////////

    return 0;
}