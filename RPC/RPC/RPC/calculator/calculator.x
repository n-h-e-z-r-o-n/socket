struct operands{
	int operation;
	float op1;
	float op2;
};

program CALCULATE_PROG{
	version CALCULATE_VERS{
		float calculate(operands)=1;
	}=1;
}=0x23451111;
