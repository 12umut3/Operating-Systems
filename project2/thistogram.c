#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#define atoa(x) #x

// These are global variable 
// hist char array keeps all data coming from input files.
typedef struct {
    double *minvalue;
	double *maxvalue;
	int bincount;
 	int N;
	char *infile;
 	char *outfile;
	char hist[1024][1024];
	int count2;
} thistogram_struct;

void *thistogram (void *args){
	//This time we need to just reading to input files
	thistogram_struct *actual_args = args;
    FILE *fr = fopen(actual_args->infile, "r");
    if (fr == NULL){
        exit(EXIT_FAILURE);
    }
	//str array keeps each line from a reading input file 
    char str[1024];
    double numb[1024];
    int lineNumb = 0;
    double n = 0.0;
	//str elements are converted double and keeps these values another array
    while(fgets (str, 1024, fr)){
		n = strtod(str,NULL);
      	numb[lineNumb] = strtod(str,NULL);
        lineNumb++;
	}

    int i  = 0;
    double min = numb[0];
    double max = numb[0];
    (*actual_args->minvalue) = min;
    (*actual_args-> maxvalue) = max;

    /*
     * Find maximum and minimum in all numb[] array elements.
     */
    for(i=0; i < lineNumb; i++)
    {
        /* If current element is greater than max */
        if(numb[i] > max)
        {
            max = numb[i];
        }

        /* If current element is smaller than min */
        if(numb[i] < min)
        {
            min = numb[i];
        }
    }
    (*actual_args->minvalue) = min;
    (*actual_args-> maxvalue) = max;

	// diff1 / diff2 gives "w" value.
    int argbincount = actual_args->bincount;
    double diff1 = (max-min);
    double w = diff1/ argbincount;
	int count = 0;
	int inc = 0;

	for(int i = 1; i < argbincount+1; i++){
		for(int j = 0; j < lineNumb; j++){
			if(numb[j] >= (min+inc) && numb[j] < (min + inc + w)){ // to find how many numbers are in these range
				count++;
			}
		}
		inc = inc + w; // increments [min+w, min+2w). 
		char *string = atoa(count);  // every count value (number of values in the range) keeps hist array
		char *string2 = atoa(i);
		asprintf (&string, "%i", count); 
		asprintf (&string2, "%i: ", i);
		char *input[] = {"",string2, string, "\n"}; // all data coming from inputs file keeps hist array as a string
		count = 0;
		for(int k = 1; k<=4; k++){
			strcpy(actual_args->hist[actual_args->count2],input[k-1]); //input array local and keeps data and strcpy to global array hist 
			actual_args->count2++;
		}
		actual_args->count2++;
	}
   fclose(fr);
}

int main(char argc, char **argv)
{
	/*struct timeval start, end;
	start.tv_usec = 0;
  	end.tv_usec = 0;
	gettimeofday(&start, NULL);*/

    int bincount = atoi(argv[1]);
    int InputsNumb = atoi(argv[2]);
	double minvalue,maxvalue;
    char outfile;

	pthread_t tid[InputsNumb];
    thistogram_struct *args = malloc(sizeof *args);

    for(int i = 0; i < InputsNumb; i++)
    {
		
		args->minvalue = &minvalue;
		args->maxvalue = &maxvalue;
		args->bincount = bincount;
		args-> N = InputsNumb;
        args->infile = argv[i+3];
		args->outfile = argv[InputsNumb+3];

       	pthread_create(&(tid[i]), NULL, &thistogram, args);
		pthread_join(tid[i], NULL);

	}
    //sum up all data to one output file
    FILE *fw;
    fw = fopen(argv[InputsNumb + 3], "wb");
    int count = 0;
    int sum2[1024];
    int j = 2;
    int sum = 0;


    for(int k = 1; k < bincount + 1; k++){
        while(count < InputsNumb){
            sum = sum + atoi(args->hist[j]);
            j = j + (5*bincount); // takes range values by using atoi(hist[j]). j is increments 5 according to char array substring
			count++;
        } 
		//write the main output file
        fprintf(fw, "%d:", k);
        fprintf(fw,"%d", sum);
        putc('\n', fw);
        sum = 0;
		count = 0;	
		j = 5*k+2;
    }

    fclose(fw);

  	/*gettimeofday(&end, NULL);
	long seconds = end.tv_sec - start.tv_sec;
	long micro_seconds = end.tv_usec - start.tv_usec;
	long total_micro_seconds = (seconds * 1000000) + abs(micro_seconds);
	printf("The program took: %ld milliseconds.\n", total_micro_seconds);*/

    _exit(0);

    return 0;
}




