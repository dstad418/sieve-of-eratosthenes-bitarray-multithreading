#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>


// MACRO
#define NUMBITS 32
//END MACRO


// STRUCT
typedef struct BitBlock_s
{
    uint32_t bits;    // Bits 0-31   
    pthread_mutex_t mutex;  // pthread_mutex_init(&Ptr->mutex, NULL);
} BitBlock_t;
// END STRUCT


// GLOBALS
static int number_of_threads = 1;     // DEFAULT NUMBER OF THREADS 
static int upper_bound = 100;   // UPPER BOUND AS DETERMINED BY USER
static int verbose = 0;     // VERBOSE FLAG
static int array_size = 0;      // Array-size in case it needs to be reviewed
BitBlock_t *sieve_array;
// END GLOBALS


// PROTOTYPES
void* thread_func(void *arg);
void bit_printer(void);
int prime_detector(unsigned long num);
// END PROTOTYPES


// DRIVER
int 
main(int argc, char *argv[])
{
    int opt = -1;
    pthread_t *threads;
    //int tid;

    while((opt = getopt(argc, argv, "t:u:h:v")) != -1)
    {
        switch(opt)
        {
// VARIABLE SET
            case 't':
                number_of_threads = atoi(optarg);
                if(number_of_threads < 1)
                {
                    fprintf(stderr, "Value given is too low. Please rerun the program with a value of one or more for the thread value.\n");
                    return 0;
                }
                break;

            case 'u':
                upper_bound = atoi(optarg);
                if(upper_bound < 2)
                {
                    fprintf(stderr, "Value given is too low. Please rerun the program with a value over 2.\n");
                    return 0;
                }
                break;

            case 'h':
                verbose = 1;
                fprintf(stderr, "Optimus Prime Help Flag Initiated\nThe Thread flag can be initiated using '-t' on the command line, followed by the number of threads you would like to use. The default value is set to a single thread\n\n\te.g.: -t 10\n\nThe Upper-Bound flag is '-u', followed by the upper-bound you'd like to set regarding the number of prime numbers to calculate. The default value is 100 numbers.\n\n\t e.g.: -u 2000\n\nYou've accessed the Help option using '-h'. This displays command-line options and exits the program.\n\nThe Verbose flag is activated with '-v'. Using it will initiate additional diagnostic options and output text to the terminal using stderr.\n");
                return 0;
                break;

            case 'v':
                verbose = 1;
                fprintf(stderr, "\n\t\t--Optimus Prime: Verbose flag actived --\n\nAt the time of the calling of this flag:\nThe Upper Bounds are set to %d\n", upper_bound);
                fprintf(stderr, "The Number of Threads are set to %d\n", number_of_threads);                
                fprintf(stderr, "And verbose mode has been activated. \n!!!!!Diagnostics and location data will now display on screen!!!!!\n");                
                break;

            default: /* ? */
                printf("Unknown flag, please use '-h' to refresh on accepted parameters.\n");
                return 0;
        }
    }

    // Set upper bound to custom level
        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "Arguements have been taken from the stream, and the upper-bound will be set. If no custome value, the default is 100\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+

    if(upper_bound != 100) // SET ARRAY TO CUSTOM VALUE
    {
        array_size = ((upper_bound/NUMBITS) + 1);
    }
    else // SET ARRAY TO DEFAULT VALUE
    {
        array_size = ((upper_bound/NUMBITS) + 1);
    }
        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "Array_size set to: %d\n", array_size);
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+

    // ARRAY SIZE DETERMINED!
    sieve_array = calloc(array_size, sizeof(BitBlock_t)); 

    for(int l = 0; l < array_size; l++)
    {
        sieve_array[l].bits = 0x0;
        pthread_mutex_init(&sieve_array[l].mutex, NULL);
    } 


// PTHREAD, MUTEX AND CREATE
        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "Declaring and initializing mutex\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
//    sieve_array = malloc(array_size * sizeof(BitBlock_t));    
    threads = malloc(number_of_threads * sizeof(pthread_t));

    for(long j = 0, start = 3; j < number_of_threads; j++, start += 2) //for(long i = 0, start = 3; i < number_of_threads; i++, start += 2)
    {
        pthread_create(&threads[j], NULL, thread_func, (void *) start);
        //printf("TID IS: %d\n", tid);
        // Spaces them out appropriately!
        //{
          //  fprintf(stderr, "Error in thread creation. Error with thread: %ld\n", j);
           // break;
        //}
    } 
        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "PTHREAD_CREATE just ran\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+




// JOIN ALL
    for(int k = 0; k < number_of_threads; k++) // Cycle and close
    {
        if(pthread_join(threads[k], NULL) != 0)
        {
            fprintf(stderr, "Failed to join thread! Error with thread: %d", k);
            break;
        }
    }
        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "PTHREAD_JOIN just ran\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+


