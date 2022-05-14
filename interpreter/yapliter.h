#pragma once


#include <iostream>
#include <vector>
#include <stack>
#include <regex>
#include <cinttypes>
#include <cmath>
#include <chrono>
#include <thread>

#include "../symbols/symbols.h"

#include "../doubly_linked_list/doubly_linked_list.h"


std::vector<__rpn_node> __global_rpn;
std::vector<__rpn_node> ::iterator __g_rpn_it;

std::vector<std::vector<__var>> __var_table;
std::vector<__var> __var_table_backup;

std::vector<__function> __function_table;
std::vector<__pack> __pack_table;

std::stack<__rpn_node> ex_stack;

std::vector<__var> ::iterator _PACK_CHAIN;
std::string _PACK_CHAIN_STR = "*";

const std::regex num_rgx("[-]?(([0-9]+\\.[0-9]*)|(\\.[0-9]+)|([0-9]*)|([0-9]*))");
const std::regex int_rgx("[-]?([0-9]*)|([0-9]*)");


void execute_rpn(const std::vector<__rpn_node> &rpn, std::vector<__function> &function_table, std::vector<__pack> &pack_table, std::string &function_name, std::vector<__var> args);


bool is_num(std::string &s)
{
	if (!std::regex_match(s, num_rgx))
	{
		return false;
	}

	return true;
}

bool is_int(std::string &s)
{
	if (!std::regex_match(s, int_rgx))
	{
		return false;
	}

	return true;
}

bool is_str(std::string &s)
{
	return s[0] == '"';
}

void trim_str(std::string &s)
{
	s.erase(0, 1);
	s.erase(s.length() - 1, 1);
}

void make_str(std::string &s)
{
	s.insert(0, 1, '"');
	s.insert(s.end(), 1, '"');
}

std::string str_unshield(std::string str)
{
    std::string new_line_shield = "\\n";

    std::string new_line_unshield ="\n";
    
    std::string::size_type index;
    while((index=str.find(new_line_shield)) != std::string::npos) 
        str.replace(index, new_line_shield.size(), new_line_unshield);

    return str;
}

std::string trim_str_and_return(std::string s)
{
	trim_str(s);
	return s;
}

std::string make_str_and_return(std::string s)
{
	make_str(s);
	return s;
}

void trim_method_or_struct(std::string &s)
{
	s.erase(0, 1);
	s.erase(s.length() - 2, 2);
}

int64_t find_in_v_table(const std::string &var)
{
	int64_t pos = 0;

	for (const auto &v : __var_table.back())
	{
		if (v.__NAME == var)
			return pos;
		++pos;
	}

	return -1;
}

int64_t find_in_p_table(const std::string &pack)
{
	int64_t pos = 0;

	for (const auto &p : __pack_table)
	{
		if (p.__NAME == pack)
			return pos;
		++pos;
	}

	return -1;
}

std::vector<__var> ::iterator find_f_in_p_chain(const std::string &field)
{
	uint64_t pos_f = 0;
	__pack &p_i = _PACK_CHAIN->__PACK;

	for (const auto &f : p_i.__FIELDS)
		if (++pos_f && f.__NAME == field)
			break;

	std::vector<__var> ::iterator f_i = p_i.__FIELDS.begin() + pos_f - 1;

	return f_i;
}

void go_to(uint64_t rpn_pos)
{
	__g_rpn_it = __global_rpn.begin() + rpn_pos - 1;
}

void print_stack(std::stack<__rpn_node> stack)
{
	std::cout << "STACK > ";
	while (!stack.empty())
	{
		std::cout << stack.top().__VALUE << ", ";
		stack.pop();
	}
	std::cout << "\n";
}

