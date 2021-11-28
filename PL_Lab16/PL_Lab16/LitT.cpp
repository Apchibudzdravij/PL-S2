#include "Error.h"
#include "LitT.h" 

namespace LitT
{
	LitTable Create(int size)
	{
		LitTable littable;
		littable.maxsize = size;
		littable.size = 0;
		littable.table = new Entry[size];
		return littable;
	}

	void Add(LitTable& littable, Entry entry)
	{
		++littable.size;
		if (littable.size > littable.maxsize)
			ERROR_THROW(121);
		littable.table[littable.size - 1] = entry;
		return;
	}

	Entry GetEntry(LitTable& littable, int str, int pos)
	{
		bool check = 0;
		for (int i = 0; ((i < littable.size)&&(littable.table[i].strNumInLT <= str)); ++i)
		{
			if ((littable.table[i].strNumInLT == str) && (littable.table[i].posNumInLT == pos))
				return littable.table[i];
		}
		ERROR_THROW(120);
	}

	void Delete(LitTable& littable)
	{
		delete[] littable.table;
		return;
	}
}