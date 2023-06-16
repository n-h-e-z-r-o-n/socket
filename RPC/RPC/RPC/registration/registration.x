struct record{
	char serial_Number[50];
      	char Reg_Number[50];
      	char Name[100];
};

program REGISTRATION_PROG{
	version REGISTRATION_VERS{
		string registration(record)=1;
	}=1;
}=0x23451111;
