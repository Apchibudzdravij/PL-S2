#pragma once
#include "stdafx.h"
#include <fstream>
namespace FST
{
	struct RELATION
	{
		char symbol;
		short nnode;
		RELATION(char c = 0x00, short ns = NULL)
		{
			symbol = c;
			nnode = ns;
		}
	};

	struct NODE
	{
		short n_relation;
		RELATION* relations;
		NODE()
		{
			n_relation = 0;
			relations = NULL;
		}
		NODE(short n, RELATION rel, ...)
		{
			n_relation = n;
			relations = new RELATION[n];
			RELATION* prel = &rel;
			for (unsigned short index = 0; index < n; ++index)
				relations[index] = prel[index];
		}
	};

	struct FST
	{
		char* string;
		short position;
		short nstates;
		NODE* nodes;
		short* rstates;
		FST(char* s, short ns, NODE  n, ...)
		{
			string = s;
			nstates = ns;
			nodes = new NODE[ns];
			NODE* pn = &n;
			for (unsigned short index = 0; index < ns; ++index)
				nodes[index] = pn[index];
			rstates = new short[nstates];
			memset(rstates, 0xff, sizeof(short)*nstates);
			rstates[0] = 0;
			position = -1;
		}
		FST(){}
	};

	bool execute(FST& fst);
	void called(char mas[]);
}