#pragma once


#include <any>

#include "pack.h"

#include "../doubly_linked_list/doubly_linked_list.h"


typedef struct __var
{
	std::string __NAME;
	std::string __R_VALUE;
	uint64_t __SCOPE;

	bool __IS_OBJECT = false;
	__pack __PACK = {};
	std::any __EMB_STRUCT;
} __var;