void perform_func(std::string func)
{
	std::string func_name = "";
	for (auto i : func)
	{
		if (i == '(')
			break;
		func_name += i;
	}

	if (func_name != "print" && func_name != "input" && func_name != "sleep" && func_name != "str")
	{
		for (auto i : __function_table)
		{
			if (i.__NAME == func_name)
			{
				std::vector<std::string> args_vals;
				std::string arg_name = "";
				for (uint64_t j = 0; j < i.__ARGS_CNT; j++)
				{
					arg_name = ex_stack.top().__VALUE;					
					args_vals.push_back(arg_name);
					ex_stack.pop();
				}
				std::reverse(args_vals.begin(), args_vals.end());

				std::vector<__var> args;
				for (uint64_t j = 0; j < i.__ARGS_CNT; j++)
				{	
					if (!is_num(args_vals[j]) && !is_str(args_vals[j]))
						args.push_back({i.__ARGS[j].__NAME, __var_table.back()[find_in_v_table(args_vals[j])].__R_VALUE, 0});
					else
						args.push_back({i.__ARGS[j].__NAME, args_vals[j], 0});
				}

				auto __g_rpn_save = __global_rpn;
				uint64_t __it_pos = std::distance(__global_rpn.begin(), __g_rpn_it);

				execute_rpn(i.__RPN, __function_table, __pack_table, i.__NAME, args);

				__global_rpn = __g_rpn_save;
				go_to(__it_pos + 1);

				return;
			}
		}
	}

	if (func_name == "print")
	{
		uint64_t args = std::stoi(ex_stack.top().__VALUE); ex_stack.pop();
		std::vector<std::string> out;
		std::string arg = "";

		while (args--)
		{
			arg = ex_stack.top().__VALUE; ex_stack.pop();
			
			if (!is_num(arg) && !is_str(arg))
			{
				for (auto i : __var_table.back())
				{
					if (i.__NAME == arg)
					{
						arg = i.__R_VALUE;
						break;
					}
				}				
			}
			
			if (is_str(arg))
			{
				trim_str(arg);
			}

			out.push_back(arg);
		}

		std::string print_str = "";
		std::reverse(out.begin(), out.end());
		for (const auto &i : out)
			print_str += i;

		std::cout << str_unshield(print_str) << std::endl;
	}
	else if (func_name == "input")
	{
		std::string hint = ex_stack.top().__VALUE; ex_stack.pop();
		std::string var = ex_stack.top().__VALUE; ex_stack.pop();

		trim_str(hint);
		std::cout << hint;

		std::string inp;
		std::cin >> inp;

		if (!is_num(inp))
		{
			inp.insert(0, "\"");
			inp.insert(inp.length(), "\"");
		}

		if (find_in_v_table(var) == -1)
		{
			__var_table.back().push_back({var, inp, 0});
		}
		else
		{
			for (auto i : __var_table.back())
			{
				if (i.__NAME == var)
				{
					__var_table.back()[find_in_v_table(var)] = {var, inp, 0};
					break;
				}
			}
		}
	}
	else if (func_name == "sleep")
	{
		uint64_t time = std::stoi(ex_stack.top().__VALUE); ex_stack.pop();
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
	}
	else if (func_name == "str")
	{
		std::string arg = ex_stack.top().__VALUE; ex_stack.pop();
		
		if (!is_num(arg) && !is_str(arg))
		{
			for (auto i : __var_table.back())
			{
				if (i.__NAME == arg)
				{
					arg = i.__R_VALUE;
					break;
				}
			}
		}

		bool is_float = false, dot = false;
		for (const auto &c : arg)
		{
			if (c == '.')
			{
				dot = true;
			}
			else if (dot)
			{
				if (c != '0')
				{
					is_float = true;
					break;
				}
				else
				{
					is_float = false;
				}
			}
		}

		if (!is_float)
		{
			std::cout << "IS INT\n\n";
			arg = std::to_string(std::stoi(arg));
		}
		else if (is_float)
		{
			std::cout << "IS FLOAT\n\n";
			arg = std::to_string(std::stof(arg));
		}

		ex_stack.push(create_rpn_node(make_str_and_return(arg)));
	}
}

