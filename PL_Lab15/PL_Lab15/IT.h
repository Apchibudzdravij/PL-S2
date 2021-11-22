#pragma once
#include "LT.h"
#include "LitT.h"

#define ID_MAXSIZE		5			//макс. кол-во символов в ID
#define TI_MAXSIZE		4096		//максю кол-во строк в ТИ
#define TI_INT_DEFAULT	0x00000000	//знач. по умолчанию integer
#define TI_STR_DEFAULT	0x00		//знач. по умолчанию string
#define TI_NULLIDX		0xffffffff	//нет элемента ТИ
#define TI_STR_MAXSIZE	255

namespace IT
{
	enum IDDATATYPE { INT = 1, STR = 2 };		//ТД идентификаторов
	enum IDTYPE { V = 1, F = 2, P = 3, L = 3 }; //типы идентиф.: переменная, функция, параметр, литерал

	struct Entry					//строка ТИ
	{
		int			idxfirstLE;		//индекс первой строки в ТЛ
		char		id[ID_MAXSIZE+1]; //id, усекается до ID_MAXSIZE
		IDDATATYPE	iddatatype;		//тип данных
		IDTYPE		idtype;			//тип идентификатора
		union
		{
			int vint;	//знач. integer
			struct
			{
				char len;
				char str[TI_STR_MAXSIZE - 1];
			}  vstr;	//знач. string
			LT::Entry** lexstring;//если id вычисляемый, то храню цепочку, чтобы вычисления проводились лишь когда это будет нужно
		}			value;			//значение идентификатора
		int lexInt;//длина цепочки lexstring
	};

	struct IdTable		//Экземпляр ТИ - таблицы идентификаторов
	{
		int maxsize;
		int size;
		Entry* table;
	};

	IdTable Create(int size);						//создать ТИ
	void Add(IdTable& idtable, Entry entry);		//добавить строку в ТИ

	Entry GetEntry(IdTable& idtable, int n);		//получить строку ТИ
	int IsId(IdTable& idtable, char* id, int len);//Вернуть номер строки или TI_NULLIDX, зная экземпляр ТИ и идентификатор
	void Delete(IdTable& idtable);					//Удалить ТИ

	void CreateFromFile(LT::LexTable& lextable, IdTable& idtable, LitT::LitTable& littable, wchar_t infile[]);
	void ShowAll(IdTable& idtable);

	void RemakeLTWithPolishNotation(LT::LexTable& lextable, IdTable& idtable); //изменить выражения в таблице лексем польской записью
}

namespace PolNot
{
	inline void RemoveTheInfixLine(int lextable_pos, LT::LexTable& lextable, int len, LT::Entry* entry, int lexlen)
	{
		int bufflen = 0;
		LT::Entry newEntry;
		newEntry.lexema[0] = '\f';
		newEntry.idxTI = LT_TI_NULLIDX;
		while (bufflen < len)
		{
			lextable.table[lextable_pos + bufflen] = entry[bufflen];
			++bufflen;
		}
		while (bufflen < lexlen)
		{
			newEntry.sn = lextable.table[lextable_pos].sn;
			lextable.table[lextable_pos + bufflen] = newEntry;
			++bufflen;
		}
	}


	inline bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable idtable)
	{
		int pos = lextable_pos;
		bool success = true;

		int lineLen = 0;
		LT::Entry* entryLine = new LT::Entry[50];
		int stackLen = 0;
		LT::Entry* stack = new LT::Entry[50];

		int commacounter = 0;

		while (lextable.table[pos].lexema[0] != ';')
		{
			if (((lextable.table[pos].lexema[0] == 'v') || (lextable.table[pos].lexema[0] == 'w')) && ((stackLen == 0) || (stack[stackLen].lexema[0] == LEX_LEFTHESIS)))
			{
				stack[stackLen] = lextable.table[pos];
				++stackLen;
			}
			else if (lextable.table[pos].lexema[0] == LEX_PLUS)
			{
				while ((stack[stackLen-1].lexema[0] == LEX_STAR) || (stack[stackLen-1].lexema[0] == LEX_PLUS))
				{
					entryLine[lineLen-1] = stack[stackLen-1];
					--stackLen;
					++lineLen;
				}
				stack[stackLen] = lextable.table[pos];
				++stackLen;
			}
			else if (lextable.table[pos].lexema[0] == LEX_STAR)
			{
				while (stack[stackLen-1].lexema[0] == LEX_STAR)
				{
					entryLine[lineLen] = stack[stackLen];
					--stackLen;
					++lineLen;
				}
				stack[stackLen] = lextable.table[pos];
				++stackLen;
			}
			else if (lextable.table[pos].lexema[0] == LEX_COMMA)
			{
				if ((commacounter == 0)&&(lextable.table[pos-1].lexema[0] == LEX_ID))
				{
					--lineLen;
				}
				++commacounter;
			}
			else if (lextable.table[pos].lexema[0] == LEX_LEFTHESIS)
			{
				stack[stackLen] = lextable.table[pos];
				++stackLen;
			}
			else if (lextable.table[pos].lexema[0] == LEX_RIGHTHESIS)
			{
				if (commacounter != 0)
				{
					entryLine[lineLen].lexema[0] = '@';
					entryLine[lineLen].idxTI = LT_TI_NULLIDX;
					entryLine[lineLen].sn = lextable.table[pos].sn;
					++lineLen;
					entryLine[lineLen].lexema[0] = (char)(commacounter + (int)'1');
					entryLine[lineLen].idxTI = LT_TI_NULLIDX;
					entryLine[lineLen].sn = lextable.table[pos].sn;
					++lineLen;
				}
				while (stackLen != 0)
				{
					if (stack[stackLen - 1].lexema[0] != LEX_LEFTHESIS)
					{
						entryLine[lineLen] = stack[stackLen - 1];
						--stackLen;
						++lineLen;
					}
					else
						--stackLen;
				}
			}
			else if ((lextable.table[pos].lexema[0] != LEX_PLUS) && (lextable.table[pos].lexema[0] != LEX_STAR) && (lextable.table[pos].lexema[0] != LEX_COMMA) && (lextable.table[pos].lexema[0] != LEX_RIGHTHESIS) && (lextable.table[pos].lexema[0] != LEX_LEFTHESIS))
			{
				entryLine[lineLen] = lextable.table[pos];
				++lineLen;
			}
			++pos;
		}
		if (stackLen != 0)
		{
			while (stackLen != 0)
			{
				entryLine[lineLen] = stack[stackLen-1];
				--stackLen;
				++lineLen;
			}
		}
		else
		{
			success = false;
		}
		RemoveTheInfixLine(lextable_pos, lextable, lineLen, entryLine, pos - lextable_pos);
		return success;
	}
}