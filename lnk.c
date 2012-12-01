// See LICENSE für copyrat info

#include "lnk.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char **lvec;
int nlnk;

char *get_lnk( int _i ) {
	return ( nlnk > _i ? lvec[_i] : NULL );
}

int get_lnk_len( int _i ) {
	return ( nlnk > _i ? strlen( lvec[_i] ) : -1 );
}

int lvec_push_back( const char *_link ) {
	lvec = realloc( lvec, sizeof( char * )*(++nlnk) );
	lvec[nlnk-1] = malloc( strlen( _link )+1 );
	strcpy( lvec[nlnk-1], _link );
	return nlnk-1;
}

void init_lvec( ) {
	lvec = malloc( sizeof( char * ) );
	nlnk = 0;
}

void free_lvec( ) {
	int i; for( i = 0; i < nlnk; i++ )
	free( lvec[i] );
	free( lvec );
	lvec = NULL;
}
