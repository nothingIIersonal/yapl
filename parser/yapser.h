#pragma once

#include <iostream>

#include "../terminals.h"

class Parser
{
private:
	std::vector< std::shared_ptr<Token> > __tokens;
	std::vector< std::shared_ptr<Token> > ::iterator __current_token_iterator;
	std::shared_ptr<Token> __last_after_token;
	std::shared_ptr<Token> __last_token;

	std::string __filepath;
	std::string __current_non_terminal;

public:
	Parser(std::vector< std::shared_ptr<Token> >& tokens, std::string filepath);

	bool push_iterator(std::vector< std::shared_ptr<Token> > ::iterator);
	std::vector< std::shared_ptr<Token> > ::iterator get_iterator();

	bool lang();
	bool exit();
	bool expr();
	bool entry();
	bool assign_expr();
	bool cycle_expr();
	bool if_expr();
	bool frame_expr();
	bool embedded_func_expr();
	bool func_expr();
	bool l_value();
	bool r_value_expr();
	bool embedded_structure();
	bool print_func_expr();
	bool input_func_expr();
	bool r_value();
	bool value_expr();
	bool var_expr();
	bool arithm_expr();
	bool str_expr();
	bool var_expr_binding();
	bool op_for_var_expr();
	bool arithm_expr_binding();
	bool op_for_arithm_expr();
	bool str_expr_binding();
	bool num_expr();
	bool func_call();
	bool do_while_cycle_expr();
	bool while_cycle_expr();
	bool for_cycle_expr();
	bool cond_expr();
	bool for_cond_expr();
	bool param_func_expr();
	bool param_func_call_expr();
	bool return_expr();
	bool var_arithm_expr();

	bool ___EOF();
	bool ___ENTRY();
	bool ___LEFT_BRACKET();
	bool ___VAR();
	bool ___RIGHT_BRACKET();
	bool ___EXIT();
	bool ___SEMICOLON();
	bool ___COMMENT();
	bool ___BREAK();
	bool ___ASSIGN_OP();
	bool ___ARITHM_ASSIGN_OP();
	bool ___LEFT_SQUARE_BRACKETS();
	bool ___RIGHT_SQUARE_BRACKETS();
	bool ___PRINT();
	bool ___COMMA();
	bool ___INPUT();
	bool ___STRING();
	bool ___LINKED_LIST();
	bool ___HASH_SET();
	bool ___ARITHM_OP_PREF();
	bool ___ARITHM_OP_UNARY();
	bool ___ARITHM_OP_BINARY();
	bool ___BIT_OP();
	bool ___LOGIC_OP();
	bool ___CMP_OP();
	bool ___FLOAT();
	bool ___INTEGER();
	bool ___CHAR();
	bool ___BOOLEAN();
	bool ___IF();
	bool ___ELIF();
	bool ___ELSE();
	bool ___DO();
	bool ___WHILE();
	bool ___FOR();
	bool ___FUN();
	bool ___LEFT_BRACE();
	bool ___RIGHT_BRACE();
	bool ___RETURN();

	bool check_terminal(std::string);

	bool parse();
};

Parser::Parser(std::vector< std::shared_ptr<Token> >& tokens, std::string filepath) : __tokens(tokens), __current_token_iterator(__tokens.begin()), __filepath(filepath) {}

std::vector< std::shared_ptr<Token> > ::iterator Parser::get_iterator()
{
	return this->__current_token_iterator;
}

bool Parser::push_iterator(std::vector< std::shared_ptr<Token> > ::iterator new_iter)
{
	this->__current_token_iterator = new_iter;

	return true;
}

bool Parser::parse()
{
	if ( !lang() )
	{
		std::cout << "______________________________________________________________________________________________________________________________________________________________\n\n"
		 		  << __filepath << ":" << __last_after_token->get_line()
		 		  << ":" << __last_after_token->get_pos() << " error: " 
				  << "unexpected token '" << __last_after_token->get_type() << "'\n";
		std::cout << std::left << std::setw(10) << "--------> " << __last_after_token->get_value() << "\n";
		std::cout << std::right << std::setw(11) << '^' << "\n";

		return false;
	}

	std::cout << "______________________________________________________________________________________________________________________________________________________________\n\n";

	return true;
}


// NON-TERMINALS

