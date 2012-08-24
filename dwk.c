// See LICENSE for copyright info

#include "config.h"
#include "dwk.h"
#include "draw.h"
#include "img.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display *dpy;
Window win;
Pixmap *pics;
GC gc;
XGCValues xgcval;
Colormap colmap;
XFontStruct *xfs;
int lh; // line height

Div *all;
int ndiv;

int main( int argc, char *argv[] ) {
	if( argc < 3 ) {
		printf( "protip: ./dwk <png file> <text file>\n" );
		return 0;
	}

	dwk_init( );

	XImage *img = NULL;
	XImage **imgp = &img;
	load_png( argv[1], imgp );
	if( img == NULL ) dwk_err( "failed loading image" );

	Div lipsum = create_div( );
	FILE *lif = fopen( argv[2], "r" );
	fseek( lif, 0, SEEK_END );
	int len = ftell( lif );
	fseek( lif, 0, SEEK_SET );
	char lis[len+1];
	int i; for( i = 0; i < len; i++ )
		lis[i] = fgetc( lif );
	lis[len] = 0;
	fclose( lif );
	div_add_wid( &lipsum, create_wid( lis ) );
	add_div( lipsum );

	XEvent ev;
	for( ;; ) {
		XNextEvent( dpy, &ev );
		switch( ev.type ) {
		case Expose:
			draw_screen( );
XPutImage( dpy, win, gc, img, 0, 0, 0, 0, 800, 600 );

			break;
		case ButtonPress:
			XCloseDisplay( dpy );
XDestroyImage( img );
			return 0;
		}
	}

	return 0;
}

void dwk_init( ) {
	all = malloc( 1 );
	ndiv = 0;


	char *dpy_env = getenv( "DISPLAY" );
	if( dpy_env ) dpy = XOpenDisplay( dpy_env );
	else dpy = XOpenDisplay( NULL );
	if( !dpy ) dwk_err( "cannot connect to display" );

	win = XCreateSimpleWindow( dpy, DefaultRootWindow( dpy ), 0, 0, 200, 200, 2, WhitePixel( dpy, DefaultScreen( dpy ) ), BlackPixel( dpy, DefaultScreen( dpy ) ) );

	pics = malloc( sizeof( Pixmap ) );

	gc = XCreateGC( dpy, win, GCForeground|GCLineWidth|GCLineStyle, &xgcval );
	colmap = DefaultColormap( dpy, DefaultScreen( dpy ) );
	ch_col( NORMFG, NORMBG );

	xfs = XLoadQueryFont( dpy, DWK_FONT );
	XSetFont( dpy, gc, xfs->fid );
	lh = xfs->ascent+xfs->descent;

	XSelectInput( dpy, win, ButtonPressMask | ExposureMask );
	XMapWindow( dpy, win );
}

void add_div( Div _div ) {
	all = realloc( all, sizeof( Div )*(++ndiv) );
	all[ndiv-1] = _div;
}

void draw_screen( ) {
	int i, j; for( i = j = 0; i < ndiv; i++ ) {
		if( all[i].nwid == 1 )
			j = draw_text( all[i].wid[0].txt, 10, j, LINE_WIDTH )+10;
	}
}

void dwk_err( const char *_err ) {
	int i, l = strlen( _err ); for( i = 0; i < l; i++ )
		fputc( _err[i], stderr );
	fputc( '\n', stderr ); fputc( '\0', stderr );
	exit( 1 );
}
