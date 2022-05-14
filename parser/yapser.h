#pragma once

#include <iostream>
#include <cinttypes>
#include <queue>
#include <stack>
#include <cassert>

#include "../terminals.h"
#include "../symbols/symbols.h"
#include "../interpreter/yapliter.h"


typedef struct __true_info
{
	std::string __TERMINAL;
	std::string __VALUE;
	uint64_t __LINE;
	uint64_t __POS;
	std::string __NON_TERMINAL;
} __true_info;


class Parser
{
private:
	std::vector< std::shared_ptr<Token> > __tokens;
	std::vector< std::shared_ptr<Token> > ::iterator __current_token_iterator;

	std::shared_ptr<Token> __last_after_token;
	std::shared_ptr<Token> __last_token;

	std::string __filepath;
	std::string __current_non_terminal;

	std::vector < std::pair<std::string, std::string> > __trues;
	std::vector <std::string> __trues_terminals;

	std::vector <__true_info> __trues_information;
	std::vector<__true_info> ::iterator token_it;

	std::vector <__function> __function_table;
	std::vector <__pack> __pack_table;
	std::vector <__var> __var_table;

	std::string entry_func_name;
	std::string exit_func_name;

	uint64_t __last_line;
	uint64_t __last_pos;


public:
	Parser(std::vector< std::shared_ptr<Token> >& tokens, std::string filepath);

	bool push_iterator(std::vector< std::shared_ptr<Token> > ::iterator);

	std::vector< std::shared_ptr<Token> > ::iterator get_iterator();

	bool lang();
	bool exit();
	bool expr();
	bool entry();
	bool assign_expr();
	bool linked_list_control_expr();
	bool linked_list_add_expr();
	bool linked_list_get_expr();
	bool linked_list_remove_expr();
	bool linked_list_insert_expr();	
	bool hash_set_control_expr();
	bool cycle_expr();
	bool if_expr();
	bool frame_expr();
	bool embedded_func_expr();
	bool sleep_func_expr();
	bool func_expr();
	bool l_value();
	bool r_value_expr();
	bool embedded_structure();
	bool print_func_expr();
	bool input_func_expr();
	bool str_func_expr();
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
	bool pack_expr();
	bool pack_frame_expr();
	bool pack_structure();
	bool embedded_structure_call();
	bool pack_call();
	bool pack_method_call();
	bool pack_field_call();
	bool array_expr();

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
	bool ___SLEEP();
	bool ___COMMA();
	bool ___INPUT();
	bool ___STR();
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
	bool ___DOT();
	bool ___COLON();
	bool ___PACK();
	bool ___FIELDS();
	bool ___INITIAL();

	bool ___ADD();
	bool ___GET();
	bool ___REMOVE();
	bool ___INSERT();
	bool ___CONTAINS();

	bool check_terminal(const std::string &);

	bool parse(std::vector<__function> &, std::vector<__pack> &, std::string &, std::string &);

	void print_stack(std::stack<std::string>);

	void print_queue(const std::vector<std::string> &);

	uint8_t get_pri(const std::string &);

	std::vector<__rpn_node> get_rpn(std::string);

	bool search_in_pack_table(const std::string &);

	std::vector<__pack>::iterator get_pack_from_table(const std::string &);

	bool search_in_var_table(const std::string &);

	std::vector<__var>::iterator get_var_from_table(const std::string &);

	bool search_field_in_pack(const std::string &, const std::string &);

	std::vector<__var>::iterator get_field_from_pack(const std::string &, const std::string &);

	bool search_method_in_pack(const std::string &, const std::string &);

	std::vector<__function>::iterator get_method_from_pack(const std::string &, const std::string &);

