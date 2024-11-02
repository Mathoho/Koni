/**
 * @file    alanc.c
 *
 * A recursive-descent compiler for the ALAN-2022 language.
 *
 * All scanning errors are handled in the scanner.  Parser errors MUST be
 * handled by the <code>abort_compile</code> function.  System and environment errors,
 * for example, running out of memory, MUST be handled in the unit in which they
 * occur.  Transient errors, for example, non-existent files, MUST be reported
 * where they occur.  There are no warnings, which is to say, all errors are
 * fatal and MUST cause compilation to terminate with an abnormal error code.
 *
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 * @date    2022-08-03
 */

/* TODO: Include the appropriate system and project header files. */

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "boolean.h"
#include "error.h"
#include "errmsg.h"
#include "scanner.h"
#include "token.h"
#include "valtypes.h"
#include "hashtable.h"
#include "symboltable.h"
#include "codegen.h"

/* --- type definitions ----------------------------------------------------- */

/* TODO: Uncomment the following for use during type checking. */

typedef struct variable_s Variable;
struct variable_s {
	char      *id;     /**< variable identifier                       */
	ValType    type;   /**< variable type                             */
	SourcePos  pos;    /**< variable position in the source           */
	Variable  *next;   /**< pointer to the next variable in the list  */
};


/* --- debugging ------------------------------------------------------------ */

/* TODO: Your Makefile has a variable called DFLAGS.  If it is set to contain
 * -DDEBUG_PARSER, it will cause the following prototypes to be included, and
 *  the functions to which they refer (given at the end of this file) will be
 *  compiled.  If, on the other hand, this flag is comment out, by setting
 *  DFLAGS to #-DDEBUG_PARSER, these functions will be excluded.  These
 *  definitions should be used at the start and end of every parse function.
 *  For an example, see the provided parse_program function.
 */

#ifdef DEBUG_PARSER
void debug_start(const char *fmt, ...);
void debug_end(const char *fmt, ...);
void debug_info(const char *fmt, ...);
#define DBG_start(...) debug_start(__VA_ARGS__)
#define DBG_end(...) debug_end(__VA_ARGS__)
#define DBG_info(...) debug_info(__VA_ARGS__)
#else
#define DBG_start(...)
#define DBG_end(...)
#define DBG_info(...)
#endif /* DEBUG_PARSER */

/* --- global variables ----------------------------------------------------- */

Token    token;        /**< the lookahead token.type                */
FILE    *src_file;     /**< the source code file                    */

ValType  return_type;  /**< the return type of the current function */


/* TODO: Uncomment the previous definition for use during type checking. */

/* --- function prototypes: parser routines --------------------------------- */

void parse_source(void);
/* TODO: Add the prototypes for the rest of the parse functions. */
void parse_funcdef();
void parse_body();
void parse_type(ValType *type);
void parse_vardef();
void parse_statements();
void parse_statement();
void parse_assign();
void parse_call();
void parse_if();
void parse_input();
void parse_leave();
void parse_output();
void parse_while();
void parse_expr(ValType *type);
void parse_simple(ValType *type);
void parse_term(ValType *type);
void parse_factor(ValType *type);
/* --- helper macros -------------------------------------------------------- */

#define STARTS_FACTOR(toktype) \
	(toktype == TOKEN_ID || toktype == TOKEN_NUMBER || \
	 toktype == TOKEN_OPEN_PARENTHESIS || toktype == TOKEN_NOT || \
	 toktype == TOKEN_TRUE || toktype == TOKEN_FALSE)

#define STARTS_EXPR(toktype) \
	(toktype == TOKEN_MINUS || STARTS_FACTOR(toktype))

#define IS_ADDOP(toktype) \
	(toktype >= TOKEN_MINUS && toktype <= TOKEN_PLUS)

#define IS_MULOP(toktype) \
(toktype == TOKEN_AND || toktype == TOKEN_DIVIDE || \
toktype == TOKEN_MULTIPLY || toktype == TOKEN_REMAINDER)


#define IS_ORDOP(toktype) /* TODO*/

