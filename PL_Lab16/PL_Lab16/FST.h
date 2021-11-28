#pragma once
#include "stdafx.h"
#include <fstream>

#define FST_INTEGER 8,\
		NODE(1, RELATION('i', 1)),\
		NODE(1, RELATION('n', 2)),\
		NODE(1, RELATION('t', 3)),\
		NODE(1, RELATION('e', 4)),\
		NODE(1, RELATION('g', 5)),\
		NODE(1, RELATION('e', 6)),\
		NODE(1, RELATION('r', 7)),\
		NODE()

#define FST_STRING 7,\
		NODE(1, RELATION('s', 1)),\
		NODE(1, RELATION('t', 2)),\
		NODE(1, RELATION('r', 3)),\
		NODE(1, RELATION('i', 4)),\
		NODE(1, RELATION('n', 5)),\
		NODE(1, RELATION('g', 6)),\
		NODE()

#define FST_FUNCTION 9,\
		NODE(1, RELATION('f', 1)),\
		NODE(1, RELATION('u', 2)),\
		NODE(1, RELATION('n', 3)),\
		NODE(1, RELATION('c', 4)),\
		NODE(1, RELATION('t', 5)),\
		NODE(1, RELATION('i', 6)),\
		NODE(1, RELATION('o', 7)),\
		NODE(1, RELATION('n', 8)),\
		NODE()

#define FST_DECLARE 8,\
		NODE(1, RELATION('d', 1)),\
		NODE(1, RELATION('e', 2)),\
		NODE(1, RELATION('c', 3)),\
		NODE(1, RELATION('l', 4)),\
		NODE(1, RELATION('a', 5)),\
		NODE(1, RELATION('r', 6)),\
		NODE(1, RELATION('e', 7)),\
		NODE()

#define FST_RETURN 7,\
		NODE(1, RELATION('r', 1)),\
		NODE(1, RELATION('e', 2)),\
		NODE(1, RELATION('t', 3)),\
		NODE(1, RELATION('u', 4)),\
		NODE(1, RELATION('r', 5)),\
		NODE(1, RELATION('n', 6)),\
		NODE()

#define FST_PRINT 6,\
		NODE(1, RELATION('p', 1)),\
		NODE(1, RELATION('r', 2)),\
		NODE(1, RELATION('i', 3)),\
		NODE(1, RELATION('n', 4)),\
		NODE(1, RELATION('t', 5)),\
		NODE()

#define FST_MAIN 5,\
		NODE(1, RELATION('m', 1)),\
		NODE(1, RELATION('a', 2)),\
		NODE(1, RELATION('i', 3)),\
		NODE(1, RELATION('n', 4)),\
		NODE()

#define FST_SEMICOLON 2,\
		NODE(1, RELATION(';', 1)),\
		NODE()

#define FST_COMMA 2,\
		NODE(1, RELATION(',', 1)),\
		NODE()

#define FST_LEFTBRACE 2,\
		NODE(1, RELATION('{', 1)),\
		NODE()

#define FST_BRACELET 2,\
		NODE(1, RELATION('}', 1)),\
		NODE()

#define FST_LEFTHESIS 2,\
		NODE(1, RELATION('(', 1)),\
		NODE()

#define FST_RIGHTHESIS 2,\
		NODE(1, RELATION(')', 1)),\
		NODE()

#define FST_PLUS 2,\
		NODE(1, RELATION('+', 1)),\
		NODE()

#define FST_MINUS 2,\
		NODE(1, RELATION('-', 1)),\
		NODE()

#define FST_STAR 2,\
		NODE(1, RELATION('*', 1)),\
		NODE()

#define FST_DIRSLASH 2,\
		NODE(1, RELATION('/', 1)),\
		NODE()

#define FST_IS 2,\
		NODE(1, RELATION('=', 1)),\
		NODE()


namespace FST
{
	struct RELATION {
		char symbol;
		short nnode;
		RELATION(char c = 0x00, short ns = NULL);
	};

	struct NODE
	{
		short n_relation;
		RELATION* relations;
		NODE();
		NODE(short n, RELATION rel, ...);
	};




	struct FST {
		char* string;
		short position;
		short nstates;
		NODE* nodes;
		short* rstates;
		FST(char* s, short ns, NODE n, ...);
	};


	bool step(FST& fst, short*& rstates);
	bool execute(FST& fst);
	char called(char mas[]);
}