	void force_quit(std::string);
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

void Parser::force_quit(std::string msg)
{
	std::cout << msg;
	abort();
}

uint8_t Parser::get_pri(const std::string &op)
{
	if (   op == "="
		|| op == "+="
		|| op == "-="
		|| op == "*="
		|| op == "/="
		|| op == "%="
		|| op == ">>="
		|| op == "<<="
		|| op == "|="
		|| op == "^="
		|| op == "&="
		|| op == "("
		|| op == "if"
		|| op == "elif"
		|| op == "else"
		|| op == "while"
		|| op == "return"
		|| op == "["
		|| op[op.size() - 1] == ')')
	{
		return 0;
	}
	else if (op == "||")
	{
		return 1;
	}
	else if (op == "&&")
	{
		return 2;
	}
	else if (op == "|")
	{
		return 3;
	}
	else if (op == "^")
	{
		return 4;
	}
	else if (op == "&")
	{
		return 5;
	}
	else if (   op == "=="
			 || op == "!=")
	{
		return 6;
	}
	else if (   op == "<"
			 || op == ">"
			 || op == ">="
			 || op == "<=")
	{
		return 7;
	}	
	else if (   op == "<<"
			 || op == ">>")
	{
		return 8;
	}


	else if (    op == "+"
			  || op == "-")
	{
		return 9;
	}
	else if (   op == "*"
			 || op == "/"
			 || op == "%")
	{
		return 10;
	}
	else if (op == "**")
	{
		return 11;
	}
	else if (   op == "+_PREF" 
		     || op == "-_PREF"
		     || op == "~"
		     || op == "!"
		     || op == "++"
		     || op == "--")
	{
		return 12;
	}
	else
	{
		return 14;
	}
}

void Parser::print_stack(std::stack<std::string> stack)
{
	std::cout << "STACK: ";
	while (!stack.empty())
	{
		std::cout << stack.top() << ", ";
		stack.pop();
	}
	std::cout << "\n";
}

void Parser::print_queue(const std::vector<std::string> &queue)
{
	std::cout << "QUEUE: ";
	for (auto i : queue)
	{
		std::cout << i << ", ";
	}
	std::cout << "\n";
}

bool Parser::search_in_pack_table(const std::string &pack_name)
{
	for (const auto &i : __pack_table)
		if (i.__NAME == pack_name)
			return true;

	return false;
}

std::vector<__pack>::iterator Parser::get_pack_from_table(const std::string &pack_name)
{
	uint64_t itm = 0;
	for (auto &i : __pack_table)
		if (++itm && (i.__NAME == pack_name))
			break;

	return __pack_table.begin() + itm - 1;
}

bool Parser::search_in_var_table(const std::string &var_name)
{
	for (const auto &i : __var_table)
		if (i.__NAME == var_name)
			return true;

	return false;
}

std::vector<__var>::iterator Parser::get_var_from_table(const std::string &var_name)
{
	uint64_t itm = 0;
	for (auto &i : __var_table)
		if (++itm && (i.__NAME == var_name))
			break;

	return __var_table.begin() + itm - 1;
}

bool Parser::search_field_in_pack(const std::string &pack_name, const std::string &field_name)
{
	for (const auto &pack : __pack_table)
		if (pack.__NAME == pack_name)
			for (const auto &field : pack.__FIELDS)
				if (field.__NAME == field_name)
					return true;

	return false;
}

std::vector<__var>::iterator Parser::get_field_from_pack(const std::string &pack_name, const std::string &field_name)
{
	uint64_t itm_pack = 0, itm_field = 0;
	for (const auto &pack : __pack_table)
	{
#ifdef __DEBUG
		std::cout << "There is pack '" << pack.__NAME << "'\n";		
#endif
		if (++itm_pack && pack.__NAME == pack_name)
		{
			for (const auto &field : pack.__FIELDS)
			{
#ifdef __DEBUG
				std::cout << "There is field '" << field.__NAME << "'\n";
#endif
				if (++itm_field && field.__NAME == field_name)
				{
					break;
				}
			}
			break;
		}
	}

	std::vector<__pack>::iterator p_i = __pack_table.begin() + itm_pack - 1;
	std::vector<__var>::iterator v_i = p_i->__FIELDS.begin() + itm_field - 1;

#ifdef __DEBUG
	std::cout << "\n" << p_i->__NAME << " | " << v_i->__NAME << " <<<<<<<<<<<<<<<<\n\n";
#endif

	return v_i;
}

bool Parser::search_method_in_pack(const std::string &pack_name, const std::string &method_name)
{
	for (const auto &pack : __pack_table)
		if (pack.__NAME == pack_name)
			for (const auto &method : pack.__METHODS)
				if (method.__NAME == method_name)
					return true;

	return false;
}

std::vector<__function>::iterator Parser::get_method_from_pack(const std::string &pack_name, const std::string &method_name)
{
	uint64_t itm_pack = 0, itm_method = 0;
	for (const auto &pack : __pack_table)
	{
#ifdef __DEBUG
		std::cout << "There is pack '" << pack.__NAME << "'\n";		
#endif
		if (++itm_pack && pack.__NAME == pack_name)
		{
			for (const auto &method : pack.__METHODS)
			{
#ifdef __DEBUG
				std::cout << "There is method '" << method.__NAME << "'\n";
#endif
				if (++itm_method && method.__NAME == method_name)
				{
					break;
				}
			}
			break;			
		}
	}

	std::vector<__pack>::iterator p_i = __pack_table.begin() + itm_pack - 1;
	std::vector<__function>::iterator m_i = p_i->__METHODS.begin() + itm_method - 1;

#ifdef __DEBUG
	std::cout << "\n" << p_i->__NAME << " | " << m_i->__NAME << " <<<<<<<<<<<<<<<<\n\n";
#endif

	return m_i;
}

std::vector<__rpn_node> Parser::get_rpn(std::string pack_name = "")   // { ... }
{
	std::vector<__rpn_node> rpn;

	std::vector<std::string> output_queue;
	std::stack<std::string> stack;

	uint64_t l_brace_cnt = 1;
	uint64_t funcs_called = 0;
	uint64_t p_pos = 0;
	uint64_t arr_size = 0;
	uint64_t print_args = 0;
	uint64_t IF_EXIT = 0;
	uint64_t NEXT_COND = 0;

	bool expr_flag = false;
	bool arr_flag = false;
	bool pack_flag = false;
	bool print_flag = false;
	bool pack_method_flag = false;
	bool frame_end = false;

	std::vector<uint64_t> _CJMP;
	std::vector<uint64_t> _JMP;
	std::vector<uint64_t> _CJMP_all;
	std::vector<uint64_t> _JMP_all;
	std::vector<uint64_t> _WHILE_COND;
	
	std::vector<__var>::iterator _PACK_CHAIN;
	std::vector<__var>::iterator _FOUND_VAR_FOR_PACK;

	std::queue<std::string> __pack_fields;

#ifdef __DEBUG
	std::cout << "MAKING RPN...\n";
#endif

	while (true)
	{
		++token_it;

		if (token_it->__TERMINAL == "COMMENT")
		{
			continue;
		}


		// print_queue(output_queue);	
		// print_stack(stack);
		// std::cout << "\n";


		if (frame_end && !stack.empty() && stack.top() == "while")
		{
			frame_end = false;

			stack.pop();

			output_queue[_JMP.back()] = std::to_string(_WHILE_COND.back());
			output_queue[_CJMP.back()] = std::to_string(output_queue.size() + 1);

			_WHILE_COND.pop_back();
			_JMP.pop_back();
			_CJMP.pop_back();
		}
		else if (frame_end && token_it->__VALUE != "if" && token_it->__VALUE != "elif" && token_it->__VALUE != "else")
		{
			frame_end = false;

			output_queue.pop_back();
			output_queue.pop_back();

			_JMP.pop_back();
			_JMP_all.pop_back();

			--p_pos;

			if (!stack.empty() && stack.top() == "else")
			{
				stack.pop();
			}
			
			IF_EXIT = output_queue.size() + 1;
			NEXT_COND = output_queue.size() + 1;
			while (!stack.empty() && stack.top() != "if")
			{
				while (!_JMP.empty() && (_JMP.back() > _CJMP.back()))
				{
					output_queue[_JMP.back()] = std::to_string(IF_EXIT);
					NEXT_COND = _JMP.back() + 3;
					_JMP.pop_back();
				}

				output_queue[_CJMP.back()] = std::to_string(NEXT_COND);
				_CJMP.pop_back();
				stack.pop();
			}

			while (!_JMP.empty() && (_JMP.back() > _CJMP.back()))
			{
				output_queue[_JMP.back()] = std::to_string(IF_EXIT);
				NEXT_COND = _JMP.back() + 3;
				_JMP.pop_back();
			}

			output_queue[_CJMP.back()] = std::to_string(NEXT_COND);
			_CJMP.pop_back();
			stack.pop();
		}
		else if (frame_end)
		{
			frame_end = false;
		}

		if (token_it->__VALUE == "{")
		{
			++l_brace_cnt;
			continue;
		}
		else if (token_it->__VALUE == "}")
		{
			uint64_t vt_erased = 0;
			uint64_t vt_size = __var_table.size();
			for (uint64_t i = 0; i < vt_size; i++)
			{
				if (__var_table[i - vt_erased].__SCOPE >= l_brace_cnt)
				{
					output_queue.push_back((__var_table.begin() + i - vt_erased)->__NAME);
					output_queue.push_back("~~");										
					__var_table.erase(__var_table.begin() + i - vt_erased);

					++vt_erased;
				}
			}

			if (!stack.empty() && (   stack.top() == "if" || stack.top() == "elif" || stack.top() == "else"
								   || stack.top() == "while"))
			{
				_JMP.push_back(output_queue.size());
				_JMP_all.push_back(output_queue.size());

				output_queue.push_back("p" + std::to_string(++p_pos));
				output_queue.push_back("!JMP");				
				frame_end = true;
			}

			--l_brace_cnt;

			if (l_brace_cnt == 0)
			{
				while (!stack.empty())
				{
					output_queue.push_back(stack.top());

					stack.pop();
				}

				break;
			}

			continue;
		}
		else if (token_it->__VALUE == ";")
		{
			while (!stack.empty() && stack.top() != "if" && stack.top() != "elif" && stack.top() != "else"
				                  && stack.top() != "while")
			{
				output_queue.push_back(stack.top());

				if (pack_method_flag && stack.top()[0] == '!')
				{
					pack_method_flag = false;
				//	output_queue.push_back(".");
				}

				stack.pop();
			}
		}

		if (token_it->__VALUE == ",")
		{
			if (arr_flag)
			{
				++arr_size;
			}

			if (print_flag)
			{
				++print_args;
			}

			while (   stack.top()[stack.top().size() - 1] != '(' 
				   && stack.top()[stack.top().size() - 1] != '[')
			{
				output_queue.push_back(stack.top());
				stack.pop();
			}
			continue;
		}

		if (token_it->__VALUE == "if" || token_it->__VALUE == "elif" || token_it->__VALUE == "else")
		{
			stack.push(token_it->__VALUE);

			continue;
		}


		if (token_it->__VALUE == "[")
		{
			stack.push(token_it->__VALUE);
			
			arr_flag = true;
			pack_flag = true;
			
			continue;
		}
		else if (token_it->__VALUE == "." && arr_flag && pack_flag)
		{
			stack.pop();

			arr_flag = false;

			continue;
		}
		else if (token_it->__VALUE == "]" && arr_flag)
		{
			while (stack.top() != "[")
			{
				output_queue.push_back(stack.top());
				stack.pop();
			}
			
			stack.pop();

			arr_size += arr_size ? 1 : 0;

			output_queue.push_back(std::to_string(arr_size));
			output_queue.push_back("[]");

			arr_size = 0;

			arr_flag = false;

			continue;
		}
		else if (token_it->__VALUE == "]" && pack_flag)
		{
			pack_flag = false;
			continue;
		}


		if (token_it->__TERMINAL == "ARITHM_OP_PREF" && expr_flag)
		{
			expr_flag = false;
			token_it->__TERMINAL = "ARITHM_OP_BINARY";
		}
		else if (expr_flag)
		{
			expr_flag = false;
		}
		else if (token_it->__TERMINAL == "ARITHM_OP_PREF")
		{
			token_it->__VALUE += "_PREF";
		}


		if (   token_it->__NON_TERMINAL == "pack_field_call" || token_it->__NON_TERMINAL == "pack_field_call_(BREAK)"
			|| token_it->__NON_TERMINAL == "pack_call" || token_it->__NON_TERMINAL == "pack_method_call"
			|| token_it->__NON_TERMINAL == "pack_method_call_(BREAK)" || token_it->__NON_TERMINAL == "pack_field_call_first")
		{
	
		}


		if ((   token_it->__NON_TERMINAL == "func_call"
			 || token_it->__NON_TERMINAL == "input_func_expr"
			 || token_it->__NON_TERMINAL == "sleep_func_expr"
			 || token_it->__NON_TERMINAL == "str_func_expr")
			&& token_it->__VALUE != "(")
		{
			if (token_it->__NON_TERMINAL == "func_call")
			{
				std::string t_name = token_it->__VALUE;
				uint64_t t_line = token_it->__LINE;
				uint64_t t_pos = token_it->__POS;

				bool _ok = false;
				for (const auto &i : __function_table)
				{
					if (i.__NAME == token_it->__VALUE)
					{
						_ok = true;
						break;
					}
				}

				if (!_ok)
				{
					force_quit("\n\x1b[31;1mUndefined function '" 
					             + t_name
					             + "' " 
					             + std::to_string(t_line)
					             + ":"
					             + std::to_string(t_pos)
					             + " \x1b[0m\n\n");
				}
			}

			++funcs_called;
			stack.push(token_it->__VALUE + "()");
		}
		else if (token_it->__NON_TERMINAL == "print_func_expr" && token_it->__VALUE != "(")
		{
			++funcs_called;
			stack.push(token_it->__VALUE + "()");
			print_flag = true;
		}
		else if ((   token_it->__NON_TERMINAL == "pack_structure"
				  || token_it->__NON_TERMINAL == "embedded_structure")
				 && token_it->__VALUE != "(")
		{
			++funcs_called;

			stack.push( (token_it->__NON_TERMINAL == "pack_structure") 
						 ?
						 ("." + token_it->__VALUE + "()")
						 :
						 (":" + token_it->__VALUE + "()")
			          );

			if (token_it->__NON_TERMINAL != "pack_structure")
			{
				_FOUND_VAR_FOR_PACK->__IS_OBJECT = false;
				_FOUND_VAR_FOR_PACK->__PACK.__NAME = "linked_list";
			}
			else
			{
				_FOUND_VAR_FOR_PACK->__IS_OBJECT = true;
			}

			if (token_it->__NON_TERMINAL == "embedded_structure")
			{
				continue;
			}

#ifdef __DEBUG
			if (pack_name != "")
			{
				std::cout << "\nPack: "
						  << pack_name
						  << " | Searching field: "
						  << _FOUND_VAR_FOR_PACK->__NAME
						  << " | Search result: "
						  << search_field_in_pack(pack_name, _FOUND_VAR_FOR_PACK->__NAME)
						  << "\n\n";
			}
#endif

			if (pack_name != "" && search_field_in_pack(pack_name, _FOUND_VAR_FOR_PACK->__NAME))
				get_field_from_pack(pack_name, _FOUND_VAR_FOR_PACK->__NAME)->__IS_OBJECT = true;

			if (!search_in_pack_table(token_it->__VALUE))
			{
				force_quit("\n\x1b[31;1mUndefined pack '" 
				             + token_it->__VALUE
				             + "' " 
				             + std::to_string(token_it->__LINE)
				             + ":"
				             + std::to_string(token_it->__POS)
				             + " \x1b[0m\n\n");				
			}	
			
			_FOUND_VAR_FOR_PACK->__PACK = *get_pack_from_table(token_it->__VALUE);
			
			if (pack_name != "" && search_field_in_pack(pack_name, _FOUND_VAR_FOR_PACK->__NAME))
				get_field_from_pack(pack_name, _FOUND_VAR_FOR_PACK->__NAME)->__PACK = *get_pack_from_table(token_it->__VALUE);

#ifdef __DEBUG
			if (pack_name != "" && search_field_in_pack(pack_name, _FOUND_VAR_FOR_PACK->__NAME))
			{
				std::cout << "In search:\n"
						  << "Field name: "
						  << get_field_from_pack(pack_name, _FOUND_VAR_FOR_PACK->__NAME)->__NAME
						  << " | Is pack? : "
						  << get_field_from_pack(pack_name, _FOUND_VAR_FOR_PACK->__NAME)->__IS_OBJECT
						  << " | Pack name: "
						  << get_field_from_pack(pack_name, _FOUND_VAR_FOR_PACK->__NAME)->__PACK.__NAME
						  << "\n\n";
			}
#endif

		}
		else if (token_it->__NON_TERMINAL == "pack_method_call" && token_it->__VALUE != "(")
		{
			pack_method_flag = true;			
			++funcs_called;

			if (!_PACK_CHAIN->__IS_OBJECT && _PACK_CHAIN->__PACK.__NAME == "linked_list")
			{
				if (   token_it->__VALUE == "add" || token_it->__VALUE == "get" 
			        || token_it->__VALUE == "remove" || token_it->__VALUE == "insert")
				{
					pack_method_flag = true;			
					++funcs_called;
					stack.push("!" + token_it->__VALUE + "()");
					continue;
				}
				else
				{
					force_quit("\n\x1b[31;1mUnknown method for linked list '" 
					             + token_it->__VALUE
					             + "' " 
					             + std::to_string(token_it->__LINE)
					             + ":"
					             + std::to_string(token_it->__POS)
					             + " \x1b[0m\n\n");
				}
			}
			else
			{
				stack.push("!" + token_it->__VALUE + "()");
			}

#ifdef __DEBUG
			std::cout << "\nReceived method '" << token_it->__VALUE << "'"
					  << " for field '" << _PACK_CHAIN->__NAME << "'\n\n";
#endif

			if (!_PACK_CHAIN->__IS_OBJECT || !search_method_in_pack(_PACK_CHAIN->__PACK.__NAME, token_it->__VALUE))
				force_quit("\n\x1b[31;1mUndefined method '" 
							+ token_it->__VALUE
							+ "' " 
							+ std::to_string(token_it->__LINE)
							+ ":"
							+ std::to_string(token_it->__POS)
							+ " \x1b[0m\n\n");
		}
		// else if (token_it->__NON_TERMINAL == "pack_method_call_(BREAK)" && token_it->__VALUE != "(")
		// {
		// 	if (token_it->__VALUE != ".")
		// 	{
		// 		if (__pack_fields.size() % 2 == 0 && __pack_fields.size() != 0)
		// 		{
		// 			//__pack_fields.push(".");
		// 		}

		// 		__pack_fields.push(token_it->__VALUE);
		// 	}

		// 	continue;
		// }
		else if (  token_it->__TERMINAL == "VAR" 
				|| token_it->__TERMINAL == "FLOAT" 
				|| token_it->__TERMINAL == "INTEGER" 
				|| token_it->__TERMINAL == "BOOLEAN" 
				|| token_it->__TERMINAL == "STRING"
				|| token_it->__TERMINAL == "CHAR")
		{
			if (token_it->__TERMINAL == "VAR")
			{
				std::string t_name = token_it->__VALUE;
				uint64_t t_line = token_it->__LINE;
				uint64_t t_pos = token_it->__POS;

				if (token_it->__NON_TERMINAL == "pack_field_call_first")
					_PACK_CHAIN = get_var_from_table(token_it->__VALUE);

				if (token_it->__NON_TERMINAL == "pack_field_call")
				{
#ifdef __DEBUG					
					std::cout << "Current token: " << token_it->__VALUE
							  << " | Previous chain: " << _PACK_CHAIN->__NAME
							  << " | Is pack? : " << _PACK_CHAIN->__IS_OBJECT
							  << "\n\n";
#endif
					if (!search_field_in_pack(_PACK_CHAIN->__PACK.__NAME, token_it->__VALUE))
					{
						force_quit("\n\x1b[31;1mUndefined field '" 
						             + token_it->__VALUE
						             + "' " 
						             + std::to_string(t_line)
						             + ":"
						             + std::to_string(t_pos)
						             + " \x1b[0m\n\n");
					}

					for (auto field = _PACK_CHAIN->__PACK.__FIELDS.begin();
					     field != _PACK_CHAIN->__PACK.__FIELDS.end();
					     field++)
					{
						if (field->__NAME == token_it->__VALUE && field->__IS_OBJECT)
						{
							_PACK_CHAIN = field;
#ifdef __DEBUG							
							std::cout << "Field found: " << field->__NAME
									  << " | Is pack? : " << field->__IS_OBJECT
									  << " | Pack name: " << field->__PACK.__NAME
									  << "\n\n";
#endif
							break;
						}
					}
				}

				if (!(   token_it->__NON_TERMINAL == "pack_field_call"
					//|| token_it->__NON_TERMINAL == "pack_method_call_(BREAK)"
					  || token_it->__NON_TERMINAL == "pack_method_call"
					//|| token_it->__NON_TERMINAL == "pack_call"
					//|| token_it->__NON_TERMINAL == "pack_field_call_first"
					//|| token_it->__NON_TERMINAL == "pack_field_call_(BREAK)"
					 )
					)
				{
					if (++token_it != __trues_information.end())
					{
						if (token_it->__VALUE == "=" && !search_in_var_table(t_name))
						{
							__var_table.push_back({t_name, "0", l_brace_cnt});
							_FOUND_VAR_FOR_PACK = get_var_from_table(t_name);
						}
						else if (!search_in_var_table(t_name))
						{
							force_quit("\n\x1b[31;1mUndefined variable '" 
							             + t_name
							             + "' " 
							             + std::to_string(t_line)
							             + ":"
							             + std::to_string(t_pos)
							             + " \x1b[0m\n\n");
						}
						else if (search_in_var_table(t_name))
						{
							_FOUND_VAR_FOR_PACK = get_var_from_table(t_name);
						}
					}

					--token_it;
				}
			}

			expr_flag = true;
			output_queue.push_back(token_it->__VALUE);
		}
		else if (token_it->__VALUE == "(")
		{
			stack.push(token_it->__VALUE);
		}
		else if (token_it->__VALUE == ")")
		{
			expr_flag = true;

			while (stack.top() != "(")
			{
				output_queue.push_back(stack.top());
				stack.pop();
			}
			stack.pop();

			uint64_t __pack_fields_size = __pack_fields.size();
			while (!__pack_fields.empty())
			{
				output_queue.push_back(__pack_fields.front());
				__pack_fields.pop();
			}

			if (pack_method_flag && (__pack_fields_size >= 2))
			{
				output_queue.push_back(".");
			}

			if (   stack.top() == "if" || stack.top() == "elif" 
				|| stack.top() == "while" || stack.top() == "do" || stack.top() == "for")	
			{
				_CJMP.push_back(output_queue.size());
				_CJMP_all.push_back(output_queue.size());

				output_queue.push_back("p" + std::to_string(++p_pos));
				output_queue.push_back("!F");
			}

			if (print_flag)
			{
				print_flag = false;
				output_queue.push_back(std::to_string(++print_args));
				print_args = 0;
			}

			if (funcs_called)
			{
				--funcs_called;
			}
		}
		else if (token_it->__VALUE != ";")
		{

			while (!stack.empty() 
					&& (     (get_pri(stack.top()) > get_pri(token_it->__VALUE))
						  || (get_pri(stack.top()) >= get_pri(token_it->__VALUE) 
						  	  && stack.top() == "." && token_it->__VALUE == ".")) 
					&& stack.top() != "(")
			{				
				output_queue.push_back(stack.top());
				stack.pop();
			}

			if (token_it->__VALUE == "while")
			{
				_WHILE_COND.push_back(output_queue.size() + 1);
			}

			stack.push(token_it->__VALUE);
		}
	}

	output_queue.push_back("ERPN");

	uint64_t pos = 0;
	for (auto i : output_queue)
	{
		rpn.push_back({++pos, i});
	}

#ifdef __DEBUG
	std::cout << "\n\nCREATED RPN << \n";
	for (auto &i : rpn)
	{
		std::cout << std::setw(20) << "< " << i.__POSITION << ": " << i.__VALUE << " >\n";
	}

	std::cout << "\n";
#endif

	return rpn;
}

bool Parser::parse(std::vector<__function> &f_table, std::vector<__pack> &p_table, std::string &ent_f_name, std::string &ex_f_name)
{
#ifdef __DEBUG
	std::cout << "______________________________________________________________________________________________________________________________________________________________\n\n";
#endif

	if ( !lang() )
	{
		std::cout << "______________________________________________________________________________________________________________________________________________________________\n\n"
		 		  << "\x1b[31;1m"
		 		  << this->__filepath << ":" << this->__last_after_token->get_line()
		 		  << ":" << this->__last_after_token->get_pos() << " error: " 
				  << "unexpected token '" << this->__last_after_token->get_type() << "'\n";
		std::cout << std::left << std::setw(10) << "--------> " << this->__last_after_token->get_value() << "\n";
		std::cout << std::right << std::setw(11) << '^' << "\x1b[0m\n";
		std::cout << "______________________________________________________________________________________________________________________________________________________________\n\n";

		return false;
	}

#ifdef __DEBUG
	std::cout << "______________________________________________________________________________________________________________________________________________________________\n\n";
	for (auto &i : this->__trues)
	{
		std::cout << i.first << i.second << "\n";
	}
	std::cout << "______________________________________________________________________________________________________________________________________________________________\n\n";		

	for (auto &i : this->__trues_terminals)
	{
		std::cout << i << "\n";
	}
	std::cout << "______________________________________________________________________________________________________________________________________________________________\n\n";			
#endif

	for (token_it = this->__trues_information.begin();
		 token_it < this->__trues_information.end(); token_it++)
	{
		if (token_it->__VALUE == "$_entry")
		{
			++token_it;
			++token_it;			
			entry_func_name = token_it->__VALUE;
			++token_it;

#ifdef __DEBUG
			std::cout << "ENTRY FOUND << " << entry_func_name << "\n";
#endif			
		}

		if (token_it->__VALUE == "$_exit")
		{
			++token_it;
			++token_it;			
			exit_func_name = token_it->__VALUE;
			++token_it;

#ifdef __DEBUG
			std::cout << "EXIT FOUND << " << exit_func_name << "\n";
#endif						
		}

		if (token_it->__VALUE == "pack")
		{
			std::string pack_name = (++token_it)->__VALUE;
			++token_it;

			uint64_t fields_cnt = 0;
			__function init_fnc;
			std::vector<__var> fields;
			std::vector<__function> methods;	

#ifdef __DEBUG
			std::cout << "PACK FOUND << " << pack_name << "\n";
#endif

			__pack_table.push_back({});
			__pack_table.back().__NAME = pack_name;

			__var_table.clear();

			while (true)
			{
				if ((++token_it)->__VALUE == "}")
				{
#ifdef __DEBUG
					std::cout << "\nCurrent pack: " << pack_name
							  << " | Fields count: " << fields_cnt
							  << "\n";
					for (auto i : fields)
					{
						std::cout << "Field: " << i.__NAME 
								  << " | Is pack? : " << i.__IS_OBJECT
								  << " | Pack name: " << i.__PACK.__NAME << "\n";
					}
					std::cout << "\n\n";
#endif
					__pack_table.back().__METHODS = methods;
					break;
				}

				if (token_it->__VALUE == "fields")
				{
#ifdef __DEBUG					
					std::cout << "FIELDS FOUND << " << token_it->__VALUE << "\n";
#endif

					++token_it;

#ifdef __DEBUG
					std::cout << "FIELDS << ";
#endif
					while ((++token_it)->__VALUE != ";")
					{
						if (token_it->__TERMINAL == "VAR")
						{
							fields.push_back({token_it->__VALUE, "0", 0});
							__var_table.push_back(fields.back());
							++fields_cnt;
#ifdef __DEBUG
							std::cout << token_it->__VALUE << ", ";
#endif
						}
					}

					__pack_table.back().__FIELDS_CNT = fields_cnt;
					__pack_table.back().__FIELDS = fields;					
#ifdef __DEBUG					
					for (const auto &i : __pack_table.back().__FIELDS)
						std::cout << "\nNow, there is '" << i.__NAME << "' field in '" << __pack_table.back().__NAME << "' pack";
					std::cout << "\n";
#endif
				}
				else if (token_it->__VALUE == "initial")
				{
					init_fnc.__NAME = "initial";
					uint64_t args_cnt = 0;
					
					while (__var_table.size() != fields_cnt)
					{
						__var_table.pop_back();
					}

#ifdef __DEBUG
					std::cout << "INITIAL FOUND << " << token_it->__VALUE << "\n";
#endif			
					while ((++token_it)->__VALUE != "{")
					{
						if (token_it->__TERMINAL == "VAR")
						{
							init_fnc.__ARGS.push_back({token_it->__VALUE, "0", 0});
							__var_table.push_back({token_it->__VALUE, "0", 0});							
							++args_cnt;
						}
					}
#ifdef __DEBUG					
					std::cout << "ARGS COUNT << " << args_cnt << "\n";
#endif 
					init_fnc.__ARGS_CNT = args_cnt;
					init_fnc.__RPN = this->get_rpn(pack_name);

					__pack_table.back().__INITIAL = init_fnc;
				}
				else if (token_it->__VALUE == "fun")
				{
					std::string func_name = (++token_it)->__VALUE;
					uint64_t args_cnt = 0;

					while (__var_table.size() != fields_cnt)
					{
						__var_table.pop_back();
					}

#ifdef __DEBUG
					std::cout << "METHOD FOUND << " << func_name << "\n";
					std::cout << "ARGS << ";				
#endif			
					std::vector<__var> method_args;
					while ((++token_it)->__VALUE != "{")
					{
						if (token_it->__TERMINAL == "VAR")
						{
							method_args.push_back({token_it->__VALUE, "0", 0});
							__var_table.push_back({token_it->__VALUE, "0", 0});							
							++args_cnt;
#ifdef __DEBUG
							std::cout << token_it->__VALUE << ", ";
#endif
						}
					}
#ifdef __DEBUG					
					std::cout << "\n";
#endif
					methods.push_back({func_name, method_args, args_cnt, this->get_rpn(pack_name)});
				}
			}
		}
		if (token_it->__VALUE == "fun")
		{
			std::string func_name = (++token_it)->__VALUE;
			uint64_t args_cnt = 0;

			__var_table.clear();

#ifdef __DEBUG
			std::cout << "FUNC FOUND << " << func_name << "\n";
#endif
			std::vector<__var> args;
			while ((++token_it)->__VALUE != "{")
			{
				if (token_it->__TERMINAL == "VAR")
				{
					args.push_back({token_it->__VALUE, "0", 0});
					__var_table.push_back({token_it->__VALUE, "0", 0});
					++args_cnt;
				}
			}
			__function_table.push_back({func_name, args, args_cnt, this->get_rpn()});
		}
	}

	f_table = __function_table;
	p_table = __pack_table;
	ent_f_name = entry_func_name;
	ex_f_name = exit_func_name;

#ifdef __DEBUG
	std::cout << "ALL PACKS:\n";
	for (auto pack : __pack_table)
	{
		std::cout << "\n__________________________________\nNAME: " << pack.__NAME << "\n";
		std::cout << "\nFIELDS: ";
		for (auto field : pack.__FIELDS)
			std::cout << field.__NAME << ", ";
		std::cout << "\nFIELDS COUNT: " << pack.__FIELDS_CNT << "\n";
		std::cout << "\nINITIAL NAME: " << pack.__INITIAL.__NAME << "\n";
		std::cout << "INITIAL ARGS: ";
		for (auto arg : pack.__INITIAL.__ARGS)
			std::cout << arg.__NAME << ", ";
		std::cout << "\nARGS CNT: " << pack.__INITIAL.__ARGS_CNT << "\n";
		std::cout << "\nMETHODS: ";
		for (auto method : pack.__METHODS)
		{
			std::cout << "\nMETHOD NAME: " << method.__NAME << "\n";
			std::cout << "METHOD ARGS: ";
			for (auto arg : method.__ARGS)
				std::cout << arg.__NAME << ", ";
			std::cout << "\nARGS CNT: " << method.__ARGS_CNT << "\n";			
		} 
	}
	std::cout << "__________________________________\n";
#endif

#ifdef __OUT_TERMINALS
	std::ofstream out;
	out.open("tokens_2.txt");

	if ( !out.is_open() ) { std::cout << "Error writing trues terminals...\n"; return 1; }

	for (auto &i : this->__trues_terminals)
	{
		out << i << std::endl;
	}

	out.close();
#endif

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
		|| ( push_iterator(__cur__iter) && pack_expr() && ___SEMICOLON() )
		|| ( push_iterator(__cur__iter) && cycle_expr() )
		|| ( push_iterator(__cur__iter) && if_expr() )
		|| ( push_iterator(__cur__iter) && frame_expr() )
		|| ( push_iterator(__cur__iter) && embedded_func_expr() && ___SEMICOLON() )
		|| ( push_iterator(__cur__iter) && func_expr() )
		|| ( push_iterator(__cur__iter) && ___COMMENT() )
		|| ( 				 ___BREAK()	&& ___SEMICOLON() )
		|| ( ___RETURN() && (return_expr() || 1) && ___SEMICOLON() )
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

	if ( ( l_value() && (___ASSIGN_OP() || ___ARITHM_ASSIGN_OP()) && r_value_expr() ) )
	{
		return true;
	}

	push_iterator(__cur__iter);

	if ( ( l_value() && (___ASSIGN_OP() || ___ARITHM_ASSIGN_OP()) && str_func_expr() ) )
	{
		return true;
	}

	push_iterator(__cur__iter);	

	if ( l_value() && ___ASSIGN_OP() && array_expr() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	if ( l_value() && ___ASSIGN_OP() && ___LEFT_SQUARE_BRACKETS() && ___DOT() && (embedded_structure() || pack_structure()) )
	{
		if ( ___LEFT_BRACKET() )
		{
			if ( r_value_expr() )
			{
				__cur__iter = get_iterator();
				while ( ___COMMA() && r_value_expr() ) { __cur__iter = get_iterator(); }
				push_iterator(__cur__iter);
			}

			if ( ___RIGHT_BRACKET() && ___RIGHT_SQUARE_BRACKETS() )
			{
				return true;
			}
		}
	}

	push_iterator(__cur__iter);

	if ( pack_field_call() )
	{
		if ( ___ASSIGN_OP() || ___ARITHM_ASSIGN_OP() )
		{
			if ( r_value_expr() )
			{
				return true;
			}
		}
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::linked_list_control_expr()
{
	this->__current_non_terminal = "linked_list_control_expr";

	auto __cur__iter = get_iterator();

	if (___VAR() && ___DOT() && 
		(
		     linked_list_add_expr()
		  || (push_iterator(this->__current_token_iterator) && linked_list_get_expr())
		  || (push_iterator(this->__current_token_iterator) && linked_list_remove_expr())
		  || (push_iterator(this->__current_token_iterator) && linked_list_insert_expr())
		)
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::linked_list_add_expr()
{
	this->__current_non_terminal = "linked_list_add_expr";

	auto __cur__iter = get_iterator();

	if ( ___ADD() && ___LEFT_BRACKET() && r_value_expr() && ___RIGHT_BRACKET() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::linked_list_get_expr()
{
	this->__current_non_terminal = "linked_list_get_expr";

	auto __cur__iter = get_iterator();

	if ( ___GET() && ___LEFT_BRACKET() && ___INTEGER() && ___RIGHT_BRACKET() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::linked_list_remove_expr()
{
	this->__current_non_terminal = "linked_list_remove_expr";

	auto __cur__iter = get_iterator();

	if ( ___REMOVE() && ___LEFT_BRACKET() && ___INTEGER() && ___RIGHT_BRACKET() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::linked_list_insert_expr()
{
	this->__current_non_terminal = "linked_list_insert_expr";

	auto __cur__iter = get_iterator();

	if ( ___INSERT() && ___LEFT_BRACKET() && r_value_expr() && ___COMMA() && ___INTEGER() && ___RIGHT_BRACKET() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::hash_set_control_expr()
{
	this->__current_non_terminal = "hash_set_control_expr";

	auto __cur__iter = get_iterator();

	if ( ___VAR() && ___DOT() && ( (___ADD() || ___CONTAINS() || ___REMOVE()) && ___LEFT_BRACKET() && r_value_expr() && ___RIGHT_BRACKET()) )
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
		|| ( push_iterator(__cur__iter) && for_cycle_expr() ) 
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

	if (   print_func_expr() 
		|| ( push_iterator(__cur__iter) && input_func_expr() ) 
		|| ( push_iterator(__cur__iter) && sleep_func_expr() ) 
		|| ( push_iterator(__cur__iter) && str_func_expr()   ) 
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::sleep_func_expr()
{
	this->__current_non_terminal = "sleep_func_expr";

	auto __cur__iter = get_iterator();

	if ( ___SLEEP() )
	{
		this->__current_non_terminal = "sleep_func_expr_(BREAK)";
		if ( ___LEFT_BRACKET() && (___INTEGER() || ___VAR()) && ___RIGHT_BRACKET() )
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

	if ( ___VAR() || ( ___LEFT_BRACKET() && l_value() && ___RIGHT_BRACKET() ) )
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
	   || ( push_iterator(__cur__iter) && pack_call() ) 
	   || ( push_iterator(__cur__iter) && embedded_structure_call() )	   
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

	if ( ___PRINT() )
	{
		if (___LEFT_BRACKET() && r_value())
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

	if ( ___INPUT() )
	{
		if ( ___LEFT_BRACKET() && l_value() && ___COMMA() )
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

bool Parser::str_func_expr()
{
	this->__current_non_terminal = "str_func_expr";

	auto __cur__iter = get_iterator();

	if ( ___STR() )
	{
		if (___LEFT_BRACKET() && r_value() && ___RIGHT_BRACKET() )
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

	if (     func_call() || ( push_iterator(__cur__iter) && pack_call() )
		  || ( push_iterator(__cur__iter) && value_expr() )  
		  || ( push_iterator(__cur__iter) && ___VAR() ) 
		  || ___FLOAT() || ___INTEGER() || ___BOOLEAN()
		  ||   ___STRING() || ___CHAR() ||
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
	this->__current_non_terminal = "var_arithm_expr";

	auto __cur__iter = get_iterator();

	if ( var_expr() || (push_iterator(__cur__iter) && arithm_expr()) )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::pack_expr()
{
	this->__current_non_terminal = "pack_expr";

	auto __cur__iter = get_iterator();

	if ( ___PACK() && ___VAR() && pack_frame_expr() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::pack_frame_expr()
{
	this->__current_non_terminal = "pack_frame_expr";

	auto __cur__iter = get_iterator();

	if ( ___LEFT_BRACE() )
	{
		if ( ___FIELDS() && ___COLON() && ___VAR() )
		{
			__cur__iter = get_iterator();			
			while ( ___COMMA() && ___VAR() ) { __cur__iter = get_iterator(); }
			push_iterator(__cur__iter);

			___SEMICOLON();
		}

		if ( ___INITIAL() && ___LEFT_BRACKET() )
		{
			if ( ___VAR() )
			{
				__cur__iter = get_iterator();			
				while ( ___COMMA() && ___VAR() ) { __cur__iter = get_iterator(); }
				push_iterator(__cur__iter);
			}

			if ( ___RIGHT_BRACKET() )
			{
				frame_expr();
			}
		}

		if ( func_expr() && ___RIGHT_BRACE() )
		{
			return true;
		}
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::pack_structure()
{
	this->__current_non_terminal = "pack_structure";

	auto __cur__iter = get_iterator();

	if ( ___VAR() )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::embedded_structure_call()
{
	this->__current_non_terminal = "embedded_structure_call";

	auto __cur__iter = get_iterator();

	if (   linked_list_control_expr()
	    || ( push_iterator(this->__current_token_iterator) && hash_set_control_expr() )
	   )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;	
}

bool Parser::pack_call()
{
	this->__current_non_terminal = "pack_call";

	auto __cur__iter = get_iterator();

	if ( pack_method_call() || ( push_iterator(__cur__iter) && pack_field_call() ) )
	{
		return true;
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::pack_method_call()
{
	this->__current_non_terminal = "pack_field_call_first";

	auto __cur__iter = get_iterator();

	if ( ___VAR() && (this->__current_non_terminal = "pack_field_call_(BREAK)").size() && ___DOT() )
	{
		this->__current_non_terminal = "pack_field_call";
		if ( ___VAR() )
		{
			__cur__iter = get_iterator();			
			while (   (this->__current_non_terminal = "pack_field_call_(BREAK)").size() && ___DOT() 
				   && (this->__current_non_terminal = "pack_field_call").size()         && ___VAR() ) 
			{ 
				__cur__iter = get_iterator(); 
			}

			__trues_information[__trues_information.size() - 1].__NON_TERMINAL = "pack_method_call";			

			push_iterator(__cur__iter);

			if ( ___LEFT_BRACKET() )
			{
				param_func_call_expr();

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

bool Parser::pack_field_call()
{
	this->__current_non_terminal = "pack_field_call_first";

	auto __cur__iter = get_iterator();

	if (  ___VAR() )
	{
		this->__current_non_terminal = "pack_field_call_(BREAK)";		
		if ( ___DOT() )
		{
			this->__current_non_terminal = "pack_field_call";
			if ( ___VAR() )
			{
				__cur__iter = get_iterator();			
				while (    (this->__current_non_terminal = "pack_field_call_(BREAK)").size() 
					    && ___DOT() 
					    && (this->__current_non_terminal = "pack_field_call").size()
					    && ___VAR() ) 
					{ __cur__iter = get_iterator(); }
				
				push_iterator(__cur__iter);

				return true;				
			}
		}
	}

	push_iterator(__cur__iter);

	return false;
}

bool Parser::array_expr()
{
	this->__current_non_terminal = "array_expr";

	auto __cur__iter = get_iterator();

	if ( ___LEFT_SQUARE_BRACKETS() )
	{
		r_value_expr();

		__cur__iter = get_iterator();			
		while ( ___COMMA() && r_value_expr() ) { __cur__iter = get_iterator(); }
		push_iterator(__cur__iter);

		if ( ___RIGHT_SQUARE_BRACKETS() )
		{
			return true;
		}
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

bool Parser::___SLEEP()
{
	return check_terminal(SLEEP);
}

bool Parser::___COMMA()
{
	return check_terminal(COMMA);
}

bool Parser::___INPUT()
{
	return check_terminal(INPUT);
}

bool Parser::___STR()
{
	return check_terminal(STR);
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

bool Parser::___DOT()
{
	return check_terminal(DOT);
}

bool Parser::___COLON()
{
	return check_terminal(COLON);
}

bool Parser::___PACK()
{
	return check_terminal(PACK);
}

bool Parser::___FIELDS()
{
	return check_terminal(FIELDS);
}

bool Parser::___INITIAL()
{
	return check_terminal(INITIAL);
}


// Embedded structure's methods

bool Parser::___ADD()
{
	return check_terminal("add");
}

bool Parser::___GET()
{
	return check_terminal("get");
}

bool Parser::___REMOVE()
{
	return check_terminal("remove");
}

bool Parser::___INSERT()
{
	return check_terminal("insert");
}

bool Parser::___CONTAINS()
{
	return check_terminal("contains");
}


// Check terminal correct

bool Parser::check_terminal(const std::string &terminal)
{
	bool __check = false;

	if ( terminal == "add" || terminal == "get" || terminal == "remove" || terminal == "insert" || terminal == "contains" )
		__check = ((*(this->__current_token_iterator))->get_value() == terminal);
	else
		__check = ((*(this->__current_token_iterator))->get_type() == terminal);

	if ( __check )
	{
#ifdef __DEBUG
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
#endif

		std::string _to_save =         
							   "\""
						     + terminal 
						     + "\""
						     + " is now "
						     + "\"" 
						     + (__check ? "true" : "false") 
						     + "\""
						     + " for "
						     + "\"" 
						     + ((*(this->__current_token_iterator))->get_value())
						     + "\""
					   	     + "\t\t\tin line "
						     + std::to_string(((*(this->__current_token_iterator))->get_line()))
					   	     + " on position "
						     + std::to_string(((*(this->__current_token_iterator))->get_pos()))
					   	     + " ---> " 
					   	     ;
		
		
		uint64_t _cur_line = (*(this->__current_token_iterator))->get_line();
		uint64_t _cur_pos = (*(this->__current_token_iterator))->get_pos();

		if ( this->__trues.size() == 0 )
		{
			this->__trues.push_back( {_to_save, __current_non_terminal} );
			this->__last_line = (*(this->__current_token_iterator))->get_line();
			this->__last_pos = (*(this->__current_token_iterator))->get_pos();

			this->__trues_terminals.push_back(((*(this->__current_token_iterator))->get_value()));

			this->__trues_information.push_back({terminal, ((*(this->__current_token_iterator))->get_value()),
												 _cur_line, _cur_pos, __current_non_terminal});

#ifdef __DEBUG			
			std::cout << "SIZE = 0\n";
#endif
		}
		else if ( _cur_line <= this->__last_line && _cur_pos < this->__last_pos )
		{
			while ( this->__trues.back().first != _to_save )
			{
				this->__trues.pop_back();
				this->__trues_terminals.pop_back();
				this->__trues_information.pop_back();		
			}

			this->__last_line = (*(this->__current_token_iterator))->get_line();
			this->__last_pos = (*(this->__current_token_iterator))->get_pos();

#ifdef __DEBUG
			std::cout << "POP_BACKED\n";
#endif
		}
		else if ( _to_save == this->__trues.back().first )
		{
			this->__trues[this->__trues.size() - 1] = {_to_save, __current_non_terminal};
			this->__last_line = (*(this->__current_token_iterator))->get_line();
			this->__last_pos = (*(this->__current_token_iterator))->get_pos();			

			this->__trues_terminals[this->__trues_terminals.size() - 1] = (((*(this->__current_token_iterator))->get_value()));

			this->__trues_information[this->__trues_information.size() - 1] = 
											{terminal, ((*(this->__current_token_iterator))->get_value()),
											 _cur_line, _cur_pos, __current_non_terminal};

#ifdef __DEBUG
			std::cout << "TO_SAVE = BACK\n";
#endif
		}
		else
		{
			this->__trues.push_back( {_to_save, __current_non_terminal} );
			this->__last_line = (*(this->__current_token_iterator))->get_line();
			this->__last_pos = (*(this->__current_token_iterator))->get_pos();

			this->__trues_terminals.push_back(((*(this->__current_token_iterator))->get_value()));

			this->__trues_information.push_back({terminal, ((*(this->__current_token_iterator))->get_value()),
												 _cur_line, _cur_pos, __current_non_terminal});

#ifdef __DEBUG			
			std::cout << "ELSE\n";
#endif
		}

		if ( this->__current_token_iterator != this->__tokens.end() )
		{
			this->__last_token = *(this->__current_token_iterator);
		}

		++__current_token_iterator;

		if ( this->__current_token_iterator != this->__tokens.end() )
		{
			this->__last_after_token = *(this->__current_token_iterator);
		}
	}

	return __check;
}
