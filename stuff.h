// See LICENSE for copyriet

#ifndef __STUFF_H
#define __STUFF_H

void dwk_err( const char *_err );

int filext( const char *_file );

int CEIL( float _x );
//#define CEIL( _x ) ( ( (float)_x-(int)_x != 0 ) ? (int)_x+1 : (int)_x )

#endif