// OUTPUT
    bit_printer();
/*
    for(long q = 0; q < upper_bound; q++)
    {
        printf("Bit for number %ld:\t%d\n", q + 1, ((sieve_array[q/NUMBITS].bits) &= (0x1 << (q % NUMBITS))));
    }
*/
// DEALLOC
    for(int m = 0; m < number_of_threads; m++) // Cycle and close
    {
        if(pthread_mutex_destroy(&sieve_array[m].mutex) != 0)
        {
            fprintf(stderr, "Failed to destroy thread!\n");
            break;
        }
    }
    free(sieve_array);
    free(threads);

    return 0;
}


//  THREAD FUNCTION
void*
thread_func(void* arg)
{
    long start_pause;
    //BitBlock_t sieve_array[array_size]; // Array size determined above
    //sieve_array[array_size].bits = 0;   // INITIALIZING ALL ARRAY VARIABLES TO o -- !!!1 IS COMPOSTIVE / NOT PRIME!!!
    //uint32_t mask = 0x0;

        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "Entering THREAD_FUNC()\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+

    start_pause = (long)arg;

    for(long i = start_pause; i < (sqrt(upper_bound) + 1); i += (number_of_threads * 2))
    {
        for(long j = (j * 2); j <= upper_bound; j += i)
        {
            int temp = prime_detector(j);
            //int temp2 = prime_detector(i);
            long bb_index = (j/NUMBITS);
            long bb_bit = (j % NUMBITS);
            //uint32_t mask = 0x1 << bb_bit;

            pthread_mutex_lock(&sieve_array[bb_index].mutex);
    
                if(verbose == 1)//!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
                {
                    fprintf(stderr, "Mutex lock was just engaged\nValue to be tested is: %ld\nReturned bit value is: %d\n\n", j, temp);       //      <-- VERBOSE FOR MUTEX LOCK
                }               //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+

            if (temp == 1)
            {
                //mask = 0x1 << (bb_bit);
                //printf("wtf");
                sieve_array[bb_index].bits |= 0x1 << (bb_bit);
            }
            else
            {
                sieve_array[bb_index].bits |= (0x0 << bb_bit);
            }

            // Set or clean   <-- Use mask, bitwise or/and

            pthread_mutex_unlock(&sieve_array[bb_index].mutex);
                
        }
    }
        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "LEAVING THREAD FUNC\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+

    pthread_exit(EXIT_SUCCESS);
}


//  BIT PRINTER
void 
bit_printer(void)
{
    uint32_t mask = 0x0;;

        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "ENTERING BIT_PRINTER\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+

    printf("2\n");  // Starting at two, from there taking it 2 at a time! 
    for(unsigned long i = 2; i < upper_bound; i += 2) // Unsigned because i'm not sure how big this will be...
    {  
        mask = 0x0;

        if(((sieve_array[i/NUMBITS].bits) &= (mask)) == 0)
        {
            printf("%ld\n", (i + 1));
        }
    }
        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "LEAVING BIT_PRINTER\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
}


// PRIME DECTECTOR
int
prime_detector(unsigned long num)
{
    //unsigned int temp = sqrtl(num);
    if(num <= 1)
    {
        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "THE VALUE FOR THIS RETURNED BIT IS 1: NOT PRIME\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        return 1; 
    }
    if(num <= 3)
    {
            if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
            {
                fprintf(stderr, "THE VALUE FOR THIS RETURNED BIT IS 0: PRIME\n");
            }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        return 0;
    }
    if(num%2==0||num%3==0||num%4==0||num%6==0||num%8==0||num%9==0||num%10==0)
    {
            if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
            {
                fprintf(stderr, "THE VALUE FOR THIS RETURNED BIT IS 1: NOT PRIME\n");
            }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        return 1;
    }
    else if(num%5==0 && num != 5)
    {
        return 1;
    }
    else if(num%7==0 && num != 7)
    {
        return 1;
    }
    else
    {}


/*
    for(int i = 5; i <= temp; i += 6)
    {
        if(num % i == 0 || num % (i + 2) == 0)
        {
                if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
                {
                fprintf(stderr, "THE VALUE FOR THIS RETURNED BIT IS 1: NOT PRIME\n");
                }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
            return 1;
        }
    }
        if(verbose == 1)        //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
        {
           fprintf(stderr, "THE VALUE FOR THIS RETURNED BIT IS 0: PRIME\n");
        }                       //!+!+!+!+!+!+!+!+!+!+!+!+ VERBOSE TESTER SECTION !+!+!+!+!+!+!+!+!+!+!+
   */ return 0;
}



