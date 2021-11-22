#pragma once

#define LEXEMA_FIXSIZE	1			//фиксированный размер лексемы
#define LT_MAXSIZE		4096		//макс. кол-во строк в таблице лексем
#define LT_TI_NULLIDX	0xffffffff	//нет элемента таблицы идентификааторов
#define LEX_INTEGER		't'
#define LEX_STRING		't'
#define LEX_ID			'i'
#define LEX_LITERAL		'l'
#define LEX_FUNCTION	'f'
#define LEX_DECLARE		'd'
#define LEX_RETURN		'r'
#define LEX_PRINT		'p'
#define LEX_MAIN		'm'
#define LEX_SEMICOLON	';'
#define LEX_COMMA		','
#define LEX_LEFTBRACE	'{'
#define LEX_BRACELET	'}'
#define LEX_LEFTHESIS	'('
#define LEX_RIGHTHESIS	')'
#define LEX_PLUS		'v'
#define LEX_MINUS		'v'
#define LEX_STAR		'v'
#define LEX_DIRSLASH	'v'
#define LEX_IS			'='

namespace LT
{
	struct Entry						//Строка таблицы лексем
	{
		char lexema[LEXEMA_FIXSIZE];
		int sn;							//Номер строки в исх. тексте
		int idxTI;		//индекс в таблице инд. или LT_TI_NULLDX
	};

	struct LexTable					//экземпляр таблицы лексем
	{
		int maxsize;				//ёмкость ТЛ
		int size;					//текущий размер ТЛ
		Entry* table;				//массив строк ТЛ
	};

	LexTable Create(int size);					//создать ТЛ, зная ёмкость
	void Add(LexTable& lextable, Entry entry);	//добавить строку в ТЛ

	Entry GetEntry(LexTable& lextable, int n);	//получить строку ТЛ
	void Delete(LexTable& lextable);			//удалить ТЛ
}