void perform_pack(std::string pack_name)
{
	trim_method_or_struct(pack_name);

	__pack pack = *(__pack_table.begin() + find_in_p_table(pack_name));

	std::vector<std::string> args_vals;
	std::string arg_name = "";

	for (uint64_t i = 0; i < pack.__INITIAL.__ARGS_CNT; i++)
	{
		arg_name = ex_stack.top().__VALUE; ex_stack.pop();
		args_vals.push_back(arg_name);
	}
	std::reverse(args_vals.begin(), args_vals.end());

	std::vector<__var> args;
	for (const auto &f : pack.__FIELDS)
		args.push_back(f);	
	for (uint64_t i = 0; i < pack.__INITIAL.__ARGS_CNT; i++)
	{	
		if (!is_num(args_vals[i]) && !is_str(args_vals[i]))
			args.push_back({pack.__INITIAL.__ARGS[i].__NAME, __var_table.back()[find_in_v_table(args_vals[i])].__R_VALUE, 0});
		else
			args.push_back({pack.__INITIAL.__ARGS[i].__NAME, args_vals[i], 0});
	}

	std::string oprnd_1 = ex_stack.top().__VALUE; ex_stack.pop();

	if (find_in_v_table(oprnd_1) == -1)
		__var_table.back().push_back({oprnd_1, "0", 0, true, pack});
	else
		__var_table.back()[find_in_v_table(oprnd_1)] = {oprnd_1, "0", 0, true, pack};


	__pack &p = __var_table.back()[find_in_v_table(oprnd_1)].__PACK;

	auto __g_rpn_save = __global_rpn;
	uint64_t __it_pos = std::distance(__global_rpn.begin(), __g_rpn_it);

	execute_rpn(p.__INITIAL.__RPN, __function_table, __pack_table, p.__INITIAL.__NAME, args);

	for (const auto &v : __var_table_backup)
		for (auto &f : p.__FIELDS)
			if (f.__NAME == v.__NAME)
				f = v;
	
	__global_rpn = __g_rpn_save;
	go_to(__it_pos + 1);


	return;
}

void perform_emb_str(std::string &emb_str_name)
{
	trim_method_or_struct(emb_str_name);

	std::string type = ex_stack.top().__VALUE; ex_stack.pop();
	std::string var_name = ex_stack.top().__VALUE; ex_stack.pop();

	trim_str(type);

	if (emb_str_name == "linked_list")
	{
		if (type == "num")
		{
			DoublyLinkedList<float> backend_linked_list = DoublyLinkedList<float>();

			if (find_in_v_table(var_name) == -1)
				__var_table.back().push_back({var_name, "0", 0, false, {"linked_list"}, backend_linked_list});
			else
				__var_table.back()[find_in_v_table(var_name)] = {var_name, "0", 0, false, {"linked_list"}, backend_linked_list};
		}
		else if (type == "str")
		{
			DoublyLinkedList<std::string> backend_linked_list = DoublyLinkedList<std::string>();

			if (find_in_v_table(var_name) == -1)
				__var_table.back().push_back({var_name, "0", 0, false, {"linked_list"}, backend_linked_list});
			else
				__var_table.back()[find_in_v_table(var_name)] = {var_name, "0", 0, false, {"linked_list"}, backend_linked_list};	
		}		

	}
	else if (emb_str_name == "hash_set")
	{
	// 		DoublyLinkedList<std::string> backend_linked_list = DoublyLinkedList<std::string>();

	// 		if (find_in_v_table(var_name) == -1)
	// 			__var_table.back().push_back({var_name, "0", 0, false, {"hash_set"}, backend_linked_list});
	// 		else
	// 			__var_table.back()[find_in_v_table(var_name)] = {var_name, "0", 0, false, {"hash_set"}, backend_linked_list};	
	}

	return;
}

