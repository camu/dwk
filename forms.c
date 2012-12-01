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

void set_rdata( int _fid, int _rid, const char *_sel ) {
	if( _fid == -1 ) _fid = nf-1;
	if( _rid == -1 ) _fid = fvec[_fid].nr-1;

	int n = fvec[_fid].nr-1;
	int i; for( i = 0; i < n; i++ )
		if( strcmp( fvec[_fid].rvec[_rid].opts[i], _sel ) == 0 )
			fvec[_fid].rvec[_rid].sel = i;
}

void set_tdata( int _fid, int _tid, int _len, const char *_data ) {
	if( _fid == -1 ) _fid = nf-1;
	if( _tid == -1 ) _fid = fvec[_fid].nt-1;

	char *data = fvec[_fid].tvec[_tid].data;
	data = realloc( data, _len );
	strcpy( data, _data );
	fvec[_fid].tvec[_tid].data = data;
}

int add_tel( int _fid, int _n ) {
	if( _fid == -1 ) _fid = nf-1;

	fvec[_fid].tvec = realloc( fvec[_fid].tvec, (fvec[_fid].nt += _n)*sizeof( Text ) );
	int i; for( i = 0; i < _n; i++ )
		fvec[_fid].tvec[i+fvec[_fid].nt].data = NULL;
	return fvec[_fid].nt-_n;
}

int add_rel( int _fid, int _n ) {
	if( _fid == -1 ) _fid = nf-1;

	fvec[_fid].rvec = realloc( fvec[_fid].rvec, (fvec[_fid].nr += _n)*sizeof( Radio ) );
	return fvec[_fid].nr-_n;
}

int add_fel( ) {
	fvec = realloc( fvec, (++nf)*sizeof( Form ) );
	fvec[nf-1].tvec = NULL;
	fvec[nf-1].rvec = NULL;
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
		for( j = 0; j < fvec[i].nt; j++ )
			free( fvec[i].tvec[j].data );
		free( fvec[i].tvec );
		free( fvec[i].rvec );
	}
	free( fvec );
}
