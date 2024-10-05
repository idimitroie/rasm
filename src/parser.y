%debug

%code requires {
    #include "ast.h"
}

%{
    /* Bison does not decleare this function properly */
    int yylex();

    #include "main.h"
    #include "storage.h"
    #include "parser.tab.h"
	#include "label.h"
	#include "loclab.h"
	#include "progc.h"
	#include "ast.h"
	#include "instr.h"
	#include "decode.h"
	#include "parser.tab.h"
extern int yyget_lineno ( void );
extern int pass_number ; /* Start with 1 - the first pass */
%}

%union {
    instr_codes_t instr_codes;
    instr_t* instr;
    char* str;
    int num;
}

%token NEW_LINE COLON COMMA NEG
%token <instr_codes> R_TYPE I_TYPE I_SHAMT_TYPE B_TYPE U_TYPE J_TYPE S_TYPE
%token <num> REG NUM LOCALLABEL REFLLBACK REFLLFORWARD
%token <str> IDENTIFIER
%token <str> LABEL

%type <instr> instr
%type <num> num
%type <num> llrel
%type <num> b_label

%%

list: /* nothing */
    | list line
    ;

line: opt_label opt_instr NEW_LINE { yyget_lineno(); }

opt_label:
    /* nothing */
	| LOCALLABEL { if (1 == pass_number) { storage_add_locl_label($1, get_pc(), yyget_lineno());loclab_add($1, get_pc()); } }
    | LABEL { if (1 == pass_number) { storage_add_glob_label($1, get_pc(), yyget_lineno()); if (label_exist($1)) {dmp_print("### ERROR: label redefinition \"%s\"\n", $1);} else {label_add($1, get_pc()); } } if ($1) free($1); }

opt_instr:
    /* nothing */
    | instr { if (2 == pass_number) {storage_add_instr($1, get_pc(), yyget_lineno());} advance_word(); }

instr: R_TYPE REG COMMA REG COMMA REG       { $$ = instr_R_type($1, $2, $4, $6); }
     | I_TYPE REG COMMA REG COMMA num       { $$ = instr_I_type($1, $2, $4, $6); }
     | I_SHAMT_TYPE REG COMMA REG COMMA num { $$ = instr_I_SHAMT_type($1, $2, $4, $6); }
     | B_TYPE REG COMMA REG COMMA num       { $$ = instr_B_type($1, $2, $4, $6); }
     | B_TYPE REG COMMA REG COMMA llrel     { $$ = instr_B_type($1, $2, $4, $6); }
     | B_TYPE REG COMMA REG COMMA b_label   { $$ = instr_B_type($1, $2, $4, $6); }
     | U_TYPE REG COMMA num                 { $$ = instr_U_type($1, $2, $4); }
     | J_TYPE REG COMMA num                 { $$ = instr_J_type($1, $2, $4); }
     | S_TYPE REG COMMA num COMMA REG       { $$ = instr_S_type($1, $2, $4, $6); }
     ;

num: NUM
   | NEG NUM { $$ = -$2; };
   ;

llrel: REFLLBACK   { $$ = 0; if (2 == pass_number) { $$ = 0; unsigned addr; int exist = loclab_getpc(&addr, -$1, get_pc()); if (exist) {$$ = addr - get_pc(); storage_add_locl_identifier($1, get_pc(), $$, addr, yyget_lineno()); } else {dmp_print("### Err non existing id \"%u\"\tline %d\n", $1, yyget_lineno());} } }
	| REFLLFORWARD { $$ = 0; if (2 == pass_number) { $$ = 0; unsigned addr; int exist = loclab_getpc(&addr, $1, get_pc());  if (exist) {$$ = addr - get_pc(); storage_add_locl_identifier($1, get_pc(), $$, addr, yyget_lineno()); } else {dmp_print("### Err non existing id \"%u\"\tline %d\n", $1, yyget_lineno());} } }
	;

b_label: IDENTIFIER
		{
			if (debug)
			{
				fprintf(stderr, "IDENTIFIER id = %s\n", $1);
				fflush(stderr);
			}
			$$ = 0;
			if (2 == pass_number)
			{
				unsigned addr = 0;
				int exist = label_getpc(&addr, $1);
				$$ = addr - get_pc();
				if (exist)
					storage_add_glob_identifier($1, get_pc(), $$, addr, yyget_lineno());
				else
				{
					fprintf(stderr, "? Error label \"%s\" not found\n", $1);
					dmp_print("### Err \"%s\" not found\n", $1);
					fflush(stdout);
				}
			}
			if ($1)
				free($1);
		}
	   ;
