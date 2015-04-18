Sangini Shah and Rob Williams
4/17/15
PA4- The Avengers' Multithreaded Bank
readme.txt

For project 4 we have implemented a multithreaded bank system that includes a client and a server. Implementation of the data structure designs 
and client/server designs are described in detail below.

**DATA STRUCTURE**
The Bank: bank.h bank.c

To implement the bank, two structs were used:
	1)Account
		The account struct contains the name of the account, the current balance of the account, whether or not it is inSession (being served)
		and a mutex lock to guarantee only one client can access an account at a time. Accounts can be 
			1) created (createAccount)- a new Account is initialized with the provided name and returned
			2) queried (queryAccount)- the currentBalance of the account is returned
			3) deposited (depositMoney)- the given amount of money is added to the currentBalance of the account
			4) withdrawn (withdrawMoney)- the given amount of money is subtracted from the currentBalance unless currentBalance falls below 0 
	2)Bank
		The bank struct contains a list of all accounts (max 20 in this situation), the number of active (present) accounts, and a mutex lock on the bank
		to allow for printing accounts and adding accounts without causing discrepancies in the data of the banks.
			1)printing (printAccounts)- the provided bank's mutex is locked so that no updates can be made to the bank during the print of the bank. All 
										accounts in the bank are traversed and their name, balance, and servicing status are printed. No lock is required to 
										be placed on the account since data is only being read from the account and not changed. On completion of print, the 
										bank's mutex is unlocked.
			2)adding account (addAccount)- the bank's mutex is locked so no other accounts can be added or the bank cannot be printed while adding an account. 
										   The bank is then checked to make sure that the given account name is not present in the bank already. If not, the 
										   account is added to the bank and then the bank's mutex is unlocked. 
		A removeAccount function was implemented for the sake of testing and debugging, however this functionality is not provided by the server.
	

**Client**	
The Client: client.c

The client serves as a customer for the bank. It has two simple threads that are used 
to communicate with the server:
	1)read_input thread: reads the input from the command line and sends data to the server
						 until the server closes or the user inputs quit. 
	2)print_output thread: reads the response sent from the server and outputs to the console.
						   This thread returns when the server is no longer in service or if quit
						   is called and server disconnects from the client. 
The main method is joined with the print_output thread and terminates on print_output's return


**Server**
The Server: server.c

The server uses the banking system implemented in bank.h and bank.c. It contains two threads as well:
	1)printBank thread: prints the bank every 20 seconds, using sleep(20) to wait on the print
	2)client_session_thread: reads the requests sent by the client and performs the corresponding bank actions.
							 Responses on success/failure are then returned to the client and also printed on the server side.
							 A new client_session_thread is spawned each time a new client connects to the server.
							 When a client attempts to serve an account, if the account is already in service, the thread
							will sleep and check every few seconds in an attempt to service the account.
Upon execution, a bank object is created and maintained throughout the duration of the server. 

***The server has also implemented the repeated 2second attempt at accessing an account if it is currently in use.In order to implement
   this functionality, the account struct being served is kept track of and is locked with a mutex. If being served, the mutex is locked for the 
   duration of service or until the client manipulating the account quits from the bank in which case the mutex is then unlocked so another client
   may access the account.
