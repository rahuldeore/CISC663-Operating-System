
/******************************************************************************************
*HISTORY FUNCTIONS
*******************************************************************************************/

/*Creates a new Node and returns pointer to it.*/
struct Node* GetNewNode(char* cmdd) 
{
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	
	
	newNode->data = malloc(sizeof(cmdd) + 1);
	strcpy(newNode->data, cmdd);
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

 
/*Inserts a Node at tail of Doubly linked list*/
void InsertAtTail(char *cmdd) 
{
	struct Node* temp = head;
	struct Node* newNode = GetNewNode(cmdd);
	if(head == NULL) 
	{
		head = newNode;
		return;
	}
	while(temp->next != NULL) temp = temp->next; // Go To last Node

	temp->next = newNode;
	newNode->prev = temp;
}
 
/*Prints all the elements in linked list in forward traversal order*/
void ReversePrint() 
{
	struct Node* temp = head;	
	int count;
	if (commandArgv[1] != NULL)
	{
		count = atoi(commandArgv[1]);	
	}
	else
	{
		count = 10;	
	}
	if(temp == NULL) return; // empty list, exit
	// Going to last Node
	while(temp->next != NULL) 
	{
		temp = temp->next;
	}
	// Traversing backward using prev pointer
	printf("Reverse: \n");
	while(temp != NULL && count != 0) 
	{
		printf("%s\n",temp->data);
		temp = temp->prev;
		count--;
	}
	printf("\n");
}

/****************************************************
*	INPUT FUNCTIONS
****************************************************/
/**
 *	gets a line of text from user
 */
void getTextLine()
{
	
        destroyCommand();                                                              	// clean buffer and command line
        while ((userInput != '\n') && (bufferChars < BUFFER_MAX_LENGTH)) {
                buffer[bufferChars++] = userInput;
                userInput = getchar();
        }
        buffer[bufferChars] = 0x00;							// null terminate buffer
	pipbuffer = malloc (sizeof buffer);	
	strcpy(pipbuffer, buffer);					
	//printf("%s\n", buffer);
	InsertAtTail(buffer);                                                        
        populateCommand();
}

/**
 * populates commandArgv array with every words
 * of the command line (space " " used as separator)
 */
void populateCommand()
{
        char* bufferPointer;                                                                  // a pointer to the buffer
        bufferPointer = strtok(buffer, " ");
        while (bufferPointer != NULL) {                                               // while the pointer is not NULL
                commandArgv[commandArgc] = bufferPointer;                   // populate commandArgv array
                bufferPointer = strtok(NULL, " ");
                commandArgc++;//token counter
        }
}

/**
 *	destroys the arrays of chars, letting the user to input the next command line
 */
void destroyCommand()
{
        while (commandArgc != 0) {
                commandArgv[commandArgc] = NULL;                                 // delete the pointer to the string
                commandArgc--;                                                   // decrement the number of words in the command line
        }
        bufferChars = 0;                                                                          // erase the number of chars in the buffer
}

/*********************************************************************
 * FUNCTIONS REGARDING ACTIVE JOBS
 *********************************************************************/

/**
 *	inserts an active process in the linked list
 */
t_job* insertJob(pid_t pid, pid_t pgid, char* name, char* descriptor,
                 int status)
{
        usleep(10000);
        t_job *newJob = malloc(sizeof(t_job));

        newJob->name = (char*) malloc(sizeof(name));
        newJob->name = strcpy(newJob->name, name);
        newJob->pid = pid;
        newJob->pgid = pgid;
        newJob->status = status;
        newJob->descriptor = (char*) malloc(sizeof(descriptor));
        newJob->descriptor = strcpy(newJob->descriptor, descriptor);
        newJob->next = NULL;

        if (jobsList == NULL) {
                numActiveJobs++;
                newJob->id = numActiveJobs;
                return newJob;
        } else {
                t_job *auxNode = jobsList;
                while (auxNode->next != NULL) {
                        auxNode = auxNode->next;
                }
                newJob->id = auxNode->id + 1;
                auxNode->next = newJob;
                numActiveJobs++;
                return jobsList;
        }
}

/**
 * modifies the status of a job
 */
int changeJobStatus(int pid, int status)
{
        usleep(10000);
        t_job *job = jobsList;
        if (job == NULL) {
                return 0;
        } else {
                int counter = 0;
                while (job != NULL) {
                        if (job->pid == pid) {
                                job->status = status;
                                return TRUE;
                        }
                        counter++;
                        job = job->next;
                }
                return FALSE;
        }
}

/**
 *	deletes a no more active process from the linked list
 */
t_job* delJob(t_job* job)
{
        usleep(10000);
        if (jobsList == NULL)
                return NULL;
        t_job* currentJob;
        t_job* beforeCurrentJob;

        currentJob = jobsList->next;
        beforeCurrentJob = jobsList;

        if (beforeCurrentJob->pid == job->pid) {

                beforeCurrentJob = beforeCurrentJob->next;
                numActiveJobs--;
                return currentJob;
        }

        while (currentJob != NULL) {
                if (currentJob->pid == job->pid) {
                        numActiveJobs--;
                        beforeCurrentJob->next = currentJob->next;
                }
                beforeCurrentJob = currentJob;
                currentJob = currentJob->next;
        }
        return jobsList;
}

/**
 * searches a job in the active jobs list, by pid, job id, job status
 */
t_job* getJob(int searchValue, int searchParameter)
{
        usleep(10000);
        t_job* job = jobsList;
        switch (searchParameter) {
        case BY_PROCESS_ID:
                while (job != NULL) {
                        if (job->pid == searchValue)
                                return job;
                        else
                                job = job->next;
                }
                break;
        case BY_JOB_ID:
                while (job != NULL) {
                        if (job->id == searchValue)
                                return job;
                        else
                                job = job->next;
                }
                break;
        case BY_JOB_STATUS:
                while (job != NULL) {
                        if (job->status == searchValue)
                                return job;
                        else
                                job = job->next;
                }
                break;
        default:
                return NULL;
                break;
        }
        return NULL;
}

/**
 * prints the active processes launched by the shell
 */
void printJobs()
{
        printf("\nActive jobs:\n");
        printf(
                "---------------------------------------------------------------------------\n");
        printf("| %7s  | %30s | %5s | %10s | %6s |\n", "job no.", "name", "pid",
               "descriptor", "status");
        printf(
                "---------------------------------------------------------------------------\n");
        t_job* job = jobsList;
        if (job == NULL) {
                printf("| %s %62s |\n", "No Jobs.", "");
        } else {
                while (job != NULL) {
                        printf("|  %7d | %30s | %5d | %10s | %6c |\n", job->id, job->name,
                               job->pid, job->descriptor, job->status);
                        job = job->next;
                }
        }
        printf(
                "---------------------------------------------------------------------------\n");
}

/*********************************************************************
 * PROMPT AND ERROR FUNCTIONS
 *********************************************************************/

/**
 *	displays a welcome screen
 */
void welcomeScreen()
{
        printf("\n_________________________________________________________\n\n");
        printf("\t\tWelcome to R-Shell \n");  
        printf("_________________________________________________________\n");
        printf("\n\n");
}

/*
*
*	gets a prompt prefix from user to append to the current prompt. returns 1 if prefix is available
*
*/
int checkPrefix()
{
	if(prefix != NULL)	
		return 1;
}


/**
 *	displays a nice prompt for the shell
 */
void shellPrompt()
{	
		
	if(checkPrefix() == 1)
	{
		printf(" %s [.%s]> ",prefix,getcwd(currentDirectory, 1024));
	}
	else
	{	
		printf(" [.%s]> ",getcwd(currentDirectory, 1024));
	}
	return;
}
/************************************************************************************************************
*
*	warnload command
*
*************************************************************************************************************/

static void *mythread(void *param)
{

	double load[1]; 
    	// if second argument to the function is "0.0" then exit the thread

	//printf("Thread created\n");

	while (getloadavg(load, 1) != -1)
   	{
		if (warnload == 0.0)
		{
			warnloadflag = 0;
			pthread_exit(NULL);	  			
		}
		if (load[0] >= warnload)
		{
   			printf("\nWarning: Load level is %.2f \n", load[0]);
			//shellPrompt();
		}		
		sleep(5);
   	}
	
}

void warnLoadThreadCreate(double x)
{
	warnload = x;
  	pthread_create(&tid1, NULL, mythread, "Thread 1");
	warnloadflag = 1; 					// indicates a warnload thread is created.

}

void updateThread(double x)
{
	warnload = x;
}

//////////////////////////////////

void updateNoClobber()
{
	if(noclobber ==1)
	{
		noclobber = 0;	
	}
	else
	{
		noclobber = 1;	
	}
}
