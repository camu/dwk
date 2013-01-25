// See LICENSE ofr copyrite

#include "config.h"
#include "parse.h"
#include "lnk.h"
#include "img.h"
#include "forms.h"
#include "stuff.h"
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

char *save = NULL;
int slen;

static jmp_buf jmpbuf;
extern char *str;
extern int len;
int pos = 0;

void save_set( ) {
	save = realloc( save, STRBUFLEN+1 );
	slen = 0;
}

void savec( char _c ) {
	save[slen] = _c;
	slen++;
}

void getsym( ) {
	pos++;
	if( pos == len )
		longjmp( jmpbuf, 1 );
}

int accept( char _c ) {
	if( str[pos] == _c ) {
		getsym( );
		return 1;
	} return 0;
}

void link( ) {
	save_set( );
	do {
		savec( str[pos] );
		getsym( );
	} while( !accept( LNKSYM ) );
	savec( 0 );
	lvec_push_back( save );
}

void img( ) {
	save_set( );
	do {
		savec( str[pos] );
		getsym( );
	} while( !accept( IMGSYM ) );
	savec( 0 );
	if( load_img( save ) < 0 ) dwk_err( "failed loading image" );
}

void action( ) {
	save_set( );
	do {
		savec( str[pos] );
		getsym( );
	} while( !accept( '\n' ) );
	savec( 0 );
	fel_action( -1, save );
}

void method( ) {
	save_set( );
	do {
		savec( str[pos] );
		getsym( );
	} while( !accept( '\n' ) );
	savec( 0 );
	if( save[0] == 'P' )
		fel_method( -1, 0 );
	else
		fel_method( -1, 1 );
}

void text( ) {
	save_set( );
	do {
		savec( str[pos] );
		getsym( );
	} while( !accept( LNKSYM ) );
	savec( 0 );
	add_tel( -1, 1 );
	tel_name( -1, -1, save );
}

void radioname( ) {
	do {
		getsym( );
	} while( !accept( '\n' ) );
}

void opt( ) {
	for( ; !accept( IMGSYM ); ) {
		do {
			getsym( );
		} while( !accept( '\n' ) );
	}
}

void radio( ) {
	add_rel( -1, 1 );
	radioname( );
	opt( );
}

void field( ) {
	for( ; !accept( FRMSYM ); getsym( ) ) {
		if( accept( LNKSYM ) )
			text( );

		if( accept( IMGSYM ) )
			radio( );
	}
}

void form( ) {
	add_fel( );
	action( );
	method( );
	field( );
}

void escape( ) {
	int i, l = strlen( str );
	for( i = pos; i < l; i++ )
		str[i-1] = str[i];
	pos--;
}

void data( ) {
	for( ;; ) {
		if( accept( LNKSYM ) )
			link( );

		if( accept( IMGSYM ) )
			img( );

		if( accept( FRMSYM ) )
			form( );

		if( accept( ESCSYM ) )
			escape( );

		getsym( );
	}
}

void title( ) {
	save_set( );
	do {
		savec( str[pos] );
		getsym( );
	} while( !accept( '\n' ) );
	savec( 0 );
}

void parse( ) {
	if( !setjmp( jmpbuf ) ) {
		title( );
		data( );
	}

	free( save );
}
