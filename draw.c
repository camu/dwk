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

extern XImage **img_arr;
extern char **imgname_arr;
extern int nimg;

int draw_text( const char *_txt, int _x, int _y ) {
	int x = _x, y = _y+lh;
	int i, j, k, l = strlen( _txt );
	char txt[l+1]; strcpy( txt, _txt );
	for( i = j = k = 0; i < l; ) {
		if( txt[i] == '[' ) {
			memmove( txt+i, txt+i+1, l-i-1 );
			txt[l--] = 0;

			if( txt[i] != '[' ) {
				XDrawString( dpy, win, gc, x, y, txt+j, i-j );
				x += XTextWidth( xfs, txt+j, i-j );
				j = i;
				ch_col( LINKFG, LINKBG );
			}
			continue;
		} if( txt[i] == ']' ) {
			memmove( txt+i, txt+i+1, l-i-1 );
			txt[l--] = 0;

			if( txt[i] != ']' ) {
				XDrawImageString( dpy, win, gc, x, y, txt+j, i-j );
				x += XTextWidth( xfs, txt+j, i-j );
				j = i;
				ch_col( NORMFG, NORMBG );
			}
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
	XPutImage( dpy, win, gc, img_arr[_index], 0, 0, _x, _y, img_arr[_index]->width, img_arr[_index]->height );

	char imgtitle[strlen( imgname_arr[_index] )+7]; // 7 = ' ' + '<' + up to 3 nums + '>' + '\0'
	sprintf( imgtitle, "%s <%i>", imgname_arr[_index], _index );
	_y = draw_text( imgtitle, _x+20, _y+img_arr[_index]->height-10 )+10;
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
