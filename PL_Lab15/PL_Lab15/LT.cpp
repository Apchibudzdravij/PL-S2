#include "iostream"
#include "Error.h"
#include "LT.h"

namespace LT
{
	LexTable Create(int size)
	{
		LexTable lextable;
		lextable.maxsize = size;
		lextable.size = 0;
		//Entry* entry = new Entry[lextable.size];
		//lextable.table = entry;
		lextable.table = new Entry[size];//если это не заработает, вернуть прежние 2 строки!
		return lextable;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		++lextable.size;
		if (lextable.size > lextable.maxsize)
			ERROR_THROW(116);
		lextable.table[lextable.size - 1] = entry;
		return;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		if ((n > lextable.size) || (n < 0))
			ERROR_THROW(113);
		return lextable.table[n - 1];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
		return;
	}

	void ShowLT(LexTable& lextable)
	{
		int line = 0;
		for (int i = 0; i < lextable.size; ++i)
		{
			if (lextable.table[i].sn > line)
			{
				++line;
				std::cout << "\n" << lextable.table[i].lexema[0];
			}
			else
				std::cout << lextable.table[i].lexema[0];
		}
	}
}