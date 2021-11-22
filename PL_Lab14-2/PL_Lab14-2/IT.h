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
}