#include "stdafx.h"
#include "LT.h"
#include "FST.h"

namespace FST
{
	bool step(FST& fst, short*& rstates) {
		bool rc = false;
		std::swap(rstates, fst.rstates);
		for (short i = 0; i < fst.nstates; i++) {
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++) {
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) {
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					};
				};
		};
		return rc;
	};

	bool execute(FST& fst) {
		short* rstates = new short[fst.nstates]; memset(rstates, 0xff, sizeof(short) * fst.nstates);
		short lstring = strlen(fst.string);
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;
			rc = step(fst, rstates);
		};
		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}
	RELATION::RELATION(char c, short nn) {
		symbol = c;
		nnode = nn;
	}


	NODE::NODE() {
		n_relation = 0;
		RELATION* relations = NULL;
	};

	NODE::NODE(short n, RELATION rel, ...) {
		n_relation = n;
		RELATION* p = &rel;
		relations = new RELATION[n];
		for (short i = 0; i < n; i++) {
			relations[i] = p[i];
		}
	}
	FST::FST(char* s, short ns, NODE n, ...) {
		string = s;
		nstates = ns;
		nodes = new NODE[ns];
		NODE* p = &n;
		for (int k = 0; k < ns; k++) nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short) * nstates);
		rstates[0] = 0;
		position = -1;
	}
	
	char called(char mas[])
	{
		std::ofstream stream;
		stream.open("in.txt.out.txt.LT.log", std::ios_base::app);

		FST integ(mas, FST_INTEGER);
		FST strin(mas, FST_STRING);
		FST funct(mas, FST_FUNCTION);
		FST decla(mas, FST_DECLARE);
		FST retur(mas, FST_RETURN);
		FST  prnt(mas, FST_PRINT);
		FST   man(mas, FST_MAIN);
		FST semic(mas, FST_SEMICOLON);
		FST comma(mas, FST_COMMA);
		FST leftb(mas, FST_LEFTBRACE);
		FST brace(mas, FST_BRACELET);
		FST lefth(mas, FST_LEFTHESIS);
		FST rghth(mas, FST_RIGHTHESIS);
		FST  plus(mas, FST_PLUS);
		FST minus(mas, FST_MINUS);
		FST  star(mas, FST_STAR);
		FST dirsl(mas, FST_DIRSLASH);
		FST    is(mas, FST_IS);

		if (execute(integ))
		{
			stream.put(LEX_INTEGER);
			stream.close();
			return LEX_INTEGER;
		}
		else if (execute(strin))
		{
			stream.put(LEX_STRING);
			stream.close();
			return LEX_STRING;
		}
		else if (execute(funct))
		{
			stream.put(LEX_FUNCTION);
			stream.close();
			return LEX_FUNCTION;
		}
		else if (execute(decla))
		{
			stream.put(LEX_DECLARE);
			stream.close();
			return LEX_DECLARE;
		}
		else if (execute(retur))
		{
			stream.put(LEX_RETURN);
			stream.close();
			return LEX_RETURN;
		}
		else if (execute(prnt))
		{
			stream.put(LEX_PRINT);
			stream.close();
			return LEX_PRINT;
		}
		else if (execute(man))
		{
			stream.put(LEX_MAIN);
			stream.close();
			return LEX_MAIN;
		}
		else if (execute(semic))
		{
			stream.put(LEX_SEMICOLON);
			stream.close();
			return LEX_SEMICOLON;
		}
		else if (execute(comma))
		{
			stream.put(LEX_COMMA);
			stream.close();
			return LEX_COMMA;
		}
		else if (execute(leftb))
		{
			stream.put(LEX_LEFTBRACE);
			stream.close();
			return LEX_LEFTBRACE;
		}
		else if (execute(brace))
		{
			stream.put(LEX_BRACELET);
			stream.close();
			return LEX_BRACELET;
		}
		else if (execute(lefth))
		{
			stream.put(LEX_LEFTHESIS);
			stream.close();
			return LEX_LEFTHESIS;
		}
		else if (execute(rghth))
		{
			stream.put(LEX_RIGHTHESIS);
			stream.close();
			return LEX_RIGHTHESIS;
		}
		else if (execute(plus))
		{
			stream.put(LEX_PLUS);
			stream.close();
			return LEX_PLUS;
		}
		else if (execute(minus))
		{
			stream.put(LEX_MINUS);
			stream.close();
			return LEX_MINUS;
		}
		else if (execute(star))
		{
			stream.put(LEX_STAR);
			stream.close();
			return LEX_STAR;
		}
		else if (execute(dirsl))
		{
			stream.put(LEX_DIRSLASH);
			stream.close();
			return LEX_DIRSLASH;
		}
		else if (execute(is))
		{
			stream.put(LEX_IS);
			stream.close();
			return LEX_IS;
		}
		else if ((mas[0] == '0') || (mas[0] == '1') || (mas[0] == '2')|| (mas[0] == '3') || (mas[0] == '4') || (mas[0] == '5') || (mas[0] == '6') || (mas[0] == '7') || (mas[0] == '8') || (mas[0] == '9'))
		{
			stream.put(LEX_LITERAL);
			stream.close();
			return LEX_LITERAL;
		}
		else if (mas[0] == (char)39)
		{
			stream.put(LEX_LITERAL);
			stream.close();
			return LEX_LITERAL;
		}
		else
		{
			stream.put(LEX_ID);
			stream.close();
			return LEX_ID;
		}
	}
}