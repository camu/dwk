// See LICENSE for copyright info

#ifndef __DIV_H
#define __DIV_H

typedef struct {
	char *txt;
} Widget;

typedef struct {
	Widget *wid;
	int nwid;
} Div;

Widget create_wid( const char *_txt );
Div create_div( );
void div_add_wid( Div *_div, Widget _wid );

#endif
