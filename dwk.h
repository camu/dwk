// See LICENSE for copyright info

#ifndef __DWK_H
#define __DWK_H

#include "div.h"

void dwk_init( );
void add_div( Div _div );

void draw_screen( );
int draw_text( const char *_txt, int _x, int _y, int _w );

void ch_col( const char *_fg, const char *_bg );

void dwk_err( const char *_err );

#endif
