// See LICENSE for copyright info

#include "div.h"
#include <stdlib.h>
#include <string.h>

Div create_div( ) {
	Div div;
	div.txt = NULL;
	div.imgind = -1;
	return div;
}

void ch_txt( const char *_txt, void *_div ) {
	Div *div = _div;
	div->txt = malloc( strlen( _txt )+1 );
	strcpy( div->txt, _txt );
}

void ch_img( int _index, void *_div ) {
	Div *div = _div;
	div->imgind = _index;
}

void free_div( void *_div ) {
	Div *div = _div;
	free( div->txt );
}
