// See LICENSE for copyright info

#include "config.h"
#include "dwk.h"
#include "stuff.h"
#include <string.h>
#include <stdio.h>
#include <X11/Xlib.h>

extern Display *dpy;
extern Window win;
extern GC gc;
extern Colormap colmap;
extern XFontStruct *xfs;
extern int lh; // line height

extern XImage **ivec;
extern char **invec;
extern int nimg;

int draw_text( const char *_txt, int _x, int _y ) {
	ch_col( NORMFG, NORMFG );

	int x = _x, y = _y;
	int i, j, k, l = strlen( _txt );
	char txt[l+1]; strcpy( txt, _txt );
	for( i = j = k = 0; i < l; ) {
		if( txt[i] == '[' || txt[i] == '<' ) {
			char c = txt[i];

			memmove( txt+i, txt+i+1, l-i-1 );
			txt[l--] = 0;
			if( txt[i] != c ) {
				XDrawString( dpy, win, gc, x, y, txt+j, i-j );
				x += XTextWidth( xfs, txt+j, i-j );
				j = i;

				if( c == '[' )
					ch_col( LINKFG, LINKBG );
				else if( c == '<' )
					ch_col( IMGFG, IMGBG );
			} else i++;
			continue;
		} if( txt[i] == ']' || txt[i] == '>' ) {
			char c = txt[i];

			memmove( txt+i, txt+i+1, l-i-1 );
			txt[l--] = 0;

			if( txt[i] != c ) {
				XDrawImageString( dpy, win, gc, x, y, txt+j, i-j );
				x += XTextWidth( xfs, txt+j, i-j );
				j = i;
				ch_col( NORMFG, NORMBG );
			} else i++;
			continue;
		}

		if( txt[i] == '\n' ) {
			XDrawString( dpy, win, gc, x, y, txt+j, i-j );
			x = _x; y += 2*lh;
			k = j = i+1;
			i++;
			continue;
		} i++;
		if( XTextWidth( xfs, txt+j, i-j ) >= TXT_COL_WIDTH ) {
			XDrawString( dpy, win, gc, x, y, txt+j, k-j );
			x = _x; y += lh;
			j = k+1;
		}
		if( txt[i] == ' ' ) k = i;
	}
	XDrawString( dpy, win, gc, x, y += lh, txt+j, i-j );
	return y;
}

int draw_img( int _index, int _x, int _y ) {
	if( _index > nimg ) return _y;
	XPutImage( dpy, win, gc, ivec[_index], 0, 0, _x, _y, ivec[_index]->width, ivec[_index]->height );

	char imgtitle[strlen( invec[_index] )+9]; // 9 = ' ' + 2x'<' + up to 3 nums + 2x'>' + '\0'
	sprintf( imgtitle, "%s <<%i>>", invec[_index], _index );
	_y = draw_text( imgtitle, _x+20, _y+ivec[_index]->height )+lh;
	return _y;
}

void ch_col( const char *_fg, const char *_bg ) {
	XColor col;
	if( XAllocNamedColor( dpy, colmap, _fg, &col, &col ) )
		XSetForeground( dpy, gc, col.pixel );
	if( XAllocNamedColor( dpy, colmap, _bg, &col, &col ) )
		XSetBackground( dpy, gc, col.pixel );
}

void ch_fgc( int _r, int _g, int _b ) {
	static XColor col; // static cuz called a million times in a short time
	static char colstr[8];
	sprintf( colstr, "#%02x%02x%02x", _r, _g, _b );	
	if( XAllocNamedColor( dpy, colmap, colstr, &col, &col ) )
		XSetForeground( dpy, gc, col.pixel );
}
