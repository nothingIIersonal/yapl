#include <iostream>
#include <deque>
#include "lexer/yaplex.h"
#include "parser/yapser.h"

#include <ctime>

int main(int argc, char* argv[])
{
	if ( argv[1] == 0 )
	{
		std::cout << "<< Usage: yaplex [source] >>\n";
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

	std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
	std::cout << std::right << std::setw(40) << "TOTAL: " << tokens.size() << "\t";
	std::cout << "TOKENIZE TIME: " << __tokenize_time << " ms.\n";
	std::cout << "|---------------------------------------------------------------------------------------------------------|\n\n\n";

	Parser parser(tokens, static_cast<std::string>(argv[1]));
	
	if ( parser.parse() )
	{
		std::cout << "\n\n";
		std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
		std::cout << std::setw(40) << "CODE IS CORRECT!\n";
		std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
	}
	else
	{
		std::cout << "\n\n";
		std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
		std::cout << std::setw(40) << "FAIL!\n";
		std::cout << "|---------------------------------------------------------------------------------------------------------|\n";
	}

	return 0;
}
