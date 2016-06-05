#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "punch-tape.h"
#include <stdio.h>
#include <math.h>
#include <cairo.h>
#include <cairo-ps.h>
#include <cups/cups.h>

#if 0
/* 3" x 42" Tape */
#define WIDTH  216
#define HEIGHT 3456
#else
/* 8.5" x 11 Letter */
#define WIDTH  612
#define HEIGHT 792
#endif

#define HMARGIN 60
#define VMARGIN 100
#define DSPACING 27

#define DEBUG 0

int letter_shift=0;
int new_line=0;
unsigned int dotrows=0;

void print_text(cairo_t *cr,char *prnt_txt) {
    cairo_select_font_face(cr, 
                         "Arial Black", 
                         CAIRO_FONT_SLANT_NORMAL, 
                         CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 9);
    cairo_move_to(cr, (HMARGIN-40), (VMARGIN+(DSPACING*dotrows))+3);
    cairo_show_text(cr, prnt_txt); // the text we got as a parameter
    cairo_fill(cr);
}


void print_bits(unsigned int code_id,cairo_t *cr) {
    int x=0;
    int y=5;

    for (x=4; x>=0 ; x--) {
	unsigned int bit=((codes[code_id].code_val&(1<<x))>>x);
	if (y==3) {
	    cairo_arc(cr, HMARGIN+(DSPACING*y), VMARGIN+(DSPACING*dotrows), 6, 0, 2*M_PI);
	    cairo_fill(cr);
	    y--;
	}
	if (bit == 1) {
	    cairo_arc(cr, HMARGIN+(DSPACING*y), VMARGIN+(DSPACING*dotrows), 9, 0, 2*M_PI);
	    cairo_fill(cr);
#if DEBUG
	    printf("*");
	} else
	    printf("-");
#else
	}
#endif
	y--;
    }

#if DEBUG
    printf("\n");
#endif

    dotrows++;
}

unsigned int find_code(unsigned int code_id) {
    int x=0;

    for (x=0; x<53 ; x++) {
	if (code_id == codes[x].code_id) 
	    return x;
    }

    return -1;
}



int main (void) {
    int code_number=0;
    int c;
    char tmptext[10];
    FILE *file;
    file = fopen("test.txt", "r");

    // setup
    char* tmpfilename = tempnam(NULL,NULL);
    cairo_surface_t* surface = cairo_ps_surface_create(tmpfilename, 
                                                     WIDTH, 
                                                     HEIGHT);
    cairo_t *context = cairo_create(surface);

    if (file) {
	while ((c = getc(file)) != EOF) {
		code_number = find_code(c);
		if ( code_number >= 0 ) {
			if (new_line) {
#if DEBUG
			    printf("SP   ");
#endif
			    print_text(context,"SP");
			    print_bits(find_code(' '),context);
			    new_line=0;
			}

#if DEBUG
		    printf("%c    ", c);
#endif

		    sprintf(tmptext, "%c    ", c);
		    print_text(context,tmptext);
		    print_bits(code_number,context);
		}
		else {
		    if ( c == 0x0a) {
#if DEBUG
			printf("\n");
#endif
			new_line=1;
		    }
		    else if ( c == '>' ) {
			c = getc(file);
#if DEBUG
			printf("\n");
			printf("BLK%c ",c);
#endif
			sprintf(tmptext,"BLK%c ",c);
			print_text(context,tmptext);
			print_bits(c-0x30,context);
			c = getc(file);
			new_line=0;
		    }
		    else if ( c == '=' ) {
#if DEBUG
			printf("\n\n\n--------------\n\n\n");
#endif
			c = getc(file);
		    }
		}
	}
	fclose(file);
    }

    // finish up
    cairo_show_page(context);
    cairo_destroy(context);
    cairo_surface_flush(surface);
    cairo_surface_destroy(surface);

    // print
    cupsPrintFile(cupsGetDefault(), tmpfilename, "cairo PS", 0, NULL);
    unlink(tmpfilename);


return 0;

}
