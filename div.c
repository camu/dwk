// See LICENSE for copyright info

#include "div.h"
#include <stdlib.h>
#include <string.h>

Widget create_wid( const char *_txt ) {
	Widget wid;
	wid.txt = (char *)_txt;
	return wid;
}

Div create_div( ) {
	Div div;
	div.wid = malloc( 1 );
	div.nwid = 0;
	return div;
}

void div_add_wid( Div *_div, Widget _wid ) {
	_div->wid = realloc( _div->wid, sizeof( Widget )*(++_div->nwid) );
	_div->wid[_div->nwid-1] = _wid;
}
