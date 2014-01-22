#include <stdio.h>

int add(int reg_a, int reg_b) {

    int pulse=0;
    int setflag=0;

	for (pulse=0; pulse < 10; pulse++) {
		printf("Pulse Count = %d : ",pulse);
		reg_a++;
		if (reg_a == 10) {
		    setflag = 1;
		    reg_a = 0;
		}

		if (setflag)
		    reg_b++;

		printf("reg_a = %d : reg_b = %d\n",reg_a,reg_b);

	}

    return 0;
}


void main (void) {

int tape_01=4;
int reg_10=0;
int reg_20=0;


add (tape_01,reg_10);


}