#define IS_RELOP(toktype) \
(toktype == TOKEN_EQUAL || toktype == TOKEN_GREATER_EQUAL || \
toktype == TOKEN_GREATER_THAN || toktype == TOKEN_LESS_EQUAL || \
toktype == TOKEN_LESS_THAN || toktype == TOKEN_NOT_EQUAL)


#define IS_TYPE_TOKEN(toktype) \
	(toktype == TOKEN_BOOLEAN || toktype == TOKEN_INTEGER)

/* --- function prototypes: helper routines --------------------------------- */

/* TODO: Uncomment the following commented-out prototypes for use during type
 * checking.
 */

void check_types(ValType type1, ValType type2, SourcePos *pos, ...);

void expect(TokenType type);
void expect_id(char **id);

IDprop *idprop(ValType type, unsigned int offset, unsigned int nparams,
		ValType *params);
Variable *variable(char *id, ValType type, SourcePos pos);


/* --- function prototypes: error reporting --------------------------------- */

void abort_compile(Error err, ...);
void abort_compile_pos(SourcePos *posp, Error err, ...);

/* --- main routine --------------------------------------------------------- */

int main(int argc, char *argv[])
{

	char *jasmin_path;

	/* TODO: Uncomment the previous definition for code generation. */

	/* set up global variables */
	setprogname(argv[0]);
	/* check command-line arguments and environment */
	if (argc != 2) {
		eprintf("usage: %s <filename>", getprogname());
	}
	// TODO: Uncomment the following for code generation
	if ((jasmin_path = getenv("JASMIN_JAR")) == NULL) {
		eprintf("JASMIN_JAR environment variable not set");
	}
	/* open the source file, and report an error if it cannot be opened */
	if ((src_file = fopen(argv[1], "r")) == NULL) {
		eprintf("file '%s' could not be opened:", argv[1]);
	}
	setsrcname(argv[1]);
	/* initialise all compiler units */
	init_scanner(src_file);
	init_symbol_table();
	init_code_generation();
	/* compile */
	get_token(&token);
	parse_source();
	/* produce the object code, and assemble */
	/* TODO: Add calls for code generation. */
	make_code_file();
	assemble(jasmin_path);
	/* release allocated resources */
	/* TODO: Release the resources of the symbol table and code generation. */
	fclose(src_file);
	freeprogname();
	freesrcname();
	release_symbol_table();
	release_code_generation();
#ifdef DEBUG_PARSER
	printf("SUCCESS!\n");
#endif
	return EXIT_SUCCESS;
}
/* --- parser routines ------------------------------------------------------ */

/*
 * <source> = "source" <id> { <funcdef> } <body>.
 */
void parse_source()
{
	char *class_name;
	DBG_start("<source>");

	/* TODO: For code generation, set the class name inside this function, and
	 * also handle initialising and closing the "main" function.  But from the
	 * perspective of simple parsing, this function is complete.
	 */
	expect(TOKEN_SOURCE);
	expect_id(&class_name);
	const char *clnm = "main";
	open_subroutine(strdup(clnm), idprop(TYPE_CALLABLE, 0, 0, NULL));
	/* TODO: Set the class name here for code generation.*/
	set_class_name(class_name);
	while (token.type == TOKEN_FUNCTION) {
		parse_funcdef();
	}
	init_subroutine_codegen("main", idprop(TYPE_CALLABLE,0,0,NULL));
	parse_body();
	gen_1(JVM_RETURN);
	close_subroutine_codegen(1);
	close_subroutine();
	free(class_name);
	DBG_end("</source>");
}

/* TODO: Turn the EBNF into a program by writing one parse function for those
 * productions as instructed in the specification.  I suggest you use the
 * production as comment to the function.  Also, you may only report errors
 * through the abort_compile and abort_compile_pos functions.  You must figure
 * out what arguments you should pass for each particular error.  Keep to the
 * *exact* error messages given in the specification.
 */

