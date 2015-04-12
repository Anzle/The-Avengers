

typedef enum {false, true} bool;

typedef struct{
	char* name[101]; //max length 100
	float currentBalance; //String formater: %2.2f 
	bool inSession;
} Account;


typedef struct {
	pthread_mutex_lock lock; //lock for printing/adding/removing accounts
	Account accounts[20]; //Bank Accounts
	int activeAccounts; //keep the number of active accounts
} Bank;


int
buildDaBank(Bank * daBank){
	if(pthread_mutex_init(&dabank->lock, 0 ) > 0 ) //initialize the mutex
	{	printf("Couldn't initialize the bank mutex.\n")
		return 1;//failure
	}
	dabank->activeAccounts = 0;
	return 0; //success
}


int
createAccount(Bank * daBank, char * aName){
	if(!daBank)
	{
		printf("This is not a Bank!\n");
		return 1; // failure
	}
	if(!aName){
		printf("This is not a Name!\n");
		return 2;
	}
	
	phread_mutex_lock( & dabank->lock );
	
}