void perform_method(std::string method)
{
	std::string method_name = "";
	for (auto i : method)
	{
		if (i == '!')
			continue;
		if (i == '(')
			break;
		method_name += i;
	}

	if (method_name == "insert")
	{
		std::string pos = ex_stack.top().__VALUE; ex_stack.pop();
		std::string val = ex_stack.top().__VALUE; ex_stack.pop();
		std::string var_name = ex_stack.top().__VALUE; ex_stack.pop();		

		float pos_n = 0;
		std::string pos_s = "";

		float val_n = 0;
		std::string val_s = "";


		if (!is_num(pos) && !is_str(pos))
		{
			for (auto i : __var_table.back())
			{
				if (i.__NAME == pos)
				{
					if (is_str(i.__R_VALUE))
						pos_s = i.__R_VALUE;
					else if (is_num(i.__R_VALUE))
						pos_n = std::stof(i.__R_VALUE);
					break;
				}
			}
		}
		else if (is_num(pos))
		{
			pos_n = std::stof(pos);
		}
		else if (is_str(pos))
		{
			pos_s = pos;
		}

		if (!is_num(val) && !is_str(val))
		{
			for (auto i : __var_table.back())
			{
				if (i.__NAME == val)
				{
					if (is_str(i.__R_VALUE))
						val_s = i.__R_VALUE;
					else if (is_num(i.__R_VALUE))
						val_n = std::stof(i.__R_VALUE);
					break;
				}
			}
		}
		else if (is_num(val))
		{
			val_n = std::stof(val);
		}
		else if (is_str(val))
		{
			val_s = val;
		}


		try
		{
			std::any_cast<DoublyLinkedList<std::string>&>(__var_table.back()[find_in_v_table(var_name)].__EMB_STRUCT).insert(val_s, pos_n);
		}
		catch (...) {  }

		try
		{
			std::any_cast<DoublyLinkedList<float>&>(__var_table.back()[find_in_v_table(var_name)].__EMB_STRUCT).insert(val_n, pos_n);
		}
		catch (...) { }

		++__g_rpn_it;
		return;
	}

	std::string oprnd = ex_stack.top().__VALUE; ex_stack.pop();
	std::string var_name = ex_stack.top().__VALUE; ex_stack.pop();

	if (__var_table.back()[find_in_v_table(var_name)].__IS_OBJECT == false && __var_table.back()[find_in_v_table(var_name)].__PACK.__NAME == "linked_list")
	{

		float add_value_n = 0;
		std::string add_value_s = "";

		if (!is_num(oprnd) && !is_str(oprnd))
		{
			for (auto i : __var_table.back())
			{
				if (i.__NAME == oprnd)
				{
					if (is_str(i.__R_VALUE))
						add_value_s = i.__R_VALUE;
					else if (is_num(i.__R_VALUE))
						add_value_n = std::stof(i.__R_VALUE);
					break;
				}
			}
		}
		else if (is_num(oprnd))
		{
			add_value_n = std::stof(oprnd);
		}
		else if (is_str(oprnd))
		{
			add_value_s = oprnd;
		}


		if (method_name == "add")
		{
			try
			{
				std::any_cast<DoublyLinkedList<std::string>&>(__var_table.back()[find_in_v_table(var_name)].__EMB_STRUCT).add(add_value_s);
			}
			catch (...) {  }

			try
			{
				std::any_cast<DoublyLinkedList<float>&>(__var_table.back()[find_in_v_table(var_name)].__EMB_STRUCT).add(add_value_n);
			}
			catch (...) {  }
		}
		else if (method_name == "get")
		{
			try
			{
				ex_stack.push(create_rpn_node(std::any_cast<DoublyLinkedList<std::string>&>(__var_table.back()[find_in_v_table(var_name)].__EMB_STRUCT).get((uint64_t)add_value_n)));
			}
			catch (...) {  }

			try
			{
				ex_stack.push(create_rpn_node(std::to_string(std::any_cast<DoublyLinkedList<float>&>(__var_table.back()[find_in_v_table(var_name)].__EMB_STRUCT).get((uint64_t)add_value_n))));
			}
			catch (...) {  }
		}
		else if (method_name == "remove")
		{
			try
			{
				std::any_cast<DoublyLinkedList<std::string>&>(__var_table.back()[find_in_v_table(var_name)].__EMB_STRUCT).remove((uint64_t)add_value_n);
			}
			catch (...) {  }

			try
			{
				std::any_cast<DoublyLinkedList<float>&>(__var_table.back()[find_in_v_table(var_name)].__EMB_STRUCT).remove((uint64_t)add_value_n);
			}
			catch (...) {  }
		}
	}

	++__g_rpn_it;
	return;
}