/*
*<funcdef> = “function”<id> “(” [<type> <id> {“,” <type> <id>}]“)”[“to” <type>
*<body>.
*/
void parse_funcdef()
{
	char *fname;
	DBG_start("<funcdef>");
    expect(TOKEN_FUNCTION);
    expect_id(&fname);
	SourcePos posp;
	posp.line = 0;
	posp.col = 0;
	char *name = fname;
	ValType type;
	int prms = 0;
	Variable *first_var, *temp_var;
	int tr = 1;
	expect(TOKEN_OPEN_PARENTHESIS);
	if (IS_TYPE_TOKEN(token.type)) {
		tr+=1;
		parse_type(&type);
		first_var = variable(fname, type, posp);
		char *idsname;
		expect_id(&idsname);
		prms+=1;
		while (token.type == TOKEN_COMMA) {
			expect(TOKEN_COMMA);
			parse_type(&type);
			expect_id(&idsname);
			temp_var = first_var;
			first_var = variable(fname, type, posp);
			first_var->next = temp_var;
			prms+=1;
		}
		free(idsname);
	}
	expect(TOKEN_CLOSE_PARENTHESIS);
	if (token.type == TOKEN_TO) {
		get_token(&token);
		ValType type1;
		parse_type(&type1);
		return_type = type1;
	}
	if (tr > 1) {
		int j = 0;
		ValType *linked_list;
		linked_list = malloc(prms * sizeof(ValType));
		temp_var = first_var;
		while (j < prms) {
			linked_list[j] = first_var->type;
			first_var = first_var->next;
			j+=1;
		}
		IDprop *prp;
		prp = idprop(temp_var->type, 0, 0, NULL);
		init_subroutine_codegen(name, idprop(return_type, 0, prms, linked_list));
		open_subroutine(name, idprop(return_type, 0, prms, linked_list));
		int k = 0;
		while (k < prms) {
			insert_name(temp_var->id, prp);
			temp_var = temp_var->next;
			k+=1;
		}
	}
	if (tr == 1) {
		Variable *first_var = NULL;
		Variable *temp_var = NULL;
		first_var = temp_var;
		init_subroutine_codegen(name, idprop(TYPE_NONE, 0, 0, NULL));
		open_subroutine(name, idprop(TYPE_NONE, 0, 0, NULL));
	}
	parse_body();
	free(fname);
	int v = prms + get_variables_width();
	close_subroutine_codegen(v);
	close_subroutine();
	DBG_end("</funcdef>");
}
/*
*⟨body⟩ = “begin” {⟨vardef ⟩} ⟨statements⟩ “end”.
*/
void parse_body()
{
	DBG_start("<body>");
	expect(TOKEN_BEGIN);
	while (token.type == TOKEN_BOOLEAN || token.type == TOKEN_INTEGER) {
		parse_vardef();
	}
	parse_statements();
	expect(TOKEN_END);
	DBG_end("</body>");
}
/*
*⟨vardef⟩ = ⟨type⟩ ⟨id⟩ {“,” ⟨id⟩} “;”.
*/
void parse_vardef()
{
	char *vname;
	DBG_start("<vardef>");
	ValType type;
	parse_type(&type);
	expect_id (&vname);
	IDprop *prp = idprop(type, get_variables_width(), 0, NULL);
	insert_name(vname, prp);
	while (token.type == TOKEN_COMMA) {
		expect(TOKEN_COMMA);
		expect_id(&vname);
		IDprop *prp2 = idprop(type, get_variables_width(), 0, NULL);
		insert_name(vname, prp2);
	}
	expect (TOKEN_SEMICOLON);
	free(vname);
	DBG_end("</vardef>");
}
/*
*⟨type⟩ = (“boolean” ∣ “integer”) [“array”].
*/
void parse_type(ValType *type)
{
	DBG_start("<type>");
	IS_TYPE_TOKEN(token.type);
	switch (token.type) {
		case TOKEN_INTEGER:
			*type = TYPE_INTEGER;
			get_token(&token);
			if (token.type == TOKEN_ARRAY) {
				get_token(&token);
				SET_AS_ARRAY(*type);
			}
		break;
		case TOKEN_BOOLEAN:
			*type = TYPE_BOOLEAN;
			get_token(&token);
			if (token.type == TOKEN_ARRAY) {
				get_token(&token);
				SET_AS_ARRAY(*type);
			}
		break;
		default:
		abort_compile(ERR_STATEMENT_EXPECTED, token.type);
		break;
	}
	DBG_end("</type>");
}
/*
*⟨statements⟩ = “relax” ∣ ⟨statement⟩ {“;” ⟨statement⟩}.
*/
void parse_statements()
{
	DBG_start("<statements>");
	if (token.type == TOKEN_RELAX) {
		get_token(&token);
	} else {
		parse_statement();
		while (token.type == TOKEN_SEMICOLON) {
			expect(TOKEN_SEMICOLON);
			parse_statement();
		}
	}
	DBG_end("</statements>");
}
/*
*⟨statement⟩ = ⟨assign⟩∣⟨call⟩∣⟨if ⟩∣⟨input⟩∣⟨leave⟩∣⟨output⟩∣⟨while⟩.
*/
void parse_statement()
{
	DBG_start("<statement>");
	switch (token.type) {
	case TOKEN_ID:
		parse_assign();
	break;
	case TOKEN_CALL:
		parse_call();
	break;
	case TOKEN_IF:
		parse_if();
	break;
	case TOKEN_GET:
		parse_input();
	break;
	case TOKEN_LEAVE:
		parse_leave();
	break;
	case TOKEN_PUT:
		parse_output();
	break;
	case TOKEN_WHILE:
		parse_while();
	break;
	default:
	abort_compile(ERR_STATEMENT_EXPECTED, token.type);
	break;
	}
	DBG_end("</statement>");
}
/*
*⟨assign⟩ = ⟨id⟩[“[”⟨simple⟩“]”] “:=” (⟨expr⟩| “array” ⟨simple⟩).
*/
void parse_assign()
{
	DBG_start("<assign>");
	ValType type;
	char *asname;
	IDprop *prop;
	expect_id(&asname);
	find_name(asname,&prop);
	if (token.type == TOKEN_OPEN_BRACKET) {
		get_token(&token);
		parse_simple(&type);
		expect(TOKEN_CLOSE_BRACKET);
	}
	expect(TOKEN_GETS);
	if (token.type == TOKEN_ARRAY) {
		get_token(&token);
		parse_simple(&type);
	} else if (STARTS_EXPR(token.type)) {
			parse_expr(&type);
			gen_2(JVM_ISTORE,prop->offset);
			} else if (token.type != TOKEN_ARRAY && !STARTS_EXPR(token.type)) {
				abort_compile(ERR_MISSING_LEAVE_EXPRESSION_FOR_FUNCTION,
				token.type);
			}
	free(asname);
	DBG_end("</assign>");
}
/*
*⟨call⟩ = “call” ⟨id⟩ “(” [⟨expr⟩ {“,” ⟨expr⟩}] “)”.
*/
void parse_call()
{
	DBG_start("<call>");
	char *clname;
	ValType type;
	expect(TOKEN_CALL);
	expect_id(&clname);
	expect(TOKEN_OPEN_PARENTHESIS);
	if (STARTS_EXPR(token.type)) {
		parse_expr(&type);
		while (token.type == TOKEN_COMMA) {
			expect(TOKEN_COMMA);
			parse_expr(&type);
		}
	}
	  expect(TOKEN_CLOSE_PARENTHESIS);
	  free(clname);
	  DBG_end("</call>");
}
/*
*⟨if ⟩ = “if” ⟨expr⟩ “then” ⟨statements⟩ {“elsif” ⟨expr⟩ “then” ⟨statements⟩}
[“else” ⟨statements⟩] “end”.
*/
void parse_if()
{
	DBG_start("<if>");
	ValType type;
	expect(TOKEN_IF);
	parse_expr(&type);
	expect(TOKEN_THEN);
	parse_statements();
	while (token.type == TOKEN_ELSIF) {
		expect(TOKEN_ELSIF);
		parse_expr(&type);
		expect(TOKEN_THEN);
		parse_statements();
	}
	if (token.type == TOKEN_ELSE) {
		expect(TOKEN_ELSE);
		parse_statements();
	}
	expect(TOKEN_END);
	DBG_end("</if>");
}
/*
*⟨input⟩ = “get” ⟨id⟩ [“[” ⟨simple⟩ “]”].
*/
void parse_input()
{
	char *iname;
	ValType type;
	DBG_start("<input>");
	expect(TOKEN_GET);
	expect_id(&iname);
	IDprop *prop;
	find_name(iname,&prop);
	gen_2(JVM_ISTORE,prop->offset);
	if (token.type == TOKEN_OPEN_BRACKET) {
		expect(TOKEN_OPEN_BRACKET);
		parse_simple(&type);
		expect(TOKEN_CLOSE_BRACKET);
	}
	free(iname);
	DBG_end("</input>");
}
/*
*⟨leave⟩ = “leave” [⟨expr⟩].
*/
void parse_leave()
{
	DBG_start("<leave>");
	ValType type;
	expect(TOKEN_LEAVE);
	if (STARTS_FACTOR(token.type)) {
		parse_expr(&type);
		if (IS_ARRAY_TYPE(type)) {
			gen_1(JVM_ARETURN);
		} else {
			gen_1(JVM_IRETURN);
		}
	} else {
		gen_1(JVM_RETURN);
	}
	DBG_end("</leave>");
}
/*
*⟨output⟩ = “put” (⟨string⟩ ∣ ⟨expr⟩) {“.” (⟨string⟩ ∣ ⟨expr⟩)}.
*/
void parse_output()
{
	DBG_start("<output>");
	ValType type;
	expect(TOKEN_PUT);
	if (token.type == TOKEN_STRING) {
		gen_print_string(strdup(token.string));
		get_token(&token);
	} else if (STARTS_EXPR(token.type)) {
		parse_expr(&type);
		gen_print(type);
		} else {
			abort_compile(ERR_EXPRESSION_OR_STRING_EXPECTED);
		}
	while (token.type == TOKEN_CONCATENATE) {
		expect(TOKEN_CONCATENATE);
		if (token.type == TOKEN_STRING) {
			gen_print_string(strdup(token.string));
			expect(TOKEN_STRING);
		} else if (STARTS_EXPR(token.type)) {
			parse_expr(&type);
			gen_print(type);
		} else {
			abort_compile(ERR_EXPRESSION_OR_STRING_EXPECTED);
		}
	}
	DBG_end("</output>");
}
/*
*⟨while⟩ = “while” ⟨expr⟩ “do” ⟨statements⟩ “end”.
*/
void parse_while() {
	DBG_start("<while>");
	ValType type;
	expect(TOKEN_WHILE);
	parse_expr(&type);
	expect(TOKEN_DO);
	parse_statements();
	expect(TOKEN_END);
	DBG_end("</while>");
}
/*
*⟨expr⟩ = ⟨simple⟩ [⟨relop⟩ ⟨simple⟩].
*/
void parse_expr(ValType *type)
{
	DBG_start("<expr>");
	ValType type1;
	parse_simple(&type1);
	if (IS_RELOP(token.type)) {
		if (token.type == TOKEN_EQUAL) {
			get_token(&token);
			ValType type2;
			parse_simple(&type2);
			gen_cmp(JVM_IF_ICMPEQ);
			*type = TYPE_BOOLEAN;
		} else if (token.type == TOKEN_NOT_EQUAL) {
			printf("the thing is NOT EQUAL here\n");
			get_token(&token);
			ValType type2;
			parse_simple(&type2);
			gen_cmp(JVM_IF_ICMPNE);
			*type = TYPE_BOOLEAN;
		} else if (token.type == TOKEN_GREATER_EQUAL) {
			get_token(&token);
			ValType type2;
			parse_simple(&type2);
			gen_cmp(JVM_IF_ICMPGE);
			*type = TYPE_BOOLEAN;
		} else if (token.type == TOKEN_GREATER_THAN) {
			get_token(&token);
			ValType type2;
			parse_simple(&type2);
			gen_cmp(JVM_IF_ICMPGT);
			*type = TYPE_BOOLEAN;
		} else if (token.type == TOKEN_LESS_EQUAL) {
			get_token(&token);
			ValType type2;
			parse_simple(&type2);
			gen_cmp(JVM_IF_ICMPLE);
			*type = TYPE_BOOLEAN;
		} else if (token.type == TOKEN_LESS_THAN) {
			get_token(&token);
			ValType type2;
			parse_simple(&type2);
			gen_cmp(JVM_IF_ICMPLT);
			*type = TYPE_BOOLEAN;
		}
	} else {
		*type = type1;
	}
	DBG_end("</expr>");
}
/*
*⟨simple⟩ = [“-”] ⟨term⟩ {⟨addop⟩ ⟨term⟩}.
*/
void parse_simple(ValType *type)
{
	DBG_start("<simple>");
	ValType type1;

	if (token.type == TOKEN_MINUS) {
		get_token(&token);
		parse_term(&type1);
		gen_1(JVM_INEG);
	}
	else {
		parse_term(&type1);
	}
	while (IS_ADDOP(token.type)) {
		if (token.type == TOKEN_PLUS) {
			ValType type2;
			get_token(&token);
			parse_term(&type2);
			gen_1(JVM_IADD);
		} else if (token.type == TOKEN_MINUS) {
			ValType type2;
			get_token(&token);
			parse_term(&type2);
			gen_1(JVM_ISUB);
		} else if (token.type == TOKEN_OR) {
			ValType type2;
			get_token(&token);
			parse_term(&type2);
			gen_1(JVM_IOR);
		}
	}
	*type =  type1;
	DBG_end("</simple>");
}
/*
*⟨term⟩ = ⟨factor⟩ {⟨mulop⟩ ⟨factor⟩}.
*/
void parse_term(ValType *type)
{
	DBG_start("<term>");
	ValType type1;
	parse_factor(&type1);
	while (IS_MULOP(token.type)) {
		if (token.type == TOKEN_AND) {
			get_token(&token);
			ValType type2;
			parse_factor(&type2);
			gen_1(JVM_IAND);
		} else if (token.type == TOKEN_DIVIDE) {
			ValType type2;
			get_token(&token);
			parse_factor(&type2);
			gen_1(JVM_IDIV);
		} else if (token.type == TOKEN_MULTIPLY) {
			ValType type2;
			get_token(&token);
			parse_factor(&type2);
			gen_1(JVM_IMUL);
		} else if (token.type == TOKEN_REMAINDER) {
			ValType type2;
			get_token(&token);
			parse_factor(&type2);
			gen_1(JVM_IREM);
		}
	 }
	*type =  type1;
	DBG_end("</term>");
}
/*
*⟨factor⟩ = ⟨id⟩ [“[” ⟨simple⟩ “]” | “(” [ ⟨expr⟩ {“,” ⟨expr⟩} ] “)”] ∣ ⟨num⟩ ∣
“(” ⟨expr⟩ “)” ∣ “not” ⟨factor⟩ ∣ “true” ∣ “false”.
*/
void parse_factor(ValType *type)
{
	char *name;
	IDprop *prop;
	DBG_start("<factor>");
	STARTS_FACTOR(token.type);
	switch (token.type) {
	case TOKEN_ID:
		expect_id(&name);
		find_name(name,&prop);
		if (token.type == TOKEN_OPEN_BRACKET) {
			ValType type1;
			get_token(&token);
			parse_simple(&type1);
			gen_2(JVM_LDC,token.value);
			expect(TOKEN_CLOSE_BRACKET);
		} else if (token.type == TOKEN_OPEN_PARENTHESIS) {
			get_token(&token);
			if (STARTS_EXPR(token.type)) {
				ValType type1;
				parse_expr(&type1);
				while (token.type == TOKEN_COMMA) {
					expect(TOKEN_COMMA);
					parse_expr(&type1);
				}
			}
			expect(TOKEN_CLOSE_PARENTHESIS);
		} else {
			*type = prop->type;
			gen_2(JVM_ILOAD, prop->offset);
		}
		break;
	case TOKEN_OPEN_PARENTHESIS:
		get_token(&token);
		ValType type11;
		parse_expr(&type11);
		*type = type11;
		expect(TOKEN_CLOSE_PARENTHESIS);
		break;
	case TOKEN_NOT:
		get_token(&token);
		ValType type1;
		gen_2(JVM_LDC,token.value);
		parse_factor(&type1);
		*type = type1;
		gen_1(JVM_IXOR);
		break;
	case TOKEN_NUMBER:
		gen_2(JVM_LDC, token.value);
		get_token(&token);
		*type =  TYPE_INTEGER;
		break;
	case TOKEN_TRUE:
		get_token(&token);
		*type = TYPE_BOOLEAN;
		gen_2(JVM_LDC,1);
		break;
	case TOKEN_FALSE:
		get_token(&token);
		*type = TYPE_BOOLEAN;
		gen_2(JVM_LDC,0);
		break;
	default:
		abort_compile(ERR_FACTOR_EXPECTED, token.type);
		break;
	}
	DBG_end("</factor>");
}
/* --- helper routines ------------------------------------------------------ */

