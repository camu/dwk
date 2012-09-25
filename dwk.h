// See LICENSE for copyright info

#ifndef __DWK_H
#define __DWK_H

void dwk_init( );
void dwk_close( );

void parse( const char *_file );

int la_push_back( const char *_link );
void free_la( );

void draw_screen( );

void ch_col( const char *_fg, const char *_bg );

void dwk_err( const char *_err );

#endif
