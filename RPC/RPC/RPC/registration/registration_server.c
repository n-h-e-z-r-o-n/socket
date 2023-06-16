
#include "registration.h"

char **
registration_1_svc(record *argp, struct svc_req *rqstp)
{
	
	//create response variable and variables to hold the passed arguments
	static char * result;
	static char * status;
	static char serial_Number[50], Reg_Number[50], Name[100];

	printf("\nRequest received from client\n");
	strcpy(serial_Number,argp->serial_Number);
	strcpy(Reg_Number,argp->Reg_Number);
	strcpy(Name,argp->Name);

	printf("\tReceived Data:\n");
	printf("\t\tSerial Number - %s\n", serial_Number);
	printf("\t\tRegistration Number - %s\n", Reg_Number);
	printf("\t\tName - %s\n", Name);

	FILE *file;
	char line[100];
	char tempSerialNumber[50], tempRegNumber[50];
	int feed = 0;

	printf("\tchecking for duplicate data.\n");

	file = fopen("data.txt", "r"); // open file in read mode
	if (file == NULL) {
		feed = 0; // return 0 if file cannot be opened
	}
	while (fgets(line, 100, file) != NULL) {
		sscanf(line, "%s %s", tempSerialNumber, tempRegNumber); // read serial number and registration number from file
		if (strcmp(tempSerialNumber, serial_Number) == 0) {
		    fclose(file);
		    feed = 1; // return 1 if either serial number is a duplicate
		    printf("\tDuplicate serial number entered: \n");
		    result = "Duplicate Error: Serial number has been duplicated";
		}
		else if (strcmp(tempRegNumber, Reg_Number) == 0) {
		    fclose(file);
		    feed = 1; // return 1 if either serial number or registration number is a duplicate
		    printf("\tDuplicate registration number entered: \n");
		    result = "Duplicate Error: Registration number has been duplicated";
		}
	}
	fclose(file);

	if (feed == 0){
		printf("\tWriting client's data to text file\n");
		FILE *file;
		file = fopen("data.txt", "a"); // open file in append mode
		fprintf(file, "%s %s %s\n", serial_Number, Reg_Number, Name); // write details to file
		fclose(file);
		printf("\tDetails saved to file successfully.\n");
		result = "Details saved to file successfully";
	}


	return &result;

}


