#pragma once


typedef struct __rpn_node
{
	uint64_t __POSITION;
	std::string __VALUE;
	std::string __TERMINAL;
	std::string __NON_TERMINAL;
	uint64_t __LINE;
	uint64_t __POS;
} __rpn_node;


__rpn_node create_rpn_node(std::string value)
{
	return {0, value, "", "", 0, 0};
}