#define MAX_MESSAGE_LENGTH 256

/* TODO: Uncomment the following function for use during type checking. */


void check_types(ValType found, ValType expected, SourcePos *pos, ...)
{
	char buf[MAX_MESSAGE_LENGTH], *s;
	va_list ap;

	if (found != expected) {
		buf[0] = '\0';
		va_start(ap, pos);
		s = va_arg(ap, char *);
		vsnprintf(buf, MAX_MESSAGE_LENGTH, s, ap);
		va_end(ap);
		if (pos != NULL) {
			position = *pos;
		}
		leprintf("incompatible types (expected %s, found %s) %s",
			get_valtype_string(expected), get_valtype_string(found), buf);
	}
}


void expect(TokenType type)
{
	if (token.type == type) {
		get_token(&token);
	} else {
		abort_compile(ERR_EXPECT, type);
	}
}

void expect_id(char **id)
{
	if (token.type == TOKEN_ID) {
		*id = strdup(token.lexeme);
		get_token(&token);
	} else {
		abort_compile(ERR_EXPECT, TOKEN_ID);
	}
}

/* TODO: Uncomment the following two functions for use during type checking. */


IDprop *idprop(ValType type, unsigned int offset, unsigned int nparams,
		ValType *params)
{
	IDprop *ip;

	ip = emalloc(sizeof(IDprop));
	ip->type = type;
	ip->offset = offset;
	ip->nparams = nparams;
	ip->params = params;

	return ip;
}

