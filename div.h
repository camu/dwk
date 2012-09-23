// See LICENSE for copyright info

#ifndef __DIV_H
#define __DIV_H

typedef struct {
	char *txt;
	int imgind;
} Div;

Div create_div( );

void ch_txt( const char *_txt, void *_div );
void ch_img( int _index, void *_div );

void free_div( void *_div );

#endif
