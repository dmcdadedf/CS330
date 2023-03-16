/**
 * Parallel version of code for calculating Pi 
 *
 * CS330 Project #4
 *
 * Compile with:
 *      gcc -lpthread pi.c -o pi -lm
 *
 * See section 7.3.1 Posix Mutex Locks in Operating Systems Concepts by Silberschatz
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void *worker(void *param);

#define NUMBER_OF_DARTS         50000000
#define NUMBER_OF_THREADS       4

/* the number of darts that hit in the circle */
int circle_count = 0;

/***** DO THIS *****/
/* Declare a variable with name mutex of type pthread_mutex_t  */
pthread_mutex_t mutex; //Garrett Contee

/*
 * This function generates a double precision random number between 0 and 1.
 * Note that random() itself produces a random integer between 0 and the large integer given by RAND_MAX.
 */
double random_double() 
{
   return random() / ((double) RAND_MAX + 1);
}


int main(int argc, const char * argv[])
{
   int darts_per_thread = NUMBER_OF_DARTS / NUMBER_OF_THREADS;
   int i;

   double estimated_pi;

   pthread_t workers[NUMBER_OF_THREADS];

   /***** DO THIS *****/
   /* Add a Pthreads-related function call to create and initialize the mutex lock.  */
   pthread_mutex_init(&mutex, NULL); //David McDade

   /* seed the random number generator from the system time */
   srandom((unsigned) time(NULL));

   for (i = 0; i < NUMBER_OF_THREADS; i++)
      pthread_create(&workers[i], 0, worker, &darts_per_thread);

   for (i = 0; i < NUMBER_OF_THREADS; i++)
      pthread_join(workers[i], NULL);

   /* estimate Pi */
   estimated_pi = 4.0 * circle_count / NUMBER_OF_DARTS;

   printf("Pi = %f\n", estimated_pi);

   return 0;
}


/*
 * This function throws "darts" at the 2 by 2 square and counts how many hit inside the radius 1 circle. 
 * param points to the number of darts needed. The x and y coordinates for where each dart hits are
 * randomly generated. The number of darts that hit within the circle is added onto the global
 * circle_count.
 *
 ***** DO THIS: Add Pthreads-related function call(s) to protect the critical section of code where
 ***** circle_count is updated inside this function.
 */
void *worker(void *param)
{
   int number_of_darts;
   number_of_darts = *((int *) param);  //*** Dereference param to find the integer pointed at (darts_per_thread).
   int i;
   int hit_count = 0;
   double x, y;

   for (i = 0; i < number_of_darts; i++) {
      /* generate random numbers between -1.0 and +1.0 (exclusive) for both x and y */
      x = random_double() * 2.0 - 1.0;
      y = random_double() * 2.0 - 1.0;

      //***** DO THIS: At this spot, write your explanation as to why the sqrt (square root) function is not
      //***** needed around the x * x + y * y. After all, the distance of the point (x, y) from the origin (0, 0)
      //***** is sqrt(x^2 + y^2), where we are using x^2 to mean x squared, x to the power 2 and sqrt means the
      //***** square root. (The ^ is not proper C code, which is why we use x * x to get x^2, etc.
      //***** NOW ANSWER THE QUESTION: Why don't we need to use the sqrt function here? Type your answer here:
      // We do not need the square root in this, because the formula sqrt(x^2 + y^2) is used to calculate distance,
      // and we do not care about the distance they are from the origin. Distance is not needed to estimate pi, 
      // just where the dart landed is needed. Therefore, we do not need the distance formula, meaning we do not 
      // need the sqrt function.  David McDade
      if (x * x + y * y < 1.0)
         hit_count++;

   }
   pthread_mutex_lock(&mutex); //Benjamin Hill
   circle_count += hit_count;
   pthread_mutex_unlock(&mutex); //Benjamin Hill
   pthread_exit(0);
}