void perform_op(std::string op)
{
	if (op[op.length() - 1] == ')' && op[0] == '!')
	{
		perform_method(op);
		return;
	}
	else if (op[op.length() - 1] == ')' && op[0] == ':')
	{
		perform_emb_str(op);

		++__g_rpn_it; 		// for avoid '=', because it's provided inside function

		return;
	}
	else if (op[op.length() - 1] == ')' && op[0] != '.')
	{
		perform_func(op);
		return;
	}
	else if (op[op.length() - 1] == ')' && op[0] == '.')
	{
		perform_pack(op);

		++__g_rpn_it; 		// for avoid '=', because it's provided inside function
		
		return;
	}

	std::string oprnd_2 = "";
	std::string oprnd_1 = "";

	float oprnd_1_n = 0;
	float oprnd_2_n = 0;

	std::string oprnd_1_s = "";
	std::string oprnd_2_s = "";

	if (op == ".")
	{
		oprnd_2 = ex_stack.top().__VALUE; ex_stack.pop();
		oprnd_1 = ex_stack.top().__VALUE; ex_stack.pop();
	}
	else if (  op == "-_PREF"
			|| op == "+_PREF"
			|| op == "++"
			|| op == "--"
			|| op == "!"
			|| op == "~"
			|| op == "return"
			|| op == "~~")
	{
		oprnd_1 = ex_stack.top().__VALUE;
	
		// if (op != "--" && op != "++")
			ex_stack.pop();

		if (!is_num(oprnd_1) && !is_str(oprnd_1))
		{
			for (auto i : __var_table.back())
			{
				if (i.__NAME == oprnd_1)
				{
					if (is_str(i.__R_VALUE))
						oprnd_1_s = i.__R_VALUE;
					else if (is_num(i.__R_VALUE))
						oprnd_1_n = std::stof(i.__R_VALUE);
					break;
				}
			}
		}
		else if (is_num(oprnd_1))
		{
			oprnd_1_n = std::stof(oprnd_1);
		}
		else if (is_str(oprnd_1))
		{
			oprnd_1_s = oprnd_1;
		}
	}
	else if (   op == "!F"
			 || op == "!JMP")
	{
		if (op == "!F")
		{
			oprnd_2 = ex_stack.top().__VALUE; ex_stack.pop();
			oprnd_1 = ex_stack.top().__VALUE; ex_stack.pop();

			if (!is_num(oprnd_1) && !is_str(oprnd_1))
			{
				for (auto i : __var_table.back())
				{
					if (i.__NAME == oprnd_1)
					{
						if (is_str(i.__R_VALUE))
							oprnd_1_s = i.__R_VALUE;
						else if (is_num(i.__R_VALUE))
							oprnd_1_n = std::stof(i.__R_VALUE);
						break;
					}
				}
			}
			else if (is_num(oprnd_1))
			{
				oprnd_1_n = std::stof(oprnd_1);
			}
			else if (is_str(oprnd_1))
			{
				oprnd_1_s = oprnd_1;
			}

			if (!oprnd_1_n && oprnd_1_s == "")
			{
				go_to(std::stoi(oprnd_2));
			}
		}
		else
		{
			oprnd_1 = ex_stack.top().__VALUE; ex_stack.pop();
			go_to(std::stoi(oprnd_1));
		}
	}
	else
	{
		oprnd_2 = ex_stack.top().__VALUE; ex_stack.pop();
		oprnd_1 = ex_stack.top().__VALUE; ex_stack.pop();

		if (op != "=")
		{
			if (!is_num(oprnd_1) && !is_str(oprnd_1))
			{
				for (auto i : __var_table.back())
				{
					if (i.__NAME == oprnd_1)
					{
						if (is_str(i.__R_VALUE))
							oprnd_1_s = i.__R_VALUE;
						else if (is_num(i.__R_VALUE))
							oprnd_1_n = std::stof(i.__R_VALUE);
						break;
					}
				}
			}
			else if (is_num(oprnd_1))
			{
				oprnd_1_n = std::stof(oprnd_1);
			}
			else if (is_str(oprnd_1))
			{
				oprnd_1_s = oprnd_1;
			}
		}

		if (!is_num(oprnd_2) && !is_str(oprnd_2))
		{
			for (auto i : __var_table.back())
			{
				if (i.__NAME == oprnd_2)
				{
					if (is_str(i.__R_VALUE))
						oprnd_2_s = i.__R_VALUE;
					else if (is_num(i.__R_VALUE))
						oprnd_2_n = std::stof(i.__R_VALUE);
					break;
				}
			}
		}
		else if (is_num(oprnd_2))
		{
			oprnd_2_n = std::stof(oprnd_2);
		}
		else if (is_str(oprnd_2))
		{
			oprnd_2_s = oprnd_2;
		}
	}


	if (op == "=")
		if (find_in_v_table(oprnd_1) == -1)
			__var_table.back().push_back({oprnd_1, is_str(oprnd_2) ? oprnd_2 : std::to_string(oprnd_2_n), 0});
		else
			__var_table.back()[find_in_v_table(oprnd_1)] = {oprnd_1, is_str(oprnd_2) ? oprnd_2 : std::to_string(oprnd_2_n), 0};
	else if (op == "+=")
		__var_table.back()[find_in_v_table(oprnd_1)] = 
			{oprnd_1, 
				(is_str(oprnd_1_s) && is_str(oprnd_2_s)) ? 
				make_str_and_return(trim_str_and_return(oprnd_1_s) + trim_str_and_return(oprnd_2_s))
				: 
				std::to_string(oprnd_1_n + oprnd_2_n)
			, 0};
	else if (op == "-=")
		__var_table.back()[find_in_v_table(oprnd_1)] = {oprnd_1, std::to_string(oprnd_1_n - oprnd_2_n), 0};
	else if (op == "*=")
		__var_table.back()[find_in_v_table(oprnd_1)] = {oprnd_1, std::to_string(oprnd_1_n * oprnd_2_n), 0};
	else if (op == "/=")
		__var_table.back()[find_in_v_table(oprnd_1)] = {oprnd_1, std::to_string(oprnd_1_n / oprnd_2_n), 0};
	else if (op == ">>=")
		__var_table.back()[find_in_v_table(oprnd_1)] = {oprnd_1, std::to_string((int64_t)oprnd_1_n >> (int64_t)oprnd_2_n), 0};
	else if (op == "<<=")
		__var_table.back()[find_in_v_table(oprnd_1)] = {oprnd_1, std::to_string((int64_t)oprnd_1_n << (int64_t)oprnd_2_n), 0};			
	else if (op == "+_PREF")
		ex_stack.push(create_rpn_node(std::to_string(oprnd_1_n)));		
	else if (op == "-_PREF")
		ex_stack.push(create_rpn_node(std::to_string(-oprnd_1_n)));
	else if (op == "!")
		ex_stack.push(create_rpn_node(std::to_string(oprnd_1_n ? 0 : 1)));
	else if (op == "~")
		ex_stack.push(create_rpn_node(std::to_string(~(int64_t)oprnd_1_n)));
	else if (op == "&")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n & (int64_t)oprnd_2_n)));
	else if (op == "|")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n | (int64_t)oprnd_2_n)));
	else if (op == "^")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n ^ (int64_t)oprnd_2_n)));
	else if (op == ">")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n > (int64_t)oprnd_2_n)));
	else if (op == "<")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n < (int64_t)oprnd_2_n)));
	else if (op == ">=")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n >= (int64_t)oprnd_2_n)));
	else if (op == "<=")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n <= (int64_t)oprnd_2_n)));
	else if (op == "==")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n == (int64_t)oprnd_2_n)));
	else if (op == "!=")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n != (int64_t)oprnd_2_n)));
	else if (op == ">>")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n >> (int64_t)oprnd_2_n)));
	else if (op == "<<")
		ex_stack.push(create_rpn_node(std::to_string((int64_t)oprnd_1_n << (int64_t)oprnd_2_n)));	
	else if (op == "--")
		__var_table.back()[find_in_v_table(oprnd_1)] = {oprnd_1, std::to_string(oprnd_1_n - 1), 0};
	else if (op == "++")
		__var_table.back()[find_in_v_table(oprnd_1)] = {oprnd_1, std::to_string(oprnd_1_n + 1), 0};
	else if (op == "-")
		ex_stack.push(create_rpn_node(std::to_string(oprnd_1_n - oprnd_2_n)));
	else if (op == "+")
		ex_stack.push(
			create_rpn_node(
			            (is_str(oprnd_1_s) && is_str(oprnd_2_s))
						? 
							make_str_and_return(trim_str_and_return(oprnd_1_s) + trim_str_and_return(oprnd_2_s))
						: 
							std::to_string(oprnd_1_n + oprnd_2_n)
					  )
					 );
	else if (op == "*")
		ex_stack.push(create_rpn_node(std::to_string(oprnd_1_n * oprnd_2_n)));
	else if (op == "/")
		ex_stack.push(create_rpn_node(std::to_string(oprnd_1_n / oprnd_2_n)));
	else if (op == "**")
		ex_stack.push(create_rpn_node(std::to_string(pow(oprnd_1_n, oprnd_2_n))));
	else if (op == "%")
		ex_stack.push(create_rpn_node(std::to_string(fmod(oprnd_1_n, oprnd_2_n))));
	else if (op == "return")
		ex_stack.push(
			create_rpn_node(
			            is_str(oprnd_1_s)
						?
							oprnd_1_s
						:
							std::to_string(oprnd_1_n)
					  )
					 );
	else if (op == "~~")
		__var_table.back().erase(__var_table.back().begin() + find_in_v_table(oprnd_1));
	else if (op == ".")
	{
		uint64_t pos = 0;

		if (_PACK_CHAIN_STR == "*")
		{
			pos = 0;
			for (const auto &v : __var_table.back())
				if (++pos && v.__NAME == oprnd_1)
					break;

			_PACK_CHAIN = __var_table.back().begin() + pos - 1;

			_PACK_CHAIN_STR += oprnd_1;
		}

		_PACK_CHAIN_STR += "." + oprnd_2;

		_PACK_CHAIN = find_f_in_p_chain(oprnd_2);

		// if (!ex_stack.empty()) ex_stack.pop();
		ex_stack.push(create_rpn_node(_PACK_CHAIN_STR));

		if (++__g_rpn_it != __global_rpn.end())
			if (__g_rpn_it->__VALUE != ".")
			{
				if (!_PACK_CHAIN->__IS_OBJECT)
				{
					ex_stack.pop();
					ex_stack.push(create_rpn_node(_PACK_CHAIN->__R_VALUE));
				}

				_PACK_CHAIN = __var_table.back().end();
				_PACK_CHAIN_STR = "*";
			}
		--__g_rpn_it;
	}
}

