#include <stdio.h>

int add(int *reg_a, int *reg_b) {

    int pulse=0;
    int setflag=0;
    int carry=0;

    for (pulse=0; pulse < 10; pulse++) {
	printf("Pulse Count = %d : ",pulse);
	(*reg_a)++;

	if (setflag)
	    (*reg_b)++;

	if (*reg_b == 10) {
	    *reg_b = 0;
	    carry = 1;
	}

	if (*reg_a == 10) {
	    *reg_a = 0;
	    setflag = 1;
	}


	printf("reg_a = %d : reg_b = %d\n",*reg_a,*reg_b);

	}

    return carry;
}


void main (void) {

    int tape_01=3;
    int tape_02=8;
    int reg_10=0;
    int reg_20=0;
    int carry=0;

    carry=add (&tape_01,&reg_10);
    printf("tape_01 = %d : reg_10 = %d : carry = %d\n",tape_01,reg_10,carry);

    carry=add (&tape_02,&reg_20);
    printf("tape_02 = %d : reg_20 = %d : carry = %d\n",tape_02,reg_20,carry);

    carry=add (&reg_10,&reg_20);
    printf("reg_10 = %d : reg_20 = %d : carry = %d\n",reg_10,reg_20,carry);

}