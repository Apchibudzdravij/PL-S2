#pragma once
#include "LT.h"
#include "LitT.h"

#define ID_MAXSIZE		5			//����. ���-�� �������� � ID
#define TI_MAXSIZE		4096		//����� ���-�� ����� � ��
#define TI_INT_DEFAULT	0x00000000	//����. �� ��������� integer
#define TI_STR_DEFAULT	0x00		//����. �� ��������� string
#define TI_NULLIDX		0xffffffff	//��� �������� ��
#define TI_STR_MAXSIZE	255

namespace IT
{
	enum IDDATATYPE { INT = 1, STR = 2 };		//�� ���������������
	enum IDTYPE { V = 1, F = 2, P = 3, L = 3 }; //���� �������.: ����������, �������, ��������, �������

	struct Entry					//������ ��
	{
		int			idxfirstLE;		//������ ������ ������ � ��
		char		id[ID_MAXSIZE+1]; //id, ��������� �� ID_MAXSIZE
		IDDATATYPE	iddatatype;		//��� ������
		IDTYPE		idtype;			//��� ��������������
		union
		{
			int vint;	//����. integer
			struct
			{
				char len;
				char str[TI_STR_MAXSIZE - 1];
			}  vstr;	//����. string
			LT::Entry** lexstring;//���� id �����������, �� ����� �������, ����� ���������� ����������� ���� ����� ��� ����� �����
		}			value;			//�������� ��������������
		int lexInt;//����� ������� lexstring
	};

	struct IdTable		//��������� �� - ������� ���������������
	{
		int maxsize;
		int size;
		Entry* table;
	};

	IdTable Create(int size);						//������� ��
	void Add(IdTable& idtable, Entry entry);		//�������� ������ � ��

	Entry GetEntry(IdTable& idtable, int n);		//�������� ������ ��
	int IsId(IdTable& idtable, char* id, int len);//������� ����� ������ ��� TI_NULLIDX, ���� ��������� �� � �������������
	void Delete(IdTable& idtable);					//������� ��

	void CreateFromFile(LT::LexTable& lextable, IdTable& idtable, LitT::LitTable& littable, wchar_t infile[]);
	void ShowAll(IdTable& idtable);

	void RemakeLTWithPolishNotation(LT::LexTable& lextable, IdTable& idtable); //�������� ��������� � ������� ������ �������� �������
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