void execute_rpn(const std::vector<__rpn_node> &rpn, std::vector<__function> &function_table, std::vector<__pack> &pack_table, std::string &function_name, std::vector<__var> args)
{
	__global_rpn = rpn;
	__function_table = function_table;
	__pack_table = pack_table;

	__var_table.push_back(args);

	__g_rpn_it = __global_rpn.begin();
	while (__g_rpn_it != __global_rpn.end())
	{
		auto r_node = *__g_rpn_it;
		++__g_rpn_it;

		// if (!ex_stack.empty())
		// {
		// 	print_stack(ex_stack);
		// }

		// if (__var_table.back().size())
		// {
		// 	std::cout << "VARS > ";
		// 	for (auto i : __var_table.back())
		// 	{
		// 		std::cout << i.__NAME << " (" << i.__R_VALUE << "), ";
		// 	}
		// 	std::cout << "\n\n";
		// }

		// std::cout << "IN MAIN: \n";
		// for (const auto &p : __pack_table)
		// {
		// 	std::cout << "\t" << p.__NAME << " >>:\n";
		// 	for (const auto &f : p.__FIELDS)
		// 		std::cout << "\t\tField '"
		// 				  << f.__NAME
		// 				  << "' is now '"
		// 				  << f.__R_VALUE
		// 				  << "'\n\n";
		// }

		if (r_node.__VALUE == "ERPN") continue;

		if (   r_node.__VALUE != "="
			&& r_node.__VALUE != "+="
			&& r_node.__VALUE != "-="
			&& r_node.__VALUE != "*="
			&& r_node.__VALUE != "/="
			&& r_node.__VALUE != "%="
			&& r_node.__VALUE != "|="
			&& r_node.__VALUE != "&="
			&& r_node.__VALUE != "^="
			&& r_node.__VALUE != ">>="
			&& r_node.__VALUE != "<<="
			&& r_node.__VALUE != "!"
			&& r_node.__VALUE != "~"
			&& r_node.__VALUE != "|"
			&& r_node.__VALUE != "&"
			&& r_node.__VALUE != "^"
			&& r_node.__VALUE != "=="
			&& r_node.__VALUE != ">="
			&& r_node.__VALUE != "<="
			&& r_node.__VALUE != "<"
			&& r_node.__VALUE != ">"
			&& r_node.__VALUE != "!="
			&& r_node.__VALUE != "||"
			&& r_node.__VALUE != "&&"
			&& r_node.__VALUE != ">>"
			&& r_node.__VALUE != "<<"
			&& r_node.__VALUE != "+"
			&& r_node.__VALUE != "-"
			&& r_node.__VALUE != "*"
			&& r_node.__VALUE != "/"
			&& r_node.__VALUE != "**"
			&& r_node.__VALUE != "%"
			&& r_node.__VALUE != "-_PREF"
			&& r_node.__VALUE != "+_PREF"
			&& r_node.__VALUE != "++"
			&& r_node.__VALUE != "--"
			&& r_node.__VALUE != "return"
			&& r_node.__VALUE != "!F"
			&& r_node.__VALUE != "!JMP"
			&& r_node.__VALUE != "~~"
			&& r_node.__VALUE != "."
			&& r_node.__VALUE[r_node.__VALUE.length() - 1] != ')')
		{
			ex_stack.push(r_node);
			continue;
		}

		perform_op(r_node.__VALUE);
	}

	// if (__var_table.back().size())
	// {
	// 	std::cout << "\n__________________________________________________________________________________\n"
	// 			  << "VARS OF "
	// 			  << "'\033[1;31m" << function_name << "\033[0m'"
	// 			  << ">\n"
	// 			  << "\x1b[32;1m";
	// 	for (auto i : __var_table.back())
	// 	{
	// 		std::cout << i.__NAME << " (" << i.__R_VALUE << "), ";
	// 	}
	// 	std::cout << "\x1b[0m"
	// 			  << "\n__________________________________________________________________________________\n\n";
	// }

	__var_table_backup = __var_table.back();
	__var_table.pop_back();
}