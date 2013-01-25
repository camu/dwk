// See LICENSE for copirates

#include "forms.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Form *fvec;
int nf;

void exec_text_prompt( int _fid, int _tid ) {
	char *data = fvec[_fid].tvec[_tid].data;

	FILE *retp = popen( "diwk -t 1", "r" );

	fseek( retp, 0, SEEK_END );
	int len = ftell( retp );
	fseek( retp, 0, SEEK_SET );

	data = realloc( data, len+1 );
	fgets( data, len+1, retp );

	pclose( retp );

	fvec[_fid].tvec[_tid].data = data;
}

void exec_radio_button( int _fid, int _rid ) {
}

void set_tdata( int _fid, int _tid, int _len, const char *_data ) {
	if( _fid == -1 ) _fid = nf-1;
	if( _tid == -1 ) _tid = fvec[_fid].nt-1;

	char *data = fvec[_fid].tvec[_tid].data;
	data = realloc( data, _len );
	strcpy( data, _data );
	fvec[_fid].tvec[_tid].data = data;
}

void set_rdata( int _fid, int _rid, const char *_sel ) {
	if( _fid == -1 ) _fid = nf-1;
	if( _rid == -1 ) _rid = fvec[_fid].nr-1;

	int n = fvec[_fid].nr-1;
	int i; for( i = 0; i < n; i++ )
		if( strcmp( fvec[_fid].rvec[_rid].opts[i], _sel ) == 0 )
			fvec[_fid].rvec[_rid].sel = i;
}

void tel_name( int _fid, int _tid, const char *_str ) {
	if( _fid == -1 ) _fid = nf-1;
	if( _tid == -1 ) _tid = fvec[_fid].nt-1;

	char *name = fvec[_fid].tvec[_tid].name;
	name = realloc( name, strlen( _str )+1 );
	strcpy( name, _str );
	fvec[_fid].tvec[_tid].name = name;
}

void rel_name( int _fid, int _rid, const char *_str ) {
	if( _fid == -1 ) _fid = nf-1;
	if( _rid == -1 ) _rid = fvec[_fid].nr-1;

	char *name = fvec[_fid].rvec[_rid].name;
	name = realloc( name, strlen( _str )+1 );
	strcpy( name, _str );
	fvec[_fid].rvec[_rid].name = name;
}

void rel_opt( int _fid, int _rid, const char *_str ) {
	if( _fid == -1 ) _fid = nf-1;
	if( _rid == -1 ) _rid = fvec[_fid].nr-1;
}

void fel_action( int _fid, const char *_str ) {
	if( _fid == -1 ) _fid = nf-1;

	char *act = fvec[_fid].action;
	act = realloc( act, strlen( _str )+1 );
	strcpy( act, _str );
	fvec[_fid].action = act;
}

void fel_method( int _fid, int _method ) {
	if( _fid == -1 ) _fid = nf-1;

	fvec[_fid].method = _method;
}

int add_tel( int _fid, int _n ) {
	if( _fid == -1 ) _fid = nf-1;

	fvec[_fid].tvec = realloc( fvec[_fid].tvec, (fvec[_fid].nt+1)*sizeof( Text ) );
	int i; for( i = 0; i < _n; i++ ) {
		fvec[_fid].tvec[fvec[_fid].nt+i].name = NULL;
		fvec[_fid].tvec[fvec[_fid].nt+i].data = NULL;
	}
	fvec[_fid].nt += _n;
	return fvec[_fid].nt-_n;
}

int add_rel( int _fid, int _n ) {
	if( _fid == -1 ) _fid = nf-1;

	fvec[_fid].rvec = realloc( fvec[_fid].rvec, (fvec[_fid].nr+1)*sizeof( Radio ) );
	int i; for( i = 0; i < _n; i++ ) {
		fvec[_fid].rvec[fvec[_fid].nr+i].sel = 0;
		fvec[_fid].rvec[fvec[_fid].nr+i].name = NULL;
//		fvec[_fid].rvec[fvec[_fid].nr+i].opts = NULL;
		fvec[_fid].rvec[fvec[_fid].nr+i].nopts = 0;
	}
	fvec[_fid].nr += _n;
	return fvec[_fid].nr-_n;
}

int add_fel( ) {
	fvec = realloc( fvec, (++nf)*sizeof( Form ) );
	fvec[nf-1].tvec = NULL;
	fvec[nf-1].rvec = NULL;
	fvec[nf-1].action = NULL;
	fvec[nf-1].nt = fvec[nf-1].nr = 0;
	return nf-1;
}

int get_nf( ) {
	return nf;
}

void init_fvec( ) {
	fvec = NULL;
	nf = 0;
}

void free_fvec( ) {
	int i, j;
	for( i = 0; i < nf; i++ ) {
		for( j = 0; j < fvec[i].nt; j++ ) {
			free( fvec[i].tvec[j].data );
			free( fvec[i].tvec[j].name );
		}
		free( fvec[i].tvec );
		free( fvec[i].rvec );
		free( fvec[i].action );
	}
	free( fvec );
}
