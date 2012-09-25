// See LICENSE for copyright info

#define STRBUFLEN 512

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

extern XImage **img_arr;
extern char **imgname_arr;
extern int nimg;

char *str;

int main( int argc, char *argv[] ) {
	if( argc < 2 ) {
		printf( "protip: ./dwk <file>\n" );
		return 0;
	}

	dwk_init( );

	parse( argv[1] );

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
	str = malloc( 1 );

	img_arr = malloc( sizeof( XImage * ) );
	imgname_arr = malloc( sizeof( char * ) );
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

void dwk_close( ) {
	free( str );
	free_ia( );
	XCloseDisplay( dpy );
}

void parse( const char *_file ) {
	FILE *fp = fopen( _file, "r" );

	fseek( fp, 0, SEEK_END );
	int len = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	str = realloc( str, len+1 );
	char c, n = fgetc( fp );

	int slen = 0;
	char *save = malloc( STRBUFLEN+1 );
	char slnk = 0, simg = 0;

	int i, pos; for( i = pos = 0; i < len; i++, pos++ ) {
		c = n;
		if( i == len-1 ) n = 0;
		else n = fgetc( fp );

		// handle links
		if( c == ']' && n != ']' ) {
			slnk = 0;
			save[slen] = 0;
			printf( "%s\n", save );
			save = realloc( save, STRBUFLEN );
		}
		if( c == '[' && n != '[' ) {
			slnk = 1;
			slen = 0;
		} else if( slnk ) {
			save[slen] = c;
			slen++;
			if( slen % STRBUFLEN == 1 )
				save = realloc( save, slen+STRBUFLEN+1 );
		}

		// handle imgs
		if( c == '>' && n != '>' ) {
			simg = 0;
			save[slen] = 0;
			int index = load_img( save );
			if( index < 0 ) dwk_err( "failed loading image" );
			save = realloc( save, STRBUFLEN );
		}
		if( c == '<' && n != '<' ) {
			simg = 1;
			slen = 0;
		} else if( simg ) {
			save[slen] = c;
			slen++;
			if( slen % STRBUFLEN == 1 )
				save = realloc( save, slen+STRBUFLEN+1 );
		}

		str[pos] = c;
	}
	str[pos] = 0;

	fclose( fp );

	free( save );
}

void draw_screen( ) {
	int i, j; for( i = j = 0; i < nimg; i++ )
		j = draw_img( i, TXT_COL_WIDTH+20, j+10 );
	draw_text( str, 10, 10 );
}

void dwk_err( const char *_err ) {
	int i, l = strlen( _err ); for( i = 0; i < l; i++ )
		fputc( _err[i], stderr );
	fputc( '\n', stderr ); fputc( '\0', stderr );
	exit( 1 );
}
