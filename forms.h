// See LICENSE for copirates

typedef struct {
	char *data;
	char *name;
} Text;

typedef struct opt Opt;
struct opt {
	char *name;
	Opt *next;
};

typedef struct {
	int sel;
	char *name;
	int nopts;
	Opt *opts;
} Radio;

typedef struct {
	Text *tvec;
	Radio *rvec;
	int nt, nr;
	char *action;
	int method; // 0 = post, 1 = get
} Form;


void exec_text_prompt( int _fid, int _tid );
void exec_radio_button( int _fid, int _rid );

void set_tdata( int _fid, int _tid, int _len, const char *_data );
void set_rdata( int _fid, int _rid, const char *_sel );

void tel_name( int _fid, int _tid, const char *_str );
void rel_name( int _fid, int _rid, const char *_str );
void rel_opt( int _fid, int _rid, const char *_str );
void fel_action( int _fid, const char *_str );
void fel_method( int _fid, int _method );

int add_tel( int _fid, int _n );
int add_rel( int _fid, int _n );
int add_fel( );

void init_ovec( Opt **_ovec );
Opt *opt_pb( Opt *_ovec, const char *_name );
Opt *get_opt( Opt *_ovec, int _n );

void print_forms( );

void init_fvec( );
void free_fvec( );
