#include <iostream>
#include <fstream>
// #include <inttypes.h>
#include <regex>
#include <cstdlib>
#include <cstring>
#include <iomanip>

// typedef uint64_t u64;
// typedef uint8_t u8;

#define ACCUMULATOR_SIZE 256

#define VAR "VAR" // var_n
#define DIGIT "DIGIT" // 123
#define STRING "STRING" // "_#$12$!@%()*^#+_..asfnoi13pofasofQ..//\\"

#define ASSIGN_OP "ASSIGN_OP" // =
#define IF "IF" // if
#define ELIF "ELIF" // elif
#define FOR "FOR" // for
#define WHILE "WHILE" // while

#define SEMICOLON "SEMICOLON" // ;
// #define QUOTE "QUOTE" // "
#define APOSTROPHE "APOSTROPHE" // '
#define COMMA "COMMA" // ,

#define LEFT_BRACKET "LEFT_BRACKET" // (
#define RIGHT_BRACKET "RIGHT_BRACKET" // )
#define LEFT_BRACE "LEFT_BRACE" // {
#define RIGHT_BRACE "RIGHT_BRACE" // }
#define LEFT_SQUARE_BRACKETS "LEFT_SQUARE_BRACKETS" // [
#define RIGHT_SQUARE_BRACKETS "RIGHT_SQUARE_BRACKETS" // ]

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

const inline void clear_array(char *p, size_t size) { memset(p, 0, size); }
const inline void print_array(char *p) { std::cout << p; }

class Token
{
private:
	std::string _type;
	std::string _value;

public:
	Token(std::string type, std::string value) : _type(type), _value(value) {}

	std::string get_type() { return this->_type; }
	std::string get_value() { return this->_value; }
};

