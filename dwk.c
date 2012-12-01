// See LICENSE for copyright info

#include "config.h"
#include "dwk.h"
#include "draw.h"
#include "lnk.h"
#include "img.h"
#include "forms.h"
#include "parse.h"
#include "stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display *dpy;
Window win;
Pixmap *pics = NULL;
GC gc;
XGCValues xgcval;
Colormap colmap;
XFontStruct *xfs;
int lh; // line height

char *str = NULL;
int len;

int main( int argc, char *argv[] ) {
	if( argc < 2 ) {
		printf( "protip: ./dwk <file>\n" );
		return 0;
	}

	dwk_init( );

	FILE *fp = fopen( argv[1], "r" );
	fseek( fp, 0, SEEK_END );
	len = ftell( fp );
	fseek( fp, 0, SEEK_SET );
	str = realloc( str, len+1 );
	int i; for( i = 0; i < len; i++ )
		str[i] = fgetc( fp );
	str[len] = 0;
	fclose( fp );

	parse( );

	XEvent ev;
	for( ;; ) {
		XNextEvent( dpy, &ev );
		switch( ev.type ) {
		case Expose:
			draw_screen( );
			break;
		case ButtonPress:
			dwk_close( );
			return 0;
		}
	}

	return 0;
}

void dwk_init( ) {
	init_lvec( );
	init_ivec( );
	init_fvec( );

	char *dpy_env = getenv( "DISPLAY" );
	if( dpy_env ) dpy = XOpenDisplay( dpy_env );
	else dpy = XOpenDisplay( NULL );
	if( !dpy ) dwk_err( "cannot connect to display" );

	win = XCreateSimpleWindow( dpy, DefaultRootWindow( dpy ), 0, 0, 200, 200, 2, WhitePixel( dpy, DefaultScreen( dpy ) ), BlackPixel( dpy, DefaultScreen( dpy ) ) );

	pics = realloc( pics, sizeof( Pixmap ) );

	gc = XCreateGC( dpy, win, GCForeground|GCLineWidth|GCLineStyle, &xgcval );
	colmap = DefaultColormap( dpy, DefaultScreen( dpy ) );
	ch_col( NORMFG, NORMBG );

	xfs = XLoadQueryFont( dpy, DWK_FONT );
	XSetFont( dpy, gc, xfs->fid );
	lh = xfs->ascent+xfs->descent;

	XSelectInput( dpy, win, ButtonPressMask | ExposureMask );
	XMapWindow( dpy, win );
}

void dwk_close( ) {
	free( str );
	free_ivec( );
	free_lvec( );
	free_fvec( );
	XCloseDisplay( dpy );
}

void draw_screen( ) {
	int i, j;
	extern int nimg, nlnk;

	for( i = j = 0; i < nimg; i++ )
		j = draw_img( i, TXT_COL_WIDTH+20, j+lh );

	for( i = j = 0; i < nlnk; i++ ) {
		char title[get_lnk_len( i )+9];
		sprintf( title, "%s [[%i]]", get_lnk( i ), i );
		j = draw_text( title, TXT_COL_WIDTH+20+IMG_COL_WIDTH+20, j+lh );
	}

	draw_text( str, 10, 20 );
}
