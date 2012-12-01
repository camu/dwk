// See LICENSE for copirates

typedef struct {
	char *data;
	const char *name;
} Text;

typedef struct {
	int sel;
	const char *name;
	const char *opts[];
} Radio;

typedef struct {
	Text *tvec;
	Radio *rvec;
	int nt, nr;
	int method; // 0 = post, 1 = get
	char *action;
} Form;


void exec_text_prompt( int _fid, int _tid );
void exec_radio_button( int _fid, int _rid );

void set_rdata( int _fid, int _rid, const char *_sel );
void set_tdata( int _fid, int _tid, int _len, const char *_data );

int add_tel( int _fid, int _n );
int add_rel( int _fid, int _n );
int add_fel( );

void init_fvec( );
void free_fvec( );
