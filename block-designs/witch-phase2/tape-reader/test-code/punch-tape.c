#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "punch-tape.h"

int letter_shift=0;
int new_line=0;

void print_bits(char code_id) {
int x=0;

    for (x=4; x>=0 ; x--) {
	int bit=((codes[code_id].code_val&(1<<x))>>x);
	if (bit == 1)
	    printf("*");
	else
	    printf("-");
    }
    printf("\n");
}

int find_code(char code_id) {
    int x=0;

    for (x=0; x<53 ; x++) {
	if (code_id == codes[x].code_id) 
	    return x;
    }

    return -1;
}



void main (void) {
    int code_number=0;
    int c;
    FILE *file;
    file = fopen("test.txt", "r");
    if (file) {
	while ((c = getc(file)) != EOF) {
		code_number = find_code(c);
		if ( code_number >= 0 ) {
			if (new_line) {
			    printf("SP   ");
			    print_bits(find_code(' '));
			    new_line=0;
			}
		    printf("%c    ", c);
		    print_bits(code_number);
		}
		else {
		    if ( c == 0x0a) {
			printf("\n");
			new_line=1;
		    }
		    else if ( c == '>' ) {
			printf("\n");
			c = getc(file);
			printf("BLK%c ",c);
			print_bits(c-0x30);
			c = getc(file);
			new_line=0;
		    }
		    else if ( c == '=' ) {
			printf("\n\n\n--------------\n\n\n");
			c = getc(file);
		    }
		}
	}
	fclose(file);
    }


return;

}
