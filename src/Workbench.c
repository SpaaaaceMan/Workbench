#include "bor-util.h"
#include "Sorts/Bubbles.c"
#include "Sorts/SelecPerm.c"
#include "Sorts/DichoInser.c"

#include <stdio.h>  // printf
#include <stdlib.h>
#include <time.h>   //clock_t
#include <string.h> // strcmp
#include <unistd.h> // sleep
#include <signal.h> //SIGALARM

// Table sizes to test
#define SIZE1  100
#define SIZE2  500
#define SIZE3  5000
#define SIZE4  10000
#define SIZE5  50000
#define SIZE6  100000
#define SIZE7  200000
#define SIZE8  300000
#define SIZE9  400000
#define SIZE10 500000
#define SIZE11 600000
#define SIZE12 700000
#define SIZE13 800000
#define SIZE14 900000
#define MAX    1000000

// List of all available alorithims
#define BUBBLES        "Bubbles"
#define SEQUENTIAL_INS "SequentialInsertion"
#define DICHO_INS 	   "DichotomousInsertion"
#define SELEC_PERM     "SelectionPermutation"
#define FUSION 		   "Fusion"
#define QUICKSORT	   "QuickSort"
#define FIND_TREES     "FindTrees"
#define STACK 		   "Stack"

// Number of test per size
#define NB_TEST_PER_SIZE 20

// Maximum time allowed to a test (5 minutes)
#define TIME_BEFORE_STOP 300

// Maximum value for a random element
#define MAX_RAND_VALUE 100

// All the sizes to test
const size_t sizesTab[15] = {SIZE1, SIZE2, SIZE3, SIZE4, SIZE5, SIZE6,
						SIZE7, SIZE8, SIZE9, SIZE10, SIZE11, SIZE12,
						SIZE13, SIZE14, MAX};

// All the available algorithims
const char* algos[8] = {BUBBLES, SEQUENTIAL_INS, DICHO_INS, SELEC_PERM, FUSION, QUICKSORT
						FIND_TREES, STACK};

// The name of the chosen algorithim
const char* chosenAlgo;
// The pointer to the chosen algorithim
void (*algo)(int tab[], const size_t tabSize);

//int * tab = (int *) malloc (1 000 000 * sizeof(int));
//		(int*) calloc (1 000 000, sizeof(int));  = malloc + memset


// The files desciptor of the tube to communicate results
int pipeFds[2];


/**
* Initialize all values in tab of size size to a random number. tab must have been set to size size.
* @param tab The table to generate values into. Must have been set to size size.
* @param size the size of tab.
* @author Remi SEGRETAIN
*/
void initTabRand (int tab[], const size_t size) {
	for (size_t i = 0; i < size; ++i) {
		tab[i] = (unsigned int) rand() % MAX_RAND_VALUE;
	}//for
}// initTabRand

/**
* Displays an error message about arguments
* @author Thomas MEDARD
*/
void displayArgsErr(void) {
	fprintf(stderr, "Need to precise a valid algorithim.\nValids algorithims are :\n");
	
	for (size_t i = 0; i < sizeof(algos) / sizeof(algos[0]) - 1; ++i) {
		fprintf(stderr, "%s, ", algos[i]);
	}
	fprintf(stderr, "\n");
}

/**
* Placeholder function to replace unfinished sorting algorithim.
* @param tab the table to test
* @param tabSize the size of tab
* @author Thomas MEDARD
*/
void testTime(int tab[], const size_t tabSize) {
	sleep(1);
}

/**
* Function called on SIGALRM (when a test is too long).
* @param sig the code of the signal (unused but needed).
* @author Thomas MEDARD
*/
void alarmHandler(int sig) {
	printf("Test too long\n");
	exit(0);
}

/**
* Handle SIGPIPE to display a messageinstead of just stoping
* @author Thomas MEDARD
*/
void sigPipeHandler(int sig) {	
	printf("Signal %d received\n", sig);
	exit(0);
}

