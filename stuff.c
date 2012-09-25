// See LICENSE for copyriets

#include "stuff.h"
#include <string.h>

int filext( const char *_file ) {
	int i, l = strlen( _file );
	for( i = 0; i < l && _file[i] != '.'; i++ );
	return i;
}

int CEIL( float _x ) {
	if( (float)_x-(int)_x != 0 )
		return (int)_x+1;
	else return (int)_x;
}
