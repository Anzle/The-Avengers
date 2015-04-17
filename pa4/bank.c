#include "bank.h"

//-------------Account functions---------------------

int
createAccount(Account * deAccount, char * aName){
	if(!deAccount){
		printf("%s Invalid Account: \n", ACCERR) ;
		return -1 ; // failure
	}

	if(!aName || strlen(aName) > NAMLEN){
		printf("%s Invalid Name: %s\n", ACCERR, aName) ;
		return -2 ; // failure
	}
	
	strcpy(deAccount->name, aName) ;
	deAccount->currentBalance = 0 ;
	deAccount->inSession = FALSE ; 
	
	return 0 ; // success
}

int
queryAccount(Account * deAccount){
	if(!deAccount){
		printf("%s Invalid Account: \n", ACCERR) ;
		return 0 ; 
	}
	
	return deAccount->currentBalance ;
}

int
depositMoney(Account * deAccount, float amount){
	if(!deAccount){
		printf("%s Invalid Account: \n", ACCERR) ;
		return 0 ; 
	} 
	
	if(amount < 0)
		return deAccount->currentBalance ;
	
	deAccount->currentBalance += amount ;
	
	return deAccount->currentBalance ;
}

int
withdrawMoney(Account * deAccount, float amount){
	if(!deAccount){
		printf("%s Invalid Account: \n", ACCERR) ;
		return 0 ; 
	}
	if(amount < 0)
		return deAccount->currentBalance ;
	
	deAccount->currentBalance -= amount ;
	
	return deAccount->currentBalance ;
}

 // ----------Bank Functions---------------------

int
buildDaBank(Bank * daBank){
	if(!daBank)	{
		printf("%s NULL Bank given", BNKERR) ;
		return -1 ; //failure
	}
		
	if(pthread_mutex_init(&daBank->lock, 0 ) > 0 ) //initialize the mutex
	{	printf("%s Couldn't initialize the Bank MUTEX\n", BNKERR) ;
		return -2 ; //failure
	}
	
	for(int i=0 ; i < ACCNUM ; i++){
		daBank->accounts[i].name[0] = '\0' ;
		if(pthread_mutex_init(&daBank->accounts[i].lock, 0 ) > 0 ) //initialize the mutex
		{	printf("%s Couldn't initialize the Account MUTEX\n", ACCERR) ;
			return -3 ; //failure
		}
	}
	daBank->activeAccounts = 0 ;
	return 0 ; // success
}

int
addAccount(Bank * daBank, char * aName){
	if(!daBank){
		printf("%s NULL Bank\n", BNKERR) ;
		return -1 ; // failure
	}
	if(!aName){
		printf("%s Invalid Name: %s\n", ACCERR, aName) ;
		return -2 ; // failure
	}
	
	pthread_mutex_lock( & daBank->lock ); // lock so no-one can interfere
	
	if(daBank->activeAccounts >= ACCNUM){
		printf("%s There are already %d accounts in the bank\n", BNKERR, daBank->activeAccounts) ;
		pthread_mutex_unlock(& daBank->lock) ;
		return -3 ; // failure
	}
	
	int emptySpot = -1; // let's multi-task
	
	for(int i=0; i < ACCNUM; i++){
		if (strcmp(aName, daBank->accounts[i].name) == 0 ){
			printf("%s There is already an account with the name %s\n", ACCERR, aName) ;
			pthread_mutex_unlock(& daBank->lock) ;
			return -4 ; // failure
		}
		
		if(daBank->accounts[i].name[0] == '\0') // find the last open position
			emptySpot = i ;
	}
	
	emptySpot = createAccount(& daBank->accounts[emptySpot], aName) ; // success 0 or failure -1/-2
	
	daBank->activeAccounts = (emptySpot < 0) ? daBank->activeAccounts : daBank->activeAccounts + 1; 
	
	pthread_mutex_unlock(& daBank->lock) ;
	
	return emptySpot ; // success 0 or failure -1 / -2
}

int
findAccount( Bank * daBank, char * aName){
	if(!daBank){
		printf("%s NULL Bank\n", BNKERR) ;
		return -1 ; // failure
	}
	if(!aName){
		printf("%s Invalid Name: %s\n", ACCERR, aName) ;
		return -2 ; // failure
	}
	
	int position = -3 ;
	
	for(int i=0; i < ACCNUM; i++){
		if (strcmp(aName, daBank->accounts[i].name) == 0 ){
			position = i ;
			break ; 
		}
	}
	
	return position ; // success position or failure -1 
}

//This was created because I like to add and remove things
//It is NOT required by the program specifications
int
removeAccount(Bank * daBank, char * aName){
	if(!daBank){
		printf("%s NULL Bank\n", BNKERR) ;
		return -1 ; // failure
	}
	
	if(!aName){
		printf("%s Invalid Name: %s\n", ACCERR, aName) ;
		return -2 ; // failure
	}
	
	pthread_mutex_lock( & daBank->lock ); // lock so no-one can interfere

	int position = -1 ;

	if( (position = findAccount(daBank, aName)) < 0 ){
		printf("%s No Account with Name: %s\n", ACCERR, aName) ;
		pthread_mutex_unlock(& daBank->lock) ;
		return -3 ; // failure
	}
		
	if ( daBank->accounts[position].inSession == TRUE ){
		printf("%s Account %s currently being served and can not be deleted\n", ACCERR, aName);
		pthread_mutex_unlock(& daBank->lock) ;
		return -4 ; // failure 
	}
	
	daBank->accounts[position].name[0] = '\0' ; 
	
	pthread_mutex_unlock(& daBank->lock) ;
	
	return 0 ; // success
}

int
printAccounts(Bank * daBank){
	if(!daBank){
		printf("%s NULL Bank\n", BNKERR) ;
		return -1 ; // failure
	}
	
	//Wrap this for multi-threading
	
	pthread_mutex_lock(& daBank->lock) ; // lock so no-one can interfere
	
	for(int i = 0; i < ACCNUM; i++){
		if(daBank->accounts[i].name[0] == '\0')
			continue ;
		printf("%s \t %2.2f ", daBank->accounts[i].name, daBank->accounts[i].currentBalance) ;
		if(daBank->accounts[i].inSession == TRUE)
			printf("\t IN SERVICE\n") ;
		else
			printf ("\t \n");
	}
	
	pthread_mutex_unlock(& daBank->lock) ;
	
	//Wrap this for multi-threading
	
	return 0; // success
}

