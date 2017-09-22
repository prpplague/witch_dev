#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "punch-tape.h"
#include <stdio.h>
#include <math.h>
#include <cairo.h>
#include <cairo-ps.h>
#include <cups/cups.h>
#include <png.h>

#if 1
/* 3" x 42" Tape */
#define WIDTH  216
#define HEIGHT 3456
#else
/* 8.5" x 11 Letter */
#define WIDTH  612
#define HEIGHT 792
#endif

#define HMARGIN 13
#define VMARGIN 100
#define DSPACING 27

#define DEBUG 0

int letter_shift=0;
int new_line=0;
unsigned int dotrows=0;



typedef struct {
    unsigned char * data;
    int height;
    int width;
    int pixel_bytes;
} bitmap_t;

/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
   success, non-zero on error.  This is based on the code at
   'http://www.lemoda.net/png/c-write-png/' */

int save_png_to_file (const bitmap_t * bitmap, const char *path)
{
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte ** row_pointers = NULL;
    int status = -1;
    int depth = 8;
    
    fp = fopen (path, "wb");
    if (! fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }
    
    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }
    
    /* Set up error handling. */

    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    }
    
    /* Set image attributes. */

    png_set_IHDR (png_ptr,
                  info_ptr,
                  bitmap->width,
                  bitmap->height,
                  depth,
                  PNG_COLOR_TYPE_RGBA,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);
    
    /* Initialize rows of PNG. */

    row_pointers = malloc (bitmap->height * sizeof (png_byte *));
    for (y = 0; y < bitmap->height; ++y) {
        row_pointers[y] = bitmap->data
                        + bitmap->width * bitmap->pixel_bytes * y;
    }
    
    /* Write the image data to "fp". */

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */

    status = 0;
    
    free (row_pointers);
    
 png_failure:
 png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
 png_create_write_struct_failed:
    fclose (fp);
 fopen_failed:
    return status;
}


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
    bitmap_t bitmap;
    char * fname = "file.png";

    // setup
//    char* tmpfilename = tempnam(NULL,NULL);
//    cairo_surface_t* surface = cairo_ps_surface_create(tmpfilename, 
//                                                     WIDTH, 
//                                                     HEIGHT);

    cairo_surface_t* surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
    cairo_t *context = cairo_create(surface);

    if (file) {
	while ((c = getc(file)) != EOF) {
		code_number = find_code(c);
		if ( code_number >= 0 ) {
			if (new_line) {
#if DEBUG
			    printf("SP   ");
#endif
//			    print_text(context,"SP");
			    print_bits(find_code(' '),context);
			    new_line=0;
			}

#if DEBUG
		    printf("%c    ", c);
#endif

		    sprintf(tmptext, "%c    ", c);
//		    print_text(context,tmptext);
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
//			print_text(context,tmptext);
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

    bitmap.data = cairo_image_surface_get_data (surface);
    bitmap.pixel_bytes = 4;
    bitmap.width = WIDTH;
    bitmap.height = HEIGHT;
    save_png_to_file (& bitmap, fname);



    cairo_surface_destroy(surface);

    // print
//    cupsPrintFile(cupsGetDefault(), tmpfilename, "cairo PS", 0, NULL);
//    unlink(tmpfilename);


return 0;

}
