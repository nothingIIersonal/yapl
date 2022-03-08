#include <iostream>
#include <deque>
#include "lexer/yaplex.h"

#include <ctime>

int main(int argc, char* argv[])
{
	if ( argv[1] == 0 )
	{
		std::cout << "<< Usage: yaplex [source] >>\n";
		return 1;
	}

	std::deque< std::unique_ptr<Token> > tokens;

	float __start_tokenize = clock();
	if (tokenize(static_cast<std::string>(argv[1]), tokens))
	{
		return 1;
	}
	float __end_tokenize = clock();
	float __tokenize_time = 1000 * (__end_tokenize - __start_tokenize) / CLOCKS_PER_SEC;

	std::cout << "|----------------------------------------------------------|\n";
	std::cout << std::right << std::setw(15) << "TOKENS:\n";
	std::cout << "|----------------------------------------------------------|\n\n";
	for ( const auto& token : tokens )
	{
		std::cout << "\t";
		std::cout << std::left << std::setw(25) << "< " + token->get_type() + " >";
		std::cout << std::left << std::setw(25) << "< " + token->get_value() + " >";
		std::cout << std::endl;
		std::cout << std::endl;
	}

	std::cout << "|----------------------------------------------------------|\n";
	std::cout << std::right << std::setw(15) << "TOTAL: " << tokens.size() << "\t";
	std::cout << "TOKENIZE TIME: " << __tokenize_time << " ms.\n";
	std::cout << "|----------------------------------------------------------|\n";

	return 0;
}
