#pragma once


#include <iostream>
#include <fstream>
#include <regex>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <cinttypes>

#include "../terminals.h"


#define ACCUMULATOR_SIZE 256


class Token
{
private:
	const std::string _type;
	const std::string _value;
	const uint64_t _line;
	const uint64_t _pos;

public:
	Token(const std::string& type, const std::string& value, const uint64_t line, const uint64_t pos) : _type(type), _value(value), _line(line), _pos(pos) {}

	std::string get_type() { return this->_type; }
	std::string get_value() { return this->_value; }
	uint64_t get_line() { return this->_line; }
	uint64_t get_pos() { return this->_pos; }
};

const inline void clear_array(char *p, size_t size) { memset(p, 0, size); }
const inline void print_array(const char *p) { std::cout << p; }

const void get_regexps(std::vector< std::pair<std::string, std::shared_ptr<std::regex>> >& regexps)
{
	regexps =
	{
		{ENTRY, std::shared_ptr<std::regex>(
			new std::regex("\\$_entry"))},

		{EXIT, std::shared_ptr<std::regex>(
			new std::regex("\\$_exit"))},

		{STRING, std::shared_ptr<std::regex>(
			new std::regex("\"((\\\\.)|[^.\\\\])*\""))},

		{CHAR, std::shared_ptr<std::regex>(
			new std::regex("\'((\\\\.)|[^.\\\\])\'"))},

		{LINKED_LIST, std::shared_ptr<std::regex>(
			new std::regex("linked_list"))},

		{HASH_SET, std::shared_ptr<std::regex>(
			new std::regex("hash_set"))},

		{PRINT, std::shared_ptr<std::regex>(
			new std::regex("print"))},

		{INPUT, std::shared_ptr<std::regex>(
			new std::regex("input"))},

		{SLEEP, std::shared_ptr<std::regex>(
			new std::regex("sleep"))},

		{STR, std::shared_ptr<std::regex>(
			new std::regex("str"))},

		{FLOAT, std::shared_ptr<std::regex>(
			new std::regex("([0-9]+\\.[0-9]*)|(\\.[0-9]+)"))},

		{INTEGER, std::shared_ptr<std::regex>(
			new std::regex("([0-9]*)|([0-9]*)"))},

		{BOOLEAN, std::shared_ptr<std::regex>(
			new std::regex("true|false"))},

		{ASSIGN_OP, std::shared_ptr<std::regex>(
			new std::regex("="))},

		{IF, std::shared_ptr<std::regex>(
			new std::regex("if"))},

		{ELIF, std::shared_ptr<std::regex>(
			new std::regex("elif"))},

		{ELSE, std::shared_ptr<std::regex>(
			new std::regex("else"))},

		{FOR, std::shared_ptr<std::regex>(
			new std::regex("for"))},

		{WHILE, std::shared_ptr<std::regex>(
			new std::regex("while"))},

		{DO, std::shared_ptr<std::regex>(
			new std::regex("do"))},

		{BREAK, std::shared_ptr<std::regex>(
			new std::regex("break"))},

		{RETURN, std::shared_ptr<std::regex>(
			new std::regex("return"))},

		{FUN, std::shared_ptr<std::regex>(
			new std::regex("fun"))},

		{SEMICOLON, std::shared_ptr<std::regex>(
			new std::regex(";"))},

		{COLON, std::shared_ptr<std::regex>(
			new std::regex(":"))},

		{DOT, std::shared_ptr<std::regex>(
			new std::regex("\\."))},

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

		{COMMENT, std::shared_ptr<std::regex>(
			new std::regex("(\\/\\/)[^\n]*$"))},

		{ARITHM_ASSIGN_OP, std::shared_ptr<std::regex>(
			new std::regex( "(\\+\\=)|(\\-\\=)|(\\*\\=)"
				           "|(\\/\\=)|(\\%\\=)"
				           "|(\\<\\<\\=)|(\\>\\>\\=)"
				           "|(\\|\\=)|(\\^\\=)|(\\&\\=)"))},

		{ARITHM_OP_PREF, std::shared_ptr<std::regex>(
			new std::regex("[\\+]|[\\-]"))},

		{ARITHM_OP_UNARY, std::shared_ptr<std::regex>(
			new std::regex("(\\+\\+)|(\\-\\-)|(\\!)|(\\~)"))},

		{ARITHM_OP_BINARY, std::shared_ptr<std::regex>(
			new std::regex("[\\*]|[\\/]|(\\*\\*)|(\\%)"))},

		{BIT_OP, std::shared_ptr<std::regex>(
			new std::regex("(\\&)|(\\|)|(\\^)|(\\<\\<)|(\\>\\>)"))},

		{LOGIC_OP, std::shared_ptr<std::regex>(
			new std::regex("(\\&\\&)|(\\|\\|)"))},

		{CMP_OP, std::shared_ptr<std::regex>(
			new std::regex("(\\=\\=)|(\\>)|(\\<)|(\\>\\=)|(\\<\\=)|(\\!\\=)"))},

		{PACK, std::shared_ptr<std::regex>(
			new std::regex("pack"))},

		{FIELDS, std::shared_ptr<std::regex>(
			new std::regex("fields"))},

		{INITIAL, std::shared_ptr<std::regex>(
			new std::regex("initial"))},

		{VAR, std::shared_ptr<std::regex>(
			new std::regex("[a-zA-Z_][a-zA-Z_0-9]*"))}
	};
}

