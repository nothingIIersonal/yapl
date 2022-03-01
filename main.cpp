#include <iostream>
#include "lexer/yaplex.h"

int main(int argc, char* argv[])
{
	if ( argv[1] == 0 )
	{
		std::cout << "<< Usage: yaplex [source] >>\n";
		return 1;
	}

	std::vector< std::unique_ptr<Token> > tokens;

	if (tokenize(static_cast<std::string>(argv[1]), tokens))
	{
		return 1;
	}

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
	std::cout << std::right << std::setw(15) << "TOTAL: " << tokens.size() << "\n";
	std::cout << "|----------------------------------------------------------|\n";

	return 0;
}