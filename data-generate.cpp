// Caroline Kirkconnell (CarolineKirkconnell8@gmail.com)
// @version (final)


#include <stdio.h>
#include <pthread.h>
#include <iostream>
using namespace std;

// This macro defines how many units of data will be generated in total.
#define MAX 10 

// Mutex makes sure that every time only one thread accesses the buffer.
// May be consumer thread or the producer thread.
pthread_mutex_t the_mutex;

// Blocked thread waits for the "traffic lights" before it make an action.
pthread_cond_t condc, condp;

// Integer variable is buffer.
// The buffer stores one unit of data, an integer.
int buffer = 0;

void* producer(void* ptr)
{
    int i;

    for (i = 1; i <= MAX; i++)
    {
        // Step 1, uses the function to lock the mutex, and then
        // producer thread use the buffer after that. 
        pthread_mutex_lock(&the_mutex);

        // Step 2, if the buffer is NOT empty, the buffer holds some data
        // Must wait for traffic light "condp" before output to the buffer,
        while (buffer != 0) pthread_cond_wait(&condp, &the_mutex);

        // Step 3, the producer generates one integer, 
        // which is one unit of data assigned to the buffer.
        buffer = i * 7;

        // Step 4, prints out what producer generates.
        cout << "producer produce item " << buffer << endl;

        // Step 5, buffer is full now.
        // We stop, and tell the consumer thread that you are ready. 
        pthread_cond_signal(&condc);

        // Step 6, we unlock the mutex so the "dressing room" 
        //is available. 
        pthread_mutex_unlock(&the_mutex);
    }

    // Step 7, exit the thread, return with 0. 
    pthread_exit(0);

}

void* consumer(void* ptr)
{
    int i;
    int get_data;

    for (i = 1; i <= MAX; i++)
    {
        // Lock the_mutex before you start, see step 1 example in producer 
        pthread_mutex_lock(&the_mutex);

        // When the buffer IS empty, nothing to take, so we wait, 
        while (buffer == 0) pthread_cond_wait(&condc, &the_mutex);

        // Assigns buffer data to the variable get_data
        get_data = buffer;

        // Prints out the content of what is in get_data
        cout << "consumer consume item " << get_data << endl;

        // Clears the buffer and assign value 0.
        buffer = 0;

        // Tells condition variable of producer is ready
        pthread_cond_signal(&condp);

        // Unlock the mutex.
        pthread_mutex_unlock(&the_mutex);
    }

    // exit the thread, return with 0, which means ready.
    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    // change the following id into your banner id
    int banner_id = 903681107;
    cout << "Banner id: " << banner_id << endl;

    pthread_t pro, con;
    pthread_mutex_init(&the_mutex, 0);      
    pthread_cond_init(&condc, 0);           
    pthread_cond_init(&condp, 0);            

    pthread_create(&con, 0, consumer, 0);   
    pthread_create(&pro, 0, producer, 0);   
    pthread_join(pro, 0);                    
    pthread_join(con, 0);                    

    // destroy the mutex and two threads
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);

    return 0;
}
