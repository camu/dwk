// See LICENSE for copyright info

#include "config.h"
#include "div.h"
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

extern XImage **img_arr;
extern int nimg;

Div *all;
int ndiv;

int main( int argc, char *argv[] ) {
	if( argc < 3 ) {
		printf( "protip: ./dwk <png file> <text file>\n" );
		return 0;
	}

	dwk_init( );

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

	ch_txt( lis, &lipsum );

	int index = load_img( argv[1] );
//	if( index < 0 ) dwk_err( "failed loading image" );
	ch_img( index, &lipsum );

	add_div( &lipsum );

	XEvent ev;
	for( ;; ) {
		XNextEvent( dpy, &ev );
		switch( ev.type ) {
		case Expose:
			draw_screen( );
			break;
		case ButtonPress:
			free_div( &lipsum );
			free_ia( );
			XCloseDisplay( dpy );
			return 0;
		}
	}

	return 0;
}

void dwk_init( ) {
	all = malloc( 1 );
	ndiv = 0;

	img_arr = malloc( sizeof( XImage * ) );
	nimg = 0;

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

void add_div( void *_div ) {
	Div *div = _div;
	all = realloc( all, sizeof( Div )*(++ndiv) );
	all[ndiv-1] = *div;
}

void draw_screen( ) {
	const int a = sizeof( Div );
	int i, j; for( i = j = 0; i < ndiv; i++ ) {
		if( all[i*a].imgind != -1 )
			j = draw_img( all[i*a].imgind, 10, j );
		if( all[i*a].txt != NULL )
			j = draw_text( all[i*a].txt, 10, j, LINE_WIDTH )+10;
	}
}

void dwk_err( const char *_err ) {
	int i, l = strlen( _err ); for( i = 0; i < l; i++ )
		fputc( _err[i], stderr );
	fputc( '\n', stderr ); fputc( '\0', stderr );
	exit( 1 );
}
