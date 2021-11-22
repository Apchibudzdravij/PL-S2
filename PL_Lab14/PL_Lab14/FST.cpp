#include "FST.h"
namespace FST
{
	bool step(FST& fst, short* &rstates)
	{
		bool rc = false;
		std::swap(rstates, fst.rstates);
		for (short i = 0; i < fst.nstates; ++i)
		{
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; ++j)
				{
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position;
						rc = true;
					}
				}
		}
		return rc;
	}
	bool execute(FST& fst)
	{
		short* rstates = new short[fst.nstates]; memset(rstates, 0xff, sizeof(short)*fst.nstates);
		short lstring = strlen(fst.string);
		bool rc = true;
		for (short i = 0; i < lstring && rc; ++i)
		{
			++fst.position;
			rc = step(fst, rstates);
		}
		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}
	void called(char mas[]) {
		FST fst(
			mas,
			4,
			NODE(1, RELATION('a', 0)),
			NODE(2, RELATION('c', 1), RELATION('b', 1)),
			NODE(4, RELATION('d', 2), RELATION('b', 2), RELATION('f', 2), RELATION('e', 2)),
			NODE(4, RELATION('f', 3), RELATION('b', 3), RELATION('d', 3), RELATION('e', 3)),
			NODE()
		);
		std::ofstream stream;
		stream.open("in.txt.log", std::ios_base::app);
		if (execute(fst))
		{
			std::cout << "Цепочка " << fst.string << " распознана" << "\n";
			stream << "Цепочка " << fst.string << " распознана" << "\n";
		}
		else
		{
			std::cout << "Цепочка " << fst.string << " нe распознана\n";
			stream << "Цепочка " << fst.string << " не распознана\n";
		}
		stream.close();
	}
}