/**
* Function to lauch a test of sizeToTest size.
* @param sizeToTest The size of the table to test.
* @author Thomas MEDARD, Rémi SEGRETAIN
*/
void launchTest(const size_t sizeToTest) {
	srand(time(0));
	// Creating the table
	int tab[sizeToTest];
	
	// Affecting SIGALRM handler
	bor_signal(SIGALRM, alarmHandler, 0);
	
	// All results
	double resultsSum = 0.0;
	
    // Launching alarm
	alarm(TIME_BEFORE_STOP);

	// Testing NB_TEST_PER_SIZE times then getting the average result
	for (int i = 0; i < NB_TEST_PER_SIZE; ++i) {
		
		// Generating  its values
		initTabRand(tab, sizeToTest);
		
		// Begin time mesure
		clock_t begin = clock();
		
		// Calling the chosen algorithim
		algo(tab, sizeToTest);
		
		// End time mesure
		clock_t end = clock();
		
		// Calculating passed time
		resultsSum += ((double)end - (double)begin) / CLOCKS_PER_SEC * 1000.0;
	}
	
	const double finalRes = resultsSum / (double) NB_TEST_PER_SIZE;
	
    ssize_t writeRes;
    double results[2];

    results[0] = sizeToTest;
    results[1] = finalRes;

    // Writing into the pipe the result

	writeRes = bor_write(pipeFds[1], results, 2);

	if (writeRes < 0) {
	    perror("read");
	    exit(1);
	}

	close(pipeFds[1]);
	printf("for size %d : %lf ms\n", sizeToTest, finalRes);
	
}

/**
* Affect chooseAlgo and algo corresponding to the argument.
* @param arg The chosen algo. If it's invalid, will exit with error message.
* @author Thomas MEDARD
*/
void chooseAlgo(char* arg) {
	if (strcmp(arg, BUBBLES) == 0) {
		chosenAlgo = BUBBLES;
		algo = bubblesSort;
	}
	else if (strcmp(arg, SEQUENTIAL_INS) == 0) {
		chosenAlgo = SEQUENTIAL_INS;
		algo = testTime;
	}
	else if (strcmp(arg, DICHO_INS) == 0) {
		chosenAlgo = DICHO_INS;
		algo = dichoInser;
	}
	else if (strcmp(arg, SELEC_PERM) == 0) {
		chosenAlgo = SELEC_PERM;
		algo = selecPerm;
	}
	else if (strcmp(arg, FUSION) == 0) {
		chosenAlgo = FUSION;
		algo = testTime;
	}
	else if (strcmp(arg, FIND_TREES) == 0) {
		chosenAlgo = FIND_TREES;
		algo = testTime;
	}
	else if (strcmp(arg, STACK) == 0) {
		chosenAlgo = STACK;
		algo = testTime;
	}
	else if (strcmp(arg, QUICKSORT) == 0) {
		chosenAlgo = QUICKSORT;
		algo = testTime;
	}
	else {
		displayArgsErr();
		exit(1);
	}
}

/**
* The main
*/
int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		displayArgsErr();
		exit(1);
	}
	
	chooseAlgo(argv[1]);

    // Creating pipe
    int fdp = pipe(pipeFds);
    
    if (fdp < 0) {
        perror("pipe");
        exit(1);
    }
	
	pid_t forkRes;
	
	for (unsigned int i = 0; i < 15; ++i) {
		forkRes = fork();
		
		if (forkRes < 0) {
			perror("fork");
			exit(1);
		}
		
        // Child
		if (forkRes == 0) {
            close(pipeFds[0]);
			launchTest(sizesTab[i]);
			exit(0);
		}
	}

    // Father
    close(pipeFds[1]);

    bor_signal(SIGPIPE, sigPipeHandler, SA_RESTART);
	
	int status;
	pid_t done;

    ssize_t readRes;
    double results[30];
	
	while (1) {
		done = wait(&status);
		
		if (done == -1) {
			// no more child processes
			if (errno == ECHILD) {
				break; 
			}
		} 
		else {
			if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
				fprintf(stderr, "pid %d failed\n", done);
				exit(1);
			}
		}
		
        printf("debug\n");

		while(1) {
			readRes = bor_read(pipeFds[0], results, 30);

			if (readRes <= 0) {
				break;
			}
		}

        printf("father : size %lf : %lf ms\n", results[0], results[1]);

	}
		
    close(pipeFds[0]);

	// TODO gather all csv files from results and create the final one (and removing the others)
	printf("Ended\n");

	return 0;
}