int8_t tokenize(const std::string& filepath, std::vector< std::shared_ptr<Token> >& tokens)
{
	std::ifstream __source__file(filepath);
	if ( !__source__file.is_open() )
	{
		std::cout << "<< Can't open file \"" << filepath << "\" >>\n";
		return 1;
	}

	std::cmatch __matches;
	std::vector< std::pair<std::string, std::shared_ptr<std::regex>> > regexps;

	get_regexps(regexps);

	char *__accumulator = (char *)calloc(ACCUMULATOR_SIZE, sizeof( char ));
	char temp_ch = 0;

	bool __success = false, __wait_success = true;
	uint64_t __pos = 0, __abs_pos = 1, __line = 1;
	std::string __current_type = { 0 };

#ifdef __DEBUG
	size_t __cycle_number = 0;
#endif

	while ( __source__file.get(temp_ch) )
	{
		if ( __pos >= 254 )
		{
			std::cout << "\x1b[31;1m<< ACCUMULATOR OVEFRLOW DETECTED >>\x1b[0m\n";

			free(__accumulator);
			__source__file.close();

			return 1;
		}

		if ( temp_ch == '\t' )
		{
			__abs_pos += 4;
		}
		else
		{
			__abs_pos++;
		}

#ifdef __DEBUG
		print_array(__accumulator); std::cout << "|\n";
#endif

		if ( __accumulator[0] == 0 && (temp_ch == ' ' || temp_ch == '\t') )
		{
			continue;
		}
		else if ( __accumulator[0] == 0 && temp_ch == '\n' )
		{
			__abs_pos = 1;
			__line++;
			continue;
		}
		else if ( !__success && __accumulator[0] != 0 && __accumulator[__pos - 1] == '\n' )
		{
			__accumulator[__pos - 1] = 0;

			std::cout << "\x1b[31;1m" << filepath << ":" << __line << " error: '" << __accumulator[0] << "' was not declared in this scope\n";
			std::cout << std::left << std::setw(10) << "--------> " << __accumulator << "\n";
			std::cout << std::right << std::setw(11) << '^' << "\x1b[0m\n";

			free(__accumulator);
			__source__file.close();

			return 1;
		}

		__accumulator[__pos++] = temp_ch;

		for (const auto& regex : regexps)
		{
			if ( std::regex_match(__accumulator, __matches, *regex.second) )
			{
				__current_type = regex.first;
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
			
			if ( __abs_pos )
			{
				__abs_pos--;
			}

			tokens.push_back( std::move(std::make_shared<Token> 
				( __current_type, static_cast<std::string>(__accumulator), __line, __abs_pos ))
			);

			clear_array(__accumulator, __pos);

			__success = false;
			__wait_success = true;
			__current_type = { 0 };
			__pos = 0;

#ifdef __DEBUG
			std::cout << "|------------------------" << __cycle_number++ << " CYCLE------------------------|\n";
#endif
		}
	}

	if ( __source__file.eof() && __current_type[0] != 0 )
	{
		tokens.push_back( std::move(std::make_shared<Token> 
			( __current_type, static_cast<std::string>(__accumulator), __line, __abs_pos ))
		);
	}

	tokens.push_back( std::move(std::make_shared<Token>
		( _EOF, static_cast<std::string>(_EOF), __line, __abs_pos ))
	);

	free(__accumulator);
	__source__file.close();

	return 0;
}
