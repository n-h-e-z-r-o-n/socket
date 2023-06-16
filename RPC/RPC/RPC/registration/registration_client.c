#include "registration.h"


void
registration_prog_1(char *host, char *serial_no, char *reg_no, char *name)
{
	CLIENT *clnt;
	char * *result_1;
	record  registration_1_arg;

#ifndef	DEBUG
	printf("\n\tConnect to server");
	clnt = clnt_create (host, REGISTRATION_PROG, REGISTRATION_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	//assign parameter values to be passed
	strcpy(registration_1_arg.serial_Number,serial_no);
	strcpy(registration_1_arg.Reg_Number,reg_no);
	strcpy(registration_1_arg.Name,name);
	printf("\n\tCall registration funtion on server");

	result_1 = registration_1(&registration_1_arg, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "server call failed");
	}
	else
	{
		printf("\n\tReceive reply from server");
		printf("\n%s\n",*result_1);
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	int ch;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	
	do{
		char serial_no[50];
		char reg_no[50];
		char name[100];
		printf("\n Enter Student details for registration\n");
		printf("\n\t 1. Enter Serial Number  :  ");
		scanf("%s", serial_no);

		printf("\n\t 2. Enter Reg_Number     :  ");
		scanf("%s", reg_no);

		printf("\n\t 3. Enter Full Name      :  ");
		scanf("%s", name);
		printf("\n\t   \n");

		printf("\n\tCalling registration function stub");
		registration_prog_1 (host, serial_no, reg_no, name);
		printf("\nTry again: (1- Yes/0- No) :: ");
		scanf("%d",&ch);
	}while(ch==1);
exit (0);
}