Variable *variable(char *id, ValType type, SourcePos pos)
{
	Variable *vp;

	vp = emalloc(sizeof(Variable));
	vp->id = id;
	vp->type = type;
	vp->pos = pos;
	vp->next = NULL;

	return vp;
}


/* --- error handling routine ----------------------------------------------- */

void _abort_compile(SourcePos *posp, Error err, va_list args);

void abort_compile(Error err, ...)
{
	va_list args;

	va_start(args, err);
	_abort_compile(NULL, err, args);
	va_end(args);
}

void abort_compile_pos(SourcePos *posp, Error err, ...)
{
	va_list args;

	va_start(args, err);
	_abort_compile(posp, err, args);
	va_end(args);
}

void _abort_compile(SourcePos *posp, Error err, va_list args)
{
	char expstr[MAX_MESSAGE_LENGTH], *s;
	int t;

	if (posp) {
		position = *posp;
	}

	snprintf(expstr, MAX_MESSAGE_LENGTH, "expected %%s, but found %s",
		get_token_string(token.type));

	switch (err) {
		case ERR_ILLEGAL_ARRAY_OPERATION:
		case ERR_MULTIPLE_DEFINITION:
		case ERR_NOT_A_FUNCTION:
		case ERR_NOT_A_PROCEDURE:
		case ERR_NOT_A_VARIABLE:
		case ERR_NOT_AN_ARRAY:
		case ERR_SCALAR_EXPECTED:
		case ERR_TOO_FEW_ARGUMENTS:
		case ERR_TOO_MANY_ARGUMENTS:
		case ERR_UNREACHABLE:
		case ERR_UNKNOWN_IDENTIFIER:
			s = va_arg(args, char *);
			break;
		default:
			break;
	}


	switch (err) {

		/* TODO: Add additional cases here as is necessary, referring to
		 * errmsg.h for all possible errors.  Some errors only become possible
		 * to recognise once we add type checking.  Until you get to type
		 * checking, you can handle such errors by adding the default case.
		 * However, your final submission *must* handle all cases explicitly.
		 */

		case ERR_EXPECT:
			t = va_arg(args, int);
			leprintf(expstr, get_token_string(t));
			break;
		case ERR_FACTOR_EXPECTED:
			leprintf(expstr, "factor");
			break;
		case ERR_UNREACHABLE:
			leprintf("unreachable: %s", s);
			break;
		case ERR_EXPRESSION_OR_STRING_EXPECTED:
			leprintf(expstr, "expression or string");
			break;
		case ERR_STATEMENT_EXPECTED:
			leprintf(expstr, "statement");
			break;
		case ERR_TYPE_EXPECTED:
			leprintf(expstr, "type");
			break;
		case ERR_ARRAY_ALLOCATION_OR_EXPRESSION_EXPECTED:
			leprintf(expstr, "array allocation or expression");
			break;
		case ERR_NOT_A_FUNCTION:
			s = va_arg(args, char *);
			leprintf("\'%s\' is not a function", s);
			break;
		case ERR_ILLEGAL_ARRAY_OPERATION:
			s = va_arg(args, char *);
			leprintf("\'%s\' is an illegal array operation",s);
			break;
		case ERR_SCALAR_EXPECTED:
			s = va_arg(args, char *);
			leprintf("expected scalar variable instead of \'%s\'",s);
			break;
		case ERR_NOT_A_PROCEDURE:
			s = va_arg(args, char *);
			leprintf("\'%s\' is not a procedure",s);
			break;
		case ERR_NOT_A_VARIABLE:
			s = va_arg(args, char *);
			leprintf("\'%s\' is not a variable",s);
			break;
		case ERR_NOT_AN_ARRAY:
			s = va_arg(args, char *);
			leprintf("\'%s\' is not an array",s);
			break;
		case ERR_TOO_FEW_ARGUMENTS:
			s = va_arg(args, char *);
			leprintf("too few arguments for call to \'%s\'",s);
			break;
		case ERR_TOO_MANY_ARGUMENTS:
			s = va_arg(args, char *);
			leprintf("too many arguments for call to \'%s\'",s);
			break;
		case ERR_LEAVE_EXPRESSION_NOT_ALLOWED_FOR_PROCEDURE:
			leprintf("a leave expression is not allowed for a procedure");
			break;
		case ERR_MISSING_LEAVE_EXPRESSION_FOR_FUNCTION:
			leprintf("missing leave expression for a function");
			break;
		case ERR_MULTIPLE_DEFINITION:
			s = va_arg(args, char *);
			leprintf("multiple definitions of \'%s\'",s);
			break;
		case ERR_UNKNOWN_IDENTIFIER:
			leprintf("unknown identifier \'%s\'",s);
			break;
		default:
		break;
	}
}

/* --- debugging output routines -------------------------------------------- */

#ifdef DEBUG_PARSER

static int indent = 0;

void debug_start(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	debug_info(fmt, ap);
	va_end(ap);
	indent += 2;
}

void debug_end(const char *fmt, ...)
{
	va_list ap;

	indent -= 2;
	va_start(ap, fmt);
	debug_info(fmt, ap);
	va_end(ap);
}

void debug_info(const char *fmt, ...)
{
	int i;
	char buf[MAX_MESSAGE_LENGTH], *buf_ptr;
	va_list ap;

	buf_ptr = buf;

	va_start(ap, fmt);

	for (i = 0; i < indent; i++) {
		*buf_ptr++ = ' ';
	}
	vsprintf(buf_ptr, fmt, ap);

	buf_ptr += strlen(buf_ptr);
	snprintf(buf_ptr, MAX_MESSAGE_LENGTH, " in line %d.\n", position.line);
	fflush(stdout);
	fputs(buf, stdout);
	fflush(NULL);

	va_end(ap);
}

#endif /* DEBUG_PARSER */