bool Parser::lang()
{
	this->__current_non_terminal = "lang";

	entry();

	exit();

	auto __cur__iter = get_iterator();

	while ( expr() ) { __cur__iter = get_iterator(); }

	if ( ___EOF() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::exit()
{
	this->__current_non_terminal = "exit";

	auto __cur__iter = get_iterator();

	if ( ___EXIT() && ___LEFT_BRACKET() && ___VAR() && ___RIGHT_BRACKET() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::expr()
{
	this->__current_non_terminal = "expr";

	auto __cur__iter = get_iterator();

	if ( 
		   ( 			  assign_expr() && ___SEMICOLON() )   
		|| ( push_iterator(__cur__iter) && r_value_expr() && ___SEMICOLON() )
		|| ( push_iterator(__cur__iter) && cycle_expr() )
		|| ( push_iterator(__cur__iter) && if_expr() )
		|| ( push_iterator(__cur__iter) && frame_expr() )
		|| ( push_iterator(__cur__iter) && embedded_func_expr() && ___SEMICOLON() )
		|| ( push_iterator(__cur__iter) && func_expr() )
		|| ( push_iterator(__cur__iter) && ___COMMENT() )
		|| ( 				 ___BREAK()	&& ___SEMICOLON() )
		|| ( ___RETURN() && return_expr() && ___SEMICOLON() )
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::entry()
{
	this->__current_non_terminal = "entry";

	auto __cur__iter = get_iterator();

	if ( ___ENTRY() && ___LEFT_BRACKET() && ___VAR() && ___RIGHT_BRACKET() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::assign_expr()
{
	this->__current_non_terminal = "assign_expr";

	auto __cur__iter = get_iterator();

	if ( ( l_value() && (___ASSIGN_OP() || ___ARITHM_ASSIGN_OP()) && r_value_expr() )
		 || 
		 ( push_iterator(__cur__iter) && l_value() && ___ASSIGN_OP() && 
		 ___LEFT_SQUARE_BRACKETS() && embedded_structure() && ___RIGHT_SQUARE_BRACKETS())
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::cycle_expr()
{
	this->__current_non_terminal = "cycle_expr";

	auto __cur__iter = get_iterator();

	if (   do_while_cycle_expr() 
		|| ( push_iterator(__cur__iter) && while_cycle_expr() )
		|| ( push_iterator(__cur__iter)&& for_cycle_expr() ) 
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::if_expr()
{
	this->__current_non_terminal = "if_expr";

	auto __cur__iter = get_iterator();

	if ( ___IF() && cond_expr() && frame_expr() )
	{
		__cur__iter = get_iterator();
		while ( ___ELIF() && cond_expr() && frame_expr() ) { __cur__iter = get_iterator(); }
		push_iterator(__cur__iter);

		if ( !(___ELSE() && frame_expr()) )
		{
			push_iterator(__cur__iter);
		}

		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::frame_expr()
{
	this->__current_non_terminal = "frame_expr";

	auto __cur__iter = get_iterator();

	if ( ___LEFT_BRACE() )
	{
		while ( expr() ) { __cur__iter = get_iterator(); }

		if ( ___RIGHT_BRACE() )
		{
			return true;
		}
	
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::embedded_func_expr()
{
	this->__current_non_terminal = "embedded_func_expr";

	auto __cur__iter = get_iterator();

	if ( print_func_expr() || ( push_iterator(__cur__iter) && input_func_expr() ) )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::func_expr()
{
	this->__current_non_terminal = "func_expr";

	auto __cur__iter = get_iterator();

	if ( ___FUN() && ___VAR() && ___LEFT_BRACKET() )
	{
		param_func_expr();

		if ( ___RIGHT_BRACKET() && frame_expr() )
		{
			return true;
		}
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::l_value()
{
	this->__current_non_terminal = "l_value";

	auto __cur__iter = get_iterator();

	if ( ___VAR() || ( push_iterator(__cur__iter) && ___LEFT_BRACKET() && l_value() && ___RIGHT_BRACKET() ) )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::r_value_expr()
{
	this->__current_non_terminal = "r_value_expr";

	auto __cur__iter = get_iterator();

	if (  func_call()
	   || ( push_iterator(__cur__iter) && value_expr() )
	   || ( push_iterator(__cur__iter) && ___LEFT_BRACKET() && r_value_expr() && ___RIGHT_BRACKET() ) 
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::embedded_structure()
{
	this->__current_non_terminal = "embedded_structure";

	auto __cur__iter = get_iterator();

	if ( ___LINKED_LIST() || ___HASH_SET() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::print_func_expr()
{
	this->__current_non_terminal = "print_func_expr";

	auto __cur__iter = get_iterator();

	if ( ___PRINT() && ___LEFT_BRACKET() )
	{
		if (r_value())
		{
			__cur__iter = get_iterator();
			while ( ___COMMA() && r_value() ) { __cur__iter = get_iterator(); }
			push_iterator(__cur__iter);
		}

		if ( ___RIGHT_BRACKET() )
		{
			return true;
		}
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::input_func_expr()
{
	this->__current_non_terminal = "input_func_expr";

	auto __cur__iter = get_iterator();

	if ( ___INPUT() && ___LEFT_BRACKET() && l_value() )
	{
		if ( ___COMMA() )
		{
			___STRING();
		}

		if ( ___RIGHT_BRACKET() )
		{
			return true;
		} 
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::r_value()
{
	this->__current_non_terminal = "r_value";

	auto __cur__iter = get_iterator();

	if ( func_call() || ( push_iterator(__cur__iter) && ___VAR() ) || ___FLOAT() || ___INTEGER() || ___BOOLEAN() ||
		 ___STRING() || ___CHAR() ||
		 ( ___LEFT_BRACKET() && r_value() && ___RIGHT_BRACKET() )
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::value_expr()
{
	this->__current_non_terminal = "value_expr";

	auto __cur__iter = get_iterator();

	if (   var_expr() 
		|| ( push_iterator(__cur__iter) && arithm_expr() ) 
		|| ( push_iterator(__cur__iter) && str_expr() ) 
		|| ( push_iterator(__cur__iter) && ___LEFT_BRACKET() && value_expr() && ___RIGHT_BRACKET() )
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::var_expr()
{
	this->__current_non_terminal = "var_expr";

	auto __cur__iter = get_iterator();

	if ( var_expr_binding() )
	{
		__cur__iter = get_iterator();
		while ( op_for_var_expr() && var_arithm_expr() ) { __cur__iter = get_iterator(); }
		push_iterator(__cur__iter);

		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::arithm_expr()
{
	this->__current_non_terminal = "arithm_expr";

	auto __cur__iter = get_iterator();

	if ( arithm_expr_binding() )
	{
		__cur__iter = get_iterator();
		while ( op_for_arithm_expr() && var_arithm_expr() ) { __cur__iter = get_iterator(); }
		push_iterator(__cur__iter);

		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::str_expr()
{
	this->__current_non_terminal = "str_expr";

	auto __cur__iter = get_iterator();

	if ( str_expr_binding() )
	{
		__cur__iter = get_iterator();
		while ( ___ARITHM_OP_PREF() && str_expr() ) { __cur__iter = get_iterator(); }
		push_iterator(__cur__iter);

		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::var_expr_binding()
{
	this->__current_non_terminal = "var_expr_binding";

	auto __cur__iter = get_iterator();

	while ( ___ARITHM_OP_PREF() ) {}
	while ( ___ARITHM_OP_UNARY() ) {}

	if ( l_value() )
	{
		___ARITHM_OP_UNARY();
		return true;
	}

	push_iterator(__cur__iter);

	while ( ___ARITHM_OP_PREF() ) {}
	if ( ___LEFT_BRACKET() )
	{
		while ( ___ARITHM_OP_UNARY() ) {}

		if ( l_value() && ___RIGHT_BRACKET() )
		{
			___ARITHM_OP_UNARY();

			return true;
		}
	}

	push_iterator(__cur__iter);

	if ( ___LEFT_BRACKET() && var_expr() && ___RIGHT_BRACKET() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::op_for_var_expr()
{
	this->__current_non_terminal = "op_for_var_expr";

	auto __cur__iter = get_iterator();

	if ( op_for_arithm_expr() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::arithm_expr_binding()
{
	this->__current_non_terminal = "arithm_expr_binding";

	auto __cur__iter = get_iterator();

	while ( ___ARITHM_OP_PREF() ) {}
	if ( num_expr() || (___LEFT_BRACKET() && arithm_expr() && ___RIGHT_BRACKET()) )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::op_for_arithm_expr()
{
	this->__current_non_terminal = "op_for_arithm_expr";

	if ( ___ARITHM_OP_PREF() || ___ARITHM_OP_BINARY() || ___BIT_OP() || ___LOGIC_OP() || ___CMP_OP() )
	{
		return true;
	}

	return false;
}

bool Parser::str_expr_binding()
{
	this->__current_non_terminal = "str_expr_binding";

	auto __cur__iter = get_iterator();

	if ( ___STRING() || (___LEFT_BRACKET() && str_expr() && ___RIGHT_BRACKET()) )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::num_expr()
{
	this->__current_non_terminal = "num_expr";

	auto __cur__iter = get_iterator();

	if ( ___FLOAT() || ___INTEGER() || ___CHAR() || ___BOOLEAN() ||
		 (___LEFT_BRACKET() && num_expr() && ___RIGHT_BRACKET())
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::func_call()
{
	this->__current_non_terminal = "func_call";

	auto __cur__iter = get_iterator();

	if ( ___VAR() && ___LEFT_BRACKET() )
	{
		param_func_call_expr();

		if ( ___RIGHT_BRACKET() )
		{
			return true;
		}
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::do_while_cycle_expr()
{
	this->__current_non_terminal = "do_while_cycle_expr";

	auto __cur__iter = get_iterator();

	if ( ___DO() && frame_expr() && ___WHILE() && cond_expr() && ___SEMICOLON() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::while_cycle_expr()
{
	this->__current_non_terminal = "while_cycle_expr";

	auto __cur__iter = get_iterator();

	if ( ___WHILE() && cond_expr() && frame_expr() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::for_cycle_expr()
{
	this->__current_non_terminal = "for_cycle_expr";

	auto __cur__iter = get_iterator();

	if ( ___FOR() && for_cond_expr() && frame_expr() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::cond_expr()
{
	this->__current_non_terminal = "cond_expr";

	auto __cur__iter = get_iterator();

	if ( ___LEFT_BRACKET() && r_value_expr() && ___RIGHT_BRACKET() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::for_cond_expr()
{
	this->__current_non_terminal = "for_cond_expr";

	auto __cur__iter = get_iterator();

	if ( ___LEFT_BRACKET() )
	{
		assign_expr();

		if ( ___SEMICOLON() )
		{
			r_value_expr();

			if ( ___SEMICOLON() )
			{
				if ( r_value_expr() )
				{
					__cur__iter = get_iterator();
					while ( ___COMMA() && r_value_expr() ) { __cur__iter = get_iterator(); }
					push_iterator(__cur__iter);
				}

				if ( ___RIGHT_BRACKET() )
				{
					return true;
				}
			}
		}
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::param_func_expr()
{
	this->__current_non_terminal = "param_func_expr";

	auto __cur__iter = get_iterator();

	if ( ___VAR() )
	{
		__cur__iter = get_iterator();
		while ( ___COMMA() && ___VAR() ) { __cur__iter = get_iterator(); }
		push_iterator(__cur__iter);

		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::param_func_call_expr()
{
	this->__current_non_terminal = "param_func_call_expr";

	auto __cur__iter = get_iterator();

	if ( r_value() )
	{
		__cur__iter = get_iterator();
		while ( ___COMMA() && r_value() ) { __cur__iter = get_iterator(); }
		push_iterator(__cur__iter);

		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::return_expr()
{
	this->__current_non_terminal = "return_expr";

	auto __cur__iter = get_iterator();

	if ( assign_expr() || ( push_iterator(__cur__iter) && r_value_expr() ) )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::var_arithm_expr()
{
	this->__current_non_terminal = "return_expr";

	auto __cur__iter = get_iterator();

	if ( var_expr() || (push_iterator(__cur__iter) && arithm_expr()) )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

// TERMINALS

bool Parser::___EOF()
{
	return check_terminal(_EOF);
}

bool Parser::___ENTRY()
{
	return check_terminal(ENTRY);
}

bool Parser::___LEFT_BRACKET()
{
	return check_terminal(LEFT_BRACKET);
}

bool Parser::___VAR()
{
	return check_terminal(VAR);
}

bool Parser::___RIGHT_BRACKET()
{
	return check_terminal(RIGHT_BRACKET);
}

bool Parser::___EXIT()
{
	return check_terminal(EXIT);
}

bool Parser::___SEMICOLON()
{
	return check_terminal(SEMICOLON);
}

bool Parser::___COMMENT()
{
	return check_terminal(COMMENT);
}

bool Parser::___BREAK()
{
	return check_terminal(BREAK);
}

bool Parser::___ASSIGN_OP()
{
	return check_terminal(ASSIGN_OP);
}

bool Parser::___ARITHM_ASSIGN_OP()
{
	return check_terminal(ARITHM_ASSIGN_OP);
}

bool Parser::___LEFT_SQUARE_BRACKETS()
{
	return check_terminal(LEFT_SQUARE_BRACKETS);
}

bool Parser::___RIGHT_SQUARE_BRACKETS()
{
	return check_terminal(RIGHT_SQUARE_BRACKETS);
}

bool Parser::___PRINT()
{
	return check_terminal(PRINT);
}

bool Parser::___COMMA()
{
	return check_terminal(COMMA);
}

bool Parser::___INPUT()
{
	return check_terminal(INPUT);
}

bool Parser::___STRING()
{
	return check_terminal(STRING);
}

bool Parser::___LINKED_LIST()
{
	return check_terminal(LINKED_LIST);
}

bool Parser::___HASH_SET()
{
	return check_terminal(HASH_SET);
}

bool Parser::___ARITHM_OP_PREF()
{
	return check_terminal(ARITHM_OP_PREF);
}

bool Parser::___ARITHM_OP_UNARY()
{
	return check_terminal(ARITHM_OP_UNARY);
}

bool Parser::___ARITHM_OP_BINARY()
{
	return check_terminal(ARITHM_OP_BINARY);
}

bool Parser::___BIT_OP()
{
	return check_terminal(BIT_OP);
}

bool Parser::___LOGIC_OP()
{
	return check_terminal(LOGIC_OP);
}

bool Parser::___CMP_OP()
{
	return check_terminal(CMP_OP);
}

bool Parser::___FLOAT()
{
	return check_terminal(FLOAT);
}

bool Parser::___INTEGER()
{
	return check_terminal(INTEGER);
}

bool Parser::___CHAR()
{
	return check_terminal(CHAR);
}

bool Parser::___BOOLEAN()
{
	return check_terminal(BOOLEAN);
}

bool Parser::___IF()
{
	return check_terminal(IF);
}

bool Parser::___ELIF()
{
	return check_terminal(ELIF);
}

bool Parser::___ELSE()
{
	return check_terminal(ELSE);
}

bool Parser::___DO()
{
	return check_terminal(DO);
}

bool Parser::___WHILE()
{
	return check_terminal(WHILE);
}

bool Parser::___FOR()
{
	return check_terminal(FOR);
}

bool Parser::___FUN()
{
	return check_terminal(FUN);
}

bool Parser::___LEFT_BRACE()
{
	return check_terminal(LEFT_BRACE);
}

bool Parser::___RIGHT_BRACE()
{
	return check_terminal(RIGHT_BRACE);
}

bool Parser::___RETURN()
{
	return check_terminal(RETURN);
}


// Check terminal correct

bool Parser::check_terminal(std::string terminal)
{
	bool __check = ((*(this->__current_token_iterator))->get_type() == terminal);
	
	if ( __check )
	{
		std::cout << std::setw(23) << "\"" + terminal + "\""
				  << std::setw(8) << " is now "
				  << std::setw(1) << "\"" 
				  << std::setw(5) << (__check ? "true" : "false") << "\""
				  << std::setw(6) << " for "
				  << std::setw(50) << "\"" + ((*(this->__current_token_iterator))->get_value()) + "\""
				  << std::setw(11) << " in line "
				  << std::setw(5) << ((*(this->__current_token_iterator))->get_line())
				  << std::setw(15) << " on position "
				  << std::setw(5) << ((*(this->__current_token_iterator))->get_pos()) 
				  << std::setw(10) << " ---> " << this->__current_non_terminal << std::endl;

		if ( __current_token_iterator != __tokens.end() )
		{
			__last_token = *__current_token_iterator;
		}

		++__current_token_iterator;

		if ( __current_token_iterator != __tokens.end() )
		{
			__last_after_token = *__current_token_iterator;
		}
	}

	return __check;
}