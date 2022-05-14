#pragma once

#define ENTRY "ENTRY" // $_entry
#define EXIT "EXIT" // $_exit

#define VAR "VAR" // var_n
#define FLOAT "FLOAT" // 123., .123, 123.123
#define INTEGER "INTEGER" // 123
#define BOOLEAN "BOOLEAN" // true, false
#define STRING "STRING" // "_#$12$!@%()*^#+_..as\"fnoi13pofasofQ..//\\"
#define CHAR "CHAR" // 'a', '1', '_', ...

#define LINKED_LIST "LINKED_LIST" // linked_list
#define HASH_SET "HASH_SET" // hash_set

#define PRINT "PRINT" // print
#define INPUT "INPUT" // input
#define SLEEP "SLEEP" // sleep
#define STR "STR" 	  // str

#define IF "IF" // if
#define ELIF "ELIF" // elif
#define ELSE "ELSE" // else
#define FOR "FOR" // for
#define WHILE "WHILE" // while
#define DO "DO" // do 
#define BREAK "BREAK" // break
#define RETURN "RETURN" // return

#define FUN "FUN" // fun

#define SEMICOLON "SEMICOLON" // ;
#define COLON "COLON" // :
#define DOT "DOT" // .
#define COMMA "COMMA" // ,
#define LEFT_BRACKET "LEFT_BRACKET" // (
#define RIGHT_BRACKET "RIGHT_BRACKET" // )
#define LEFT_BRACE "LEFT_BRACE" // {
#define RIGHT_BRACE "RIGHT_BRACE" // }
#define LEFT_SQUARE_BRACKETS "LEFT_SQUARE_BRACKETS" // [                //**//   FOR
#define RIGHT_SQUARE_BRACKETS "RIGHT_SQUARE_BRACKETS" // ]				 //**//	  	TYPES
#define COMMENT "COMMENT" // //

#define ASSIGN_OP "ASSIGN_OP" // =
#define ARITHM_OP_PREF "ARITHM_OP_PREF" // +, -
#define ARITHM_OP_UNARY "ARITHM_OP_UNARY" // ++, --, !, ~
#define ARITHM_OP_BINARY "ARITHM_OP_BINARY" // *, /, **, %
#define ARITHM_ASSIGN_OP "ARITHM_ASSIGN_OP" // +=, -=, *=, /=, %=, >>=, <<=, |=, ^=, &=

#define BIT_OP "BIT_OP" // &, |, ^, <<, >>
#define LOGIC_OP "LOGIC_OP" // &&, ||
#define CMP_OP "CMP_OP" // ==, >, <, >=, <=, !=

#define PACK "PACK" // pack
#define FIELDS "FIELDS" // fields
#define INITIAL "INITIAL" // initial

// Embedded structure's terminals (not recongized)
/*	
	add, get, remove, insert, contains
*/

#define _EOF "EOF" // end of file