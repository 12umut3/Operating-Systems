#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>


#define cylinders 200
#define request 8

int start = 0;
int end = 0;


// FCFS schedule
int fcfs(int *numb, int headpos) {

	int i = 0, head_movement = 0;

    for(i = start; i < request; i++) {

    	head_movement += abs(numb[i] - headpos);
		//printf("%s%d%s%d\n", " numb[i]: ",numb[i]," headpos: ", headpos);
		//printf("%d\n", head_movement);
		headpos = numb[i];
    }

    for(i = 0; i < start; i++) {

    	head_movement += abs(headpos - numb[i]);
    }
        
    return head_movement;
}
// SSTF schedule
int sstf(int *numb,int headpos) {
	int head_movement = 0;
	int numb2[request];
	int temp = 0;
	//get distance from head of elems in queue
	for(int i=0; i<request; i++){
		numb2[i] = abs(headpos-numb[i]);
	}

	//swap elems based on their distance from each other
	for(int i=0; i<request; i++){
	  for(int j=i+1; j<request;j++){

		if(numb2[i]>numb2[j] || numb2[i]==numb2[j]){
		    temp = numb2[i];
		    numb2[i]=numb2[j];
		    numb2[j]=temp;

		    temp=numb[i];
		    numb[i]=numb[j];
		    numb[j]=temp;
		}
	}

	}
	for(int i=0; i<request; i++){
		head_movement += abs(headpos-numb[i]);
		//printf("%s%d%s%d\n", " numb[i]: ",numb[i]," headpos: ", headpos);
		//printf("%d\n",head_movement);
		headpos = numb[i];
	}
	return head_movement;
}
// SCAN schedule
int scan(int *numb, int headpos){
	int head_movement = 0;
	sort_array(numb);
	int i = 0;
	while(numb[i] < headpos){
		i++;
	}
	//printf("%d",i);
	//printf("%d", numb[i]);
	if(numb[i] > headpos || numb[i] == headpos){
		int j = i;
		while(j < request){
			head_movement += abs(numb[j] -headpos);
			//printf("%s%d%s%d\n", " numb[i]: ",numb[j]," headpos: ", headpos);
			//printf("%d\n",head_movement);
			headpos = numb[j];
			j++;
		}
		//printf("%s%d", "headpos: ",headpos);
		head_movement += abs((cylinders-1) -headpos);
		//printf("%d",head_movement);
		headpos = cylinders -1;		
		while(i > 0){
			head_movement += abs(numb[i-1] - headpos);
			//printf("%s%d%s%d\n", " numb[i]: ",numb[i-1]," headpos: ", headpos);
			//printf("%d\n",head_movement);
			headpos = numb[i-1];
			i--;
		}

	}
	return head_movement;
}
// C-SCAN schedule
int c_scan(int *numb, int headpos){
	int head_movement = 0;
	sort_array(numb);
	int i = 0;
	while(numb[i] < headpos){
		i++;
	}
	//printf("%d",i);
	//printf("%d", numb[i]);
	if(numb[i] > headpos || numb[i] == headpos){
		int j = i;
		int k = i;
		while(j < request){
			head_movement += abs(numb[j] -headpos);
			//printf("%s%d%s%d\n", " numb[i]: ",numb[j]," headpos: ", headpos);
			//printf("%d\n",head_movement);
			headpos = numb[j];
			j++;
		}
		//printf("%s%d", "headpos: ",headpos);
		head_movement += abs((cylinders-1) -headpos);
		//printf("%d",head_movement);
		headpos = cylinders -1;	
		head_movement += headpos;
		i = 0;
		headpos = 0;
		while(i < k){
			head_movement += abs(numb[i] - headpos);
			//printf("%s%d%s%d\n", " numb[i]: ",numb[i]," headpos: ", headpos);
			//printf("%d\n",head_movement);
			headpos = numb[i];
			i++;
		}
	}
	return head_movement;
}
// C-LOOK schedule
int c_look(int *numb, int headpos){
	int head_movement = 0;
	sort_array(numb);
	int i = 0;
	while(numb[i] < headpos){
		i++;
	}
	//printf("%d",i);
	//printf("%d", numb[i]);
	if(numb[i] > headpos || numb[i] == headpos){
		int j = i;
		int k = i;
		while(j < request){
			head_movement += abs(numb[j] -headpos);
			//printf("%s%d%s%d\n", " numb[i]: ",numb[j]," headpos: ", headpos);
			//printf("%d\n",head_movement);
			headpos = numb[j];
			j++;
		}
		//printf("%s%d", "headpos: ",headpos);
		//printf("%d",head_movement);
		int i = 0;
		while(i < k){
			head_movement += abs(numb[i] - headpos);
			//printf("%s%d%s%d\n", " numb[i]: ",numb[i]," headpos: ", headpos);
			//printf("%d\n",head_movement);
			headpos = numb[i];
			i++;
		}
	}
	return head_movement;
}
// LOOK schedule
int look(int *numb, int headpos){
	int head_movement = 0;
	sort_array(numb);
	int lastElem = numb[request-1];
	int firstElem = numb[0];
	head_movement = abs(lastElem - headpos) + abs(lastElem - firstElem);
	return head_movement;
}
//sort array
void sort_array(int *numb) {

	int temp = 0, i = 0, j = 0;

	for (i = 0; i < request; i++) {

	   	for (j = 0 ; j < request; j++) {

		    if (numb[i] < numb[j]) {

		        temp =  numb[i];
		        numb[i] = 	numb[j];
		        numb[j] = temp;
		    }
		}
	}
}

int main(char argc, char **argv)
{
	int headpos = atoi(argv[1]);
	char *inputfile = argv[2];

    FILE *fr = fopen(inputfile, "r");
	//str array keeps each line from a reading input file 
	char str[8];
	int numb[8];
	int lineNumb = 0;
	int n = 0;
	if(fr != NULL){
		//str elements are converted float and keeps these values another array
		while (fgets (str, sizeof str, fr)) {
		
			char* num = strchr(str, ' '); // Retrieve a pointer to the first occurence found of =

			if ( num != NULL )              // Test if we found an occurence
			{
				num++;                      // We want to begin after the '='
			}
		    numb[n] = atoi(num);
		    n++;
		}
	}
	else if(fr == NULL){
		srand (time(NULL) );
		for(int i = 0; i < request; i++) {
			numb[i] = rand() % cylinders;
			//printf("%d ",numb[i]);
		}
			//printf("\n");
	}
	printf("FCFS: %d\n", fcfs(numb,headpos));
	printf("SSTF: %d\n", sstf(numb,headpos));
	printf("SCAN: %d\n", scan(numb,headpos));
	printf("C-SCAN: %d\n", c_scan(numb,headpos));
	printf("LOOK: %d\n", look(numb,headpos));
	printf("C-LOOK: %d\n", c_look(numb,headpos));
	return 0;
}

