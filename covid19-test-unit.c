#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> 

// The total number of person threads.
#define NUM_PERSON 28

void *person(void *numofPerson); //This function used for people's actions
void *healthStaff(void *numofHealthStaff);//This function used for health staffs' actions
void randwait(int secs);

//Defining semaphores
sem_t test_unit[8]; //test_unit semaphore limits each unit with 5 people at one time. It is counting semaphore
					//if value of this semaphore is 0, it means the test unit is full.
					//so no more people can enter this room
sem_t busyWaiting[8]; // busyWaiting semaphore using for defining waitings. It is binary semaphore
					  //0=busy, 1=not busy
int allDone = 0;//Flag to stop the thread when all person are gone.

//The test unit which we will start first
int availUnit= 0; //Units will be filled in order to prevent starvation. 
				  //The variable availUnit indicates which room the queue is in.
				  //The test units are used for testing. Starting from test unit 0 and are used sequentially.

int value; //Value for getting value of test_unit semaphore

int main(){
    
    pthread_t staffId[8];
    pthread_t personId[NUM_PERSON];

    
    int NumberP[NUM_PERSON]; //Number array of person
    int NumberS[8]; //Number array of health staff

    for(int a=0; a<NUM_PERSON; a++){
        NumberP[a] = a;
    }

    for(int c=0; c<8; c++){
        NumberS[c] = c;
    }
    //Initializing the semaphores with their initial values
    for(int i=0; i<8; i++){
        sem_init(&test_unit[i], 0, 3);
    }
    for(int j=0; j<8; j++){
        sem_init(&busyWaiting[j], 0, 0);  
    }

    //Creating Health Staffs
    for(int b= 0; b<8; b++){
        pthread_create(&staffId[b], NULL, healthStaff, (void *)&NumberS[b]);
    }

    //Creating Person
    for(int k= 0; k<NUM_PERSON; k++){
        int n= rand() %10;
        randwait(n);
        pthread_create(&personId[k], NULL, person, (void *)&NumberP[k]);//Creating 
    
    }
	// Join each of the threads to wait for them to finish.
    for(int t=0; t<NUM_PERSON; t++){
        pthread_join(personId[t], NULL);   
    }
    //When all of the people are gone, kill the health staff thread
    allDone=1;
    
	 for(int t=0; t<8; t++){
        pthread_join(staffId[t], NULL);   
    }
	
    system("PAUSE");  
    return 0;
}

//The function for room keeper thread
void *healthStaff(void *number){
    int num = *(int *)number;    
    printf("Health Staff %d ventilating the Test Unit %d.\n",num, num);
    sem_getvalue(&test_unit[availUnit],&value); 

    while(allDone==0){
        sem_wait(&busyWaiting[num]);
		//Starts Critical Section
			//In this section health staff makes some announcements about the test unit.
			//Other threads should not run in this section because the staff counting the value.
		if(value==3){
			printf("Health Staff %d stops ventilating the Test Unit %d.\n",num, num); 
			printf("Health Staff %d announcing there are %d chair available (_ _ _)\n",num, value); 
			
		}
		else if(value==2){
			printf("Health Staff %d announcing there are %d chair available (X _ _)\n",num, value); 
			
		}
		else if(value==1){
			printf("Health Staff %d announcing there are %d chair available (X X _)\n",num, value);
			printf("The last people, let's start! Please, pay attention to your social distance and hygiene; use a mask.\n");
			
		}
		else{
			printf("The Test Unit is full. We are starting the testing (X X X)\n",num);
			
			printf("The health staff %d is starting the test\n",num);
			randwait(5);
			printf("The health staff %d has finished the testing.\n",num); 
			
		}
    }
}
//The function for person thread
void *person(void *number){
    
    int num = *(int *)number;  
    printf("Person %d entering the hospital.\n", num);
    sem_getvalue(&test_unit[availUnit],&value); 
    if(value== 0){
        availUnit= (availUnit+1) % 8;
        
        randwait(5);
        for(int n=0; n<3; n++){
            sem_post(&test_unit[(availUnit-1)%8]);
        }
        
        printf("People leaving the test unit %d.\n", (availUnit)%8);
        
        sem_getvalue(&test_unit[availUnit],&value); 
        
    }
    sem_wait(&test_unit[availUnit]);   
    sem_post(&busyWaiting[availUnit]);
    sem_getvalue(&test_unit[availUnit],&value);
	if(value==2){
		printf("Person %d entering the test unit %d (X_ _)\n", num, availUnit);
	}
	else if(value==1){
		printf("Person %d entering the test unit %d (X X _) \n", num, availUnit);
	}
	else{
		printf("Person %d entering the test unit %d (X X X)\n", num, availUnit);
	}
    
    if(allDone==1){ 
        for(int n=0; n<3; n++){
            sem_post(&test_unit[(availUnit)%8]);
        }
        printf("People leaving the test unit %d.\n", (availUnit-1)%8);
    }
    
}

void randwait(int secs){
     int len = 1;
     sleep(len);
}