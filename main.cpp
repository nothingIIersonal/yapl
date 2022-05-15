// #define _GLIBCXX_DEBUG

#ifdef __linux__
	#define USE_COLORS
#endif

// #define __OUT_TERMINALS
// #define __DEBUG


#include <iostream>
#include <deque>

#include <ctime>

#include "lexer/yaplex.h"
#include "parser/yapser.h"
#include "interpreter/yapliter.h"


int main(int argc, char* argv[])
{
	if (    argv[1] == nullptr 
		||  strlen(argv[1]) <= 5
		||  (  strlen(argv[1]) >= 6
			&& argv[1][strlen(argv[1]) - 1] != 'l' 
			&& argv[1][strlen(argv[1]) - 2] != 'p' 
			&& argv[1][strlen(argv[1]) - 3] != 'a' 
			&& argv[1][strlen(argv[1]) - 4] != 'y'
			&& argv[1][strlen(argv[1]) - 5] != '.' 
			)
		)
	{
		std::cout << "<< Usage: yapl [source].yapl >>\n";
		return 1;
	}

	std::vector< std::shared_ptr<Token> > tokens;

	float __start_tokenize = clock();
	if (tokenize(static_cast<std::string>(argv[1]), tokens))
	{
		return 1;
	}

	float __end_tokenize = clock();
	float __tokenize_time = 1000 * (__end_tokenize - __start_tokenize) / CLOCKS_PER_SEC;

#ifdef __DEBUG	
	std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
	std::cout << std::right << std::setw(40) << "TOKENS:\n";
	std::cout << "|---------------------------------------------------------------------------------------------------------|\n\n";
	for ( const auto& token : tokens )
	{
		std::cout << "\t";
		std::cout << std::left << std::setw(30) << "< " + token->get_type() + " >";
		std::cout << std::left << std::setw(30) << "< " + token->get_value() + " >";
		std::cout << std::left << std::setw(30) << "< " + std::to_string(token->get_line()) + " >";
		std::cout << std::left << std::setw(30) << "< " + std::to_string(token->get_pos()) + " >";
		std::cout << std::endl;
		std::cout << std::endl;
	}
#endif

	std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
	std::cout << std::right << std::setw(40) << "TOTAL: " << tokens.size() << "\t";
	std::cout << "TOKENIZE TIME: " << __tokenize_time << " ms.\n";
	std::cout << "|---------------------------------------------------------------------------------------------------------|\n\n\n";

	Parser parser(tokens, static_cast<std::string>(argv[1]));

	std::vector<__function> __function_table;
	std::vector<__pack> __pack_table;
	std::string entry_func_name;
	std::string exit_func_name;

	__start_tokenize = clock();

	bool parse_success = parser.parse(__function_table, __pack_table, entry_func_name, exit_func_name);

	__end_tokenize = clock();
	float __parser_time = 1000 * (__end_tokenize - __start_tokenize) / CLOCKS_PER_SEC;		

	if ( parse_success )
	{
		std::cout << "\n\n";
		std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
#ifdef USE_COLORS
		std::cout << "\x1b[32;40m" << std::setw(40) << "CODE IS CORRECT!" 
		          << std::setw(20) << "PARSE TIME: " << __parser_time << " ms."
		          << std::setw(42) << "\x1b[0m\n";
#else
		std::cout << std::setw(40) << "CODE IS CORRECT!" 
		          << std::setw(20) << "PARSE TIME: " << __parser_time << " ms."
		          << std::setw(42) << "\n";
#endif
		std::cout << "|---------------------------------------------------------------------------------------------------------|\n";

#ifdef USE_COLORS
		std::cout << "\n\n\x1b[32;40m" << std::setw(10) << "Starting program..." << "\x1b[0m";
		std::cout << "\n\x1b[32;40m" << std::setw(10) << "^^^^^^^^^^^^^^^^^^^" << "\x1b[0m\n\n";
#else
		std::cout << "\n\n" << std::setw(10) << "Starting program...";
		std::cout << "\n" << std::setw(10) << "^^^^^^^^^^^^^^^^^^^" << "\n\n";
#endif
		for (const auto &ent: __function_table)
		{
			if (ent.__NAME == entry_func_name)
			{
				__start_tokenize = clock();
				execute_rpn(ent.__RPN, __function_table, __pack_table, entry_func_name, {});
				__end_tokenize = clock();
				float __execution_time = 1000 * (__end_tokenize - __start_tokenize) / CLOCKS_PER_SEC;		

				for (const auto &exxt : __function_table)
				{
					if (exxt.__NAME == exit_func_name)
					{
						execute_rpn(exxt.__RPN, __function_table, __pack_table, exit_func_name, {});
						break;
					}
				}
#ifdef USE_COLORS
				std::cout << "\n\n\x1b[32;40m"
				          << "Program executing time (without end() function) -> "
				          << __execution_time
					  << " ms."
				          << "\x1b[0m\n";
#else
				std::cout << "\n\n"
				          << "Program executing time (without end() function) -> "
				          << __execution_time
					  << " ms."
				          << "\n";
#endif

				break;
			}
		}
	}
	else
	{
		std::cout << "\n\n";
		std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
#ifdef USE_COLORS
		std::cout << std::setw(40) << "\x1b[31;1mFAIL!\x1b[0m\n";
#else
		std::cout << std::setw(40) << "FAIL!\n";
#endif
		std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
	}

#ifdef __OUT_TERMINALS
	std::ofstream out;
	out.open("tokens_1.txt");

	if ( !out.is_open() ) 
	{
#ifdef USE_COLORS
		std::cout << "\x1b[31;1mError writing lexer terminals...\x1b[0m\n";
#else
		std::cout << "Error writing lexer terminals...\n";
#endif
		return 1;
	}

	for (auto i : tokens)
	{
		out << (*i).get_value() << std::endl;
	}

	out.close();
#endif

	return 0;
}
