#pragma once


#include "var.h"
#include "rpn_node.h"


typedef struct __function
{
	std::string __NAME;
	std::vector<__var> __ARGS;
	uint64_t __ARGS_CNT;
	std::vector<__rpn_node> __RPN;	
} __function;