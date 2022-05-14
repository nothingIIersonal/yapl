#pragma once


#include "var.h"
#include "function.h"


typedef struct __pack
{
	std::string __NAME;
	std::vector<__var> __FIELDS;
	uint64_t __FIELDS_CNT;
	__function __INITIAL;
	std::vector<__function> __METHODS;
} __pack;