#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <cstdlib>
#include <cstring>
#include <iomanip>



#define ACCUMULATOR_SIZE 256

#define VAR "VAR" // var_n
#define DIGIT "DIGIT" // 123
#define STRING "STRING" // "_#$12$!@%()*^#+_..as\"fnoi13pofasofQ..//\\"

#define IF "IF" // if
#define ELIF "ELIF" // elif
#define FOR "FOR" // for
#define WHILE "WHILE" // while

#define SEMICOLON "SEMICOLON" // ;
#define COLON "COLON" // :
#define APOSTROPHE "APOSTROPHE" // '
#define COMMA "COMMA" // ,
#define LEFT_BRACKET "LEFT_BRACKET" // (
#define RIGHT_BRACKET "RIGHT_BRACKET" // )
#define LEFT_BRACE "LEFT_BRACE" // {
#define RIGHT_BRACE "RIGHT_BRACE" // }
#define LEFT_SQUARE_BRACKETS "LEFT_SQUARE_BRACKETS" // [
#define RIGHT_SQUARE_BRACKETS "RIGHT_SQUARE_BRACKETS" // ]

#define ASSIGN_OP "ASSIGN_OP" // =
#define ADD_OP "ADD_OP" // +
#define SUB_OP "SUB_OP" // -
#define MUL_OP "MUL_OP" // *
#define DIV_OP "DIV_OP" // /
#define POW_OP "POW_OP" // **
#define INC_OP "INC_OP" // ++
#define DEC_OP "DEC_OP" // --
#define EQUAL_OP "EQUAL_OP" // ==
#define GREATER_THAN_OP "GREATER_THAN_OP" // >
#define LESS_THAN_OP "LESS_THAN_OP" // <
#define GREATER_OR_EQUAL_OP "GREATER_OR_EQUAL_OP" // >=
#define LESS_OR_EQUAL_OP "LESS_OR_EQUAL_OP" // <=

#define _EOF "EOF" // end of file



class Token
{
private:
	const std::string _type;
	const std::string _value;

public:
	Token(const std::string& type, const std::string& value) : _type(type), _value(value) {}

	std::string get_type() { return this->_type; }
	std::string get_value() { return this->_value; }
};

const inline void clear_array(char *p, size_t size) { memset(p, 0, size); }
const inline void print_array(const char *p) { std::cout << p; }

const void get_regexps(std::map< std::string, std::shared_ptr<std::regex> >& regexps)
{
	regexps =
	{
		{STRING, std::shared_ptr<std::regex>(
			new std::regex("\"((\\\\.)|[^.\\\\])*\""))},

		{DIGIT, std::shared_ptr<std::regex>(
			new std::regex("0|([1-9][0-9]*)"))},

		{ASSIGN_OP, std::shared_ptr<std::regex>(
			new std::regex("="))},

		{IF, std::shared_ptr<std::regex>(
			new std::regex("if"))},

		{ELIF, std::shared_ptr<std::regex>(
			new std::regex("elif"))},

		{FOR, std::shared_ptr<std::regex>(
			new std::regex("for"))},

		{WHILE, std::shared_ptr<std::regex>(
			new std::regex("while"))},

		{SEMICOLON, std::shared_ptr<std::regex>(
			new std::regex(";"))},

		{COLON, std::shared_ptr<std::regex>(
			new std::regex(":"))},

		{APOSTROPHE, std::shared_ptr<std::regex>(
			new std::regex("\\'"))},

		{COMMA, std::shared_ptr<std::regex>(
			new std::regex("\\,"))},

		{LEFT_BRACKET, std::shared_ptr<std::regex>(
			new std::regex("\\("))},

		{RIGHT_BRACKET, std::shared_ptr<std::regex>(
			new std::regex("\\)"))},

		{LEFT_BRACE, std::shared_ptr<std::regex>(
			new std::regex("\\{"))},

		{RIGHT_BRACE, std::shared_ptr<std::regex>(
			new std::regex("\\}"))},

		{LEFT_SQUARE_BRACKETS, std::shared_ptr<std::regex>(
			new std::regex("\\["))},

		{RIGHT_SQUARE_BRACKETS, std::shared_ptr<std::regex>(
			new std::regex("\\]"))},

		{ADD_OP, std::shared_ptr<std::regex>(
			new std::regex("\\+"))},

		{SUB_OP, std::shared_ptr<std::regex>(
			new std::regex("\\-"))},

		{MUL_OP, std::shared_ptr<std::regex>(
			new std::regex("\\*"))},

		{DIV_OP, std::shared_ptr<std::regex>(
			new std::regex("\\/"))},

		{POW_OP, std::shared_ptr<std::regex>(
			new std::regex("\\*\\*"))},

		{INC_OP, std::shared_ptr<std::regex>(
			new std::regex("\\+\\+"))},

		{DEC_OP, std::shared_ptr<std::regex>(
			new std::regex("\\-\\-"))},

		{EQUAL_OP, std::shared_ptr<std::regex>(
			new std::regex("\\=\\="))},

		{GREATER_THAN_OP, std::shared_ptr<std::regex>(
			new std::regex("\\>"))},

		{LESS_THAN_OP, std::shared_ptr<std::regex>(
			new std::regex("\\<"))},

		{GREATER_OR_EQUAL_OP, std::shared_ptr<std::regex>(
			new std::regex("\\>\\="))},

		{LESS_OR_EQUAL_OP, std::shared_ptr<std::regex>(
			new std::regex("\\<\\="))},

		{VAR, std::shared_ptr<std::regex>(
			new std::regex("[a-zA-Z_][a-zA-Z_0-9]*"))}
	};
}

ssize_t tokenize(const std::string& filepath, std::vector< std::unique_ptr<Token> >& tokens)
{
	if ( filepath.empty() )
	{
		std::cout << "<< Usage: yaplex [source] >>\n";
		return 1;
	}

	std::ifstream __source__file(filepath);
	if ( !__source__file.is_open() )
	{
		std::cout << "<< Can't open file \"" << filepath << "\" >>\n";
		return 1;
	}

	std::cmatch __matches;
	std::map< std::string, std::shared_ptr<std::regex> > regexps;

	get_regexps(regexps);

	char *__accumulator = (char *)calloc(ACCUMULATOR_SIZE, sizeof( char ));
	char temp_ch = 0;

	bool __success = false, __wait_success = true;
	size_t __pos = 0, __line = 1;
	std::string __current_type = { 0 };

	while ( __source__file.get(temp_ch) )
	{
		if ( __pos >= 254 )
		{
			std::cout << "<< ACCUMULATOR OVEFRLOW DETECTED >>\n";

			free(__accumulator);
			__source__file.close();

			return 1;
		}

		if ( __accumulator[0] == 0 && (temp_ch == ' ' || temp_ch == '\t') )
		{
			continue;
		}
		else if ( __accumulator[0] == 0 && temp_ch == '\n' )
		{
			__line++;
			continue;
		}
		else if ( !__success && __accumulator[0] != 0 && __accumulator[__pos - 1] == '\n' )
		{
			__accumulator[__pos - 1] = 0;

			std::cout << filepath << ":" << __line << " error: '" << __accumulator[0] << "' was not declared in this scope\n";
			std::cout << std::left << std::setw(10) << "--------> " << __accumulator << "\n";
			std::cout << std::right << std::setw(11) << '^' << "\n";

			free(__accumulator);
			__source__file.close();

			return 1;
		}

		__accumulator[__pos++] = temp_ch;

		for (const auto& [REGEXP_STR, REGEXP] : regexps)
		{
			if ( std::regex_match(__accumulator, __matches, *REGEXP) )
			{
				__current_type = REGEXP_STR;
				__success = true;

				break;
			}
		}

		if ( __success && __wait_success )
		{
			__success = false;
			__wait_success = false;
			continue;
		}
		else if ( __success && !__wait_success )
		{
			__success = false;
			continue;
		}
		else if ( !__success && __wait_success )
		{
			if ( __source__file.get() == -1 )
			{
				__accumulator[__pos - 1] = temp_ch;

				std::cout << filepath << ":" << __line << " error: '" << __accumulator[0] << "' was not declared in this scope\n";
				std::cout << std::left << std::setw(10) << "--------> " << __accumulator << "\n";
				std::cout << std::right << std::setw(11) << '^' << "\n";

				free(__accumulator);
				__source__file.close();

				return 1;
			}
			__source__file.unget();

			__success = false;
			continue;
		}
		else if ( !__success && !__wait_success )
		{
			__accumulator[__pos - 1] = 0;
			__source__file.unget();

			tokens.push_back( std::make_unique<Token> 
				( __current_type, static_cast<std::string>(__accumulator) )
			);

			clear_array(__accumulator, __pos);

			__success = false;
			__wait_success = true;
			__current_type = { 0 };
			__pos = 0;
		}
	}

	if ( __source__file.eof() && __current_type[0] != 0 )
	{
		tokens.push_back( std::make_unique<Token> 
			( __current_type, static_cast<std::string>(__accumulator) )
		);
	}

	tokens.push_back( std::make_unique<Token>
		(_EOF, static_cast<std::string>(_EOF))
	);

	free(__accumulator);
	__source__file.close();

	return 0;
}