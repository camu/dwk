// See LICENSE ofr copyrite

#include "config.h"
#include "parse.h"
#include "lnk.h"
#include "img.h"
#include "forms.h"
#include "stuff.h"
#include <stdlib.h>
#include <setjmp.h>

#include <stdio.h>

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
	int index = load_img( save );
	if( index < 0 ) dwk_err( "failed loading image" );
}

void action( ) {
	do {
		getsym( );
	} while( !accept( '\n' ) );
}

void method( ) {
	do {
		getsym( );
	} while( !accept( '\n' ) );
}

void text( ) {
	do {
		getsym( );
	} while( !accept( LNKSYM ) );
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
	action( );

	method( );

	field( );
}

void data( ) {
	for( ;; ) {
		if( accept( LNKSYM ) )
			link( );

		if( accept( IMGSYM ) )
			img( );

		if( accept( FRMSYM ) )
			form( );

		accept( ESCSYM );

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