int main(int argc, char *argv[])
{

	std::cmatch _matches;

	std::regex OPEN_FILE_regex("[a-zA-Z0-9\\_\\.]+");

	if (!std::regex_match(argv[1], _matches, OPEN_FILE_regex))
	{
		std::cout << "<< Usage: yaplex [source] >>\n";
		return 1;
	}

	std::regex VAR_regex("[a-zA-Z_][a-zA-Z_0-9]*");
	std::regex DIGIT_regex("0|([1-9][0-9]*)");
	std::regex STRING_regex("(\"[a-zA-Z0-9\\.\\/\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)_\\+\\-\\=\\<\\>\\{\\}\\'\\|\\s\\,]*)|([[a-zA-Z0-9\\.\\/\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)_\\+\\-\\=\\<\\>\\{\\}\\'\\|\\s\\,]*\"])|(\"[a-zA-Z0-9\\.\\/\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)_\\+\\-\\=\\<\\>\\{\\}\\'\\|\\s\\,]*\")");

	std::regex ASSIGN_OP_regex("=");
	std::regex IF_regex("if");
	std::regex ELIF_regex("elif");
	std::regex FOR_regex("for");
	std::regex WHILE_regex("while");

	std::regex SEMICOLON_regex(";");
	// std::regex QUOTE_regex("\\\"");
	std::regex APOSTROPHE_regex("\\'");
	std::regex COMMA_regex("\\,");

	std::regex LEFT_BRACKET_regex("\\(");
	std::regex RIGHT_BRACKET_regex("\\)");	
	std::regex LEFT_BRACE_regex("\\{");
	std::regex RIGHT_BRACE_regex("\\}");
	std::regex LEFT_SQUARE_BRACKETS_regex("\\[");
	std::regex RIGHT_SQUARE_BRACKETS_regex("\\]");

	std::regex ADD_OP_regex("\\+");
	std::regex SUB_OP_regex("\\-");
	std::regex MUL_OP_regex("\\*");
	std::regex DIV_OP_regex("\\/");
	std::regex POW_OP_regex("\\*\\*");
	std::regex INC_OP_regex("\\+\\+");
	std::regex DEC_OP_regex("\\-\\-");
	std::regex EQUAL_OP_regex("\\=\\=");
	std::regex GREATER_THAN_OP_regex("\\>");
	std::regex LESS_THAN_OP_regex("\\<");
	std::regex GREATER_OR_EQUAL_OP_regex("\\>=");
	std::regex LESS_OR_EQUAL_OP_regex("\\<=");

	std::vector< std::pair<std::regex, std::string> > regexps
	{
		std::make_pair(STRING_regex, STRING),
		std::make_pair(DIGIT_regex, DIGIT),
		std::make_pair(ASSIGN_OP_regex, ASSIGN_OP),
		std::make_pair(IF_regex, IF),
		std::make_pair(ELIF_regex, ELIF),
		std::make_pair(FOR_regex, FOR),
		std::make_pair(WHILE_regex, WHILE),
		std::make_pair(SEMICOLON_regex, SEMICOLON),
		// std::make_pair(QUOTE_regex, QUOTE),
		std::make_pair(APOSTROPHE_regex, APOSTROPHE),
		std::make_pair(COMMA_regex, COMMA),
		std::make_pair(LEFT_BRACKET_regex, LEFT_BRACKET),
		std::make_pair(RIGHT_BRACKET_regex, RIGHT_BRACKET),
		std::make_pair(LEFT_BRACE_regex, LEFT_BRACE),
		std::make_pair(RIGHT_BRACE_regex, RIGHT_BRACE),
		std::make_pair(LEFT_SQUARE_BRACKETS_regex, LEFT_SQUARE_BRACKETS),
		std::make_pair(RIGHT_SQUARE_BRACKETS_regex, RIGHT_SQUARE_BRACKETS),
		std::make_pair(ADD_OP_regex, ADD_OP),
		std::make_pair(SUB_OP_regex, SUB_OP),
		std::make_pair(MUL_OP_regex, MUL_OP),
		std::make_pair(DIV_OP_regex, DIV_OP),
		std::make_pair(POW_OP_regex, POW_OP),
		std::make_pair(INC_OP_regex, INC_OP),
		std::make_pair(DEC_OP_regex, DEC_OP),
		std::make_pair(EQUAL_OP_regex, EQUAL_OP),
		std::make_pair(GREATER_THAN_OP_regex, GREATER_THAN_OP),
		std::make_pair(LESS_THAN_OP_regex, LESS_THAN_OP),
		std::make_pair(GREATER_OR_EQUAL_OP_regex, GREATER_OR_EQUAL_OP),
		std::make_pair(LESS_OR_EQUAL_OP_regex, LESS_OR_EQUAL_OP),
		std::make_pair(VAR_regex, VAR)
	};

	std::vector< std::shared_ptr<Token> > tokens;

	char *__accumulator = (char *)calloc(ACCUMULATOR_SIZE, sizeof( char ));
	char temp_ch = 0;

	std::ifstream __source__file(argv[1]);
	if ( !__source__file.is_open() )
	{
		std::cout << "<< Can't open file \"" << argv[1] << "\">>\n";
		return 1;
	}

	bool _unknown_ident = false;
	while ( !__source__file.eof() )
	{
		static size_t __cycle__number = 0;

		std::string _current_type = { 0 };
		for (size_t i = 0; ; i++)
		{
			if ( !__source__file.get(temp_ch) )
			{
				break;
			}

			__accumulator[i] = temp_ch;

			if (__accumulator[i] == ' ' && std::regex_match(__accumulator, _matches, STRING_regex)) {}
			else if (__accumulator[i] == ' ' || __accumulator[i] == '\n' ||
				__accumulator[i] == '\0' || __accumulator[i] == '\t')
			{
				i--;
				continue;
			}

			print_array(__accumulator); std::cout << '|' << std::endl;
			
			bool __no_one = false;
			for (size_t j = 0; j < regexps.size(); j++)
			{
				if (std::regex_match(__accumulator, _matches, regexps[j].first))
				{
					_current_type = regexps[j].second;
					_unknown_ident = false;
					break;
				}

				if (j == regexps.size() - 1)
				{
					__no_one = true;
				}
			}

			if (__no_one)
			{
				if ( _unknown_ident )
				{
					std::cout << "<< SYNTAX ERROR: UNKNOWN SYMBOL \"" << temp_ch << "\" >>\n";
					return 1;
				}

				__accumulator[i] = 0;

				tokens.push_back( std::make_shared<Token> 
					(_current_type, static_cast<std::string>(__accumulator))
				);

				clear_array(__accumulator, i);

				__source__file.unget();

				_unknown_ident = true;

				break;
			}
		}

		std::cout << "<-------- " << __cycle__number++ << " CYCLE -------->\n";
	}
	tokens.push_back( std::make_shared<Token>
		(_EOF, static_cast<std::string>(_EOF))
	);

	__source__file.close();
	free(__accumulator);

	std::cout << "\n\nTOKENS:\n"
					"|----------------------------------------------------------|\n";
	for (auto token : tokens)
	{
		std::cout << "\t";
		std::cout << std::left << std::setw(30) << "< '" + token->get_type() + "' >";
		std::cout << std::left << std::setw(30) << "< '" + token->get_value() + "' >";
		std::cout << std::endl;			
		std::cout << "|----------------------------------------------------------|";
		std::cout << std::endl;	
	}

	return 0;
}