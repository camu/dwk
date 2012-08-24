// See LICENSE for copyrite stuff

#include "stuff.h"
#include "img.h"
#include "draw.h"
#include <png.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

extern Display *dpy;
extern Window win;
extern GC gc;

int load_img( const char *_file, void *_ximg ) {
	int a = filext( _file );
	char type[strlen( _file )-a+1];
	strcpy( type, _file+a+1 );

	if( strcmp( type, "png" ) == 0 )
		return load_png( _file, _ximg );
	else return 1;
}

int load_png( const char *_file, void *_ximg ) {
	FILE *img = fopen( _file, "rb" );
	if( img == NULL ) return 2;

	char head[8];
	fread( head, 1, 8, img );
	if( png_sig_cmp( (png_const_bytep )head, 0, 8 ) )
		return 3;

	png_structp pngp = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	png_infop infop = png_create_info_struct( pngp );
	setjmp( png_jmpbuf( pngp ) );

	png_init_io( pngp, img );
	png_set_sig_bytes( pngp, 8 );

	png_read_png( pngp, infop, PNG_TRANSFORM_IDENTITY, NULL );

	if( png_get_color_type( pngp, infop ) != PNG_COLOR_TYPE_RGB )
		return 4;

	int w = png_get_image_width( pngp, infop );
	int h = png_get_image_height( pngp, infop );
//	int bpp = png_get_bit_depth( pngp, infop );

	png_bytep *rowp = png_get_rows( pngp, infop );

	char *data = malloc( w*h*4 );
	char *pixl = data;
	int i, j; for( i = 0; i < h; i++ ) {
		png_byte *row = rowp[i];
		for( j = 0; j < w; j++ ) {
			png_byte *pixel = row+j*3;
			pixl[2] = pixel[0];
			pixl[1] = pixel[1];
			pixl[0] = pixel[2];
			pixl += 4;
		}
	}

	XImage **ximg = _ximg;
	*ximg = XCreateImage( dpy, DefaultVisual( dpy, DefaultScreen( dpy ) ), 24, ZPixmap, 0, data, w, h, 32, 0 );

	png_destroy_read_struct( &pngp, &infop, NULL );
	fclose( img );

	return 0;
}
