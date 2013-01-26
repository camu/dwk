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

//	int n = fvec[_fid].nr-1;
//	int i; for( i = 0; i < n; i++ )
//		if( strcmp( fvec[_fid].rvec[_rid].opts[i], _sel ) == 0 )
//			fvec[_fid].rvec[_rid].sel = i;
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

	opt_pb( fvec[_fid].rvec[_rid].opts, _str );

/*	int nopts = fvec[_fid].rvec[_rid].nopts-1;
	char **opts = fvec[_fid].rvec[_rid].opts;
	opts = realloc( opts, nopts*sizeof( char* ) );
	opts[nopts] = realloc( opts[nopts], strlen( _str )+1 );
	strcpy( opts[nopts], _str );
	fvec[_fid].rvec[_rid].opts = opts;
*/}

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
		init_ovec( &fvec[_fid].rvec[fvec[_fid].nr+i].opts );
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

void print_forms( ) {
	int i, j, k;
	for( i = 0; i < nf; i++ ) {
		printf( "FORM\n METHOD: %i\n ACTION: %s\n", fvec[i].method, fvec[i].action );
		for( j = 0; j < fvec[i].nt; j++ )
			printf( " TEXT\n  NAME: %s\n  DATA: %s\n", fvec[i].tvec[j].name, fvec[i].tvec[j].data );
		for( j = 0; j < fvec[i].nt; j++ ) {
			printf( " RADIO\n  NAME: %s\n  OPTS\n", fvec[i].rvec[j].name );
			Opt *cur = fvec[i].rvec[j].opts;
			k = 0;
			do {
				if( k == fvec[i].rvec[j].sel )
					printf( "  >%s\n", cur->name );
				else
					printf( "   %s\n", cur->name );
				k++;
			} while( cur->next && ( cur = cur->next ) );
		}
	}
}

void init_ovec( Opt **_ovec ) {
	*_ovec = malloc( sizeof( Opt ) );
	(*_ovec)->name = NULL;
	(*_ovec)->next = NULL;
}

void free_ovec( Opt *_ovec ) {
	if( _ovec->next )
		free_ovec( _ovec->next );
	else {
		free( _ovec->name );
		free( _ovec );
	}
}

Opt *opt_pb( Opt *_ovec, const char *_name ) {
	Opt *cur = _ovec;
	for( ; cur->next; cur = cur->next );
	if( cur->name ) {
		Opt *opt = malloc( sizeof( Opt ) );
		opt->name = NULL;
		opt->next = NULL;
		cur->next = opt;
		cur = cur->next;
	}
	cur->name = realloc( cur->name, strlen( _name )+1 );
	strcpy( cur->name, _name );
	return cur;
}

Opt *get_opt( Opt *_ovec, int _n ) {
	Opt *cur = _ovec;
	if( _n == -1 ) {
		for( ; cur->next; )
			cur = cur->next;
	} else {
		int i; for( i = 0; i < _n && cur->next; i++ )
			cur = cur->next;
	}
	return cur;
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
		for( j = 0; j < fvec[i].nr; j++ ) {
			free( fvec[i].rvec[j].name );
			free_ovec( fvec[i].rvec[j].opts );
		}
		free( fvec[i].tvec );
		free( fvec[i].rvec );
		free( fvec[i].action );
	}
	free( fvec );
}
