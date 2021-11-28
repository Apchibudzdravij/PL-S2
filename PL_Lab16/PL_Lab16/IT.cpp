#include "stdafx.h"
#include <fstream>
#include <string>
#include "Error.h"
#include "FST.h"
#include "LT.h"
#include "IT.h"
#include "LitT.h"

namespace IT
{
	IdTable Create(int size)
	{
		IdTable idtable;
		idtable.maxsize = size;
		idtable.size = 0;
		idtable.table = new Entry[size];
		return idtable;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		++idtable.size;
		if (idtable.size > idtable.maxsize)
			ERROR_THROW(119);
		idtable.table[idtable.size - 1] = entry;
		return;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		if ((n > idtable.size) || (n < 0))
			ERROR_THROW(117);
		return idtable.table[n - 1];
	}

	int IsId(IdTable& idtable, char* id, int len)
	{
		bool ohmy = true;
		for (int i = 0; i < idtable.size; ++i)
		{
			for (int j = 0; j < len; ++j)
				if (idtable.table[i].id[j] != id[j])
				{
					ohmy = false;
					break;
				}
			if (ohmy)
			{
				return i + 1;
			}
			ohmy = true;
		}
		return TI_NULLIDX;
	}
	int IsId(IdTable& idtable, std::string id, int len)
	{
		char* buffId = new char[id.length() + 1];
		for (int i = 0; ((i < ID_MAXSIZE)&&(i < len)); ++i)
			buffId[i] = id[i];
		buffId[len] = id[id.length()];
		bool ohmy = true;
		for (int i = 0; i < idtable.size; ++i)
		{
			for (int j = 0; j < len; ++j)
				if (idtable.table[i].id[j] != buffId[j])
				{
					ohmy = false;
					break;
				}
			if (ohmy)
			{
				return i + 1;
			}
			ohmy = true;
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		return;
	}

	void CreateFromFile(LT::LexTable& lextable, IdTable& idtable, LitT::LitTable& littable, wchar_t infile[])
	{
		setlocale(LC_ALL, ".1251");

		std::ifstream input;
		input.open("in.txt.out.txt", std::ios::in);//������� ���� - ��������� ������ in.cpp
		if (!input)
		{
			ERROR_THROW(201);
		}
		int elementLeng = 0;
		std::string element;
		bool emptyIDvalue = false;

		int line_ok = 1;//������� ����� ��� ��
		std::ofstream stream;//������ � ��� ������� ������ ����� ������ ������
		stream.open("in.txt.out.txt.LT.log", std::ios_base::app);
		stream << "000 ";
		stream.close();

		int strNumber = 0;
		int posNumber = 0;
		char lex; //������ ��� �������� ������� �������

		bool integerWas = 0;//������� ��� �������� ���� ����������� ����������
		bool stringWas = 0;

		bool isFunc = 0;//������ ��� ��������� ����� �������� ��� ����������
		std::string ifisFunc;
		bool isMain = 0;

		bool strlenWas = 0;
		bool substrWas = 0;//��������� ������ ��������-�����, ���� ��� ��������� ������� ��� ���

		char ch = input.get();
		while (ch != '�')//�������, �� ��� ����. ������ � ����� "�" �����������.
		{
			if (ch == '\v')//��������� ��� ������ ��� �������� �����, �� �� �������� ������� �����
			{
				++strNumber;
				posNumber = -1;
			}
			else if (((ch == '	') && (element[0] != (char)39)) || ((ch == ' ') && (element[0] != (char)39)) || (ch == ',') || (ch == ';') || (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '{') || (ch == '}') || (ch == '(') || (ch == ')') || (ch == '=') || (ch == '\n'))
			{
				char* buffWord = new char[element.length() + 1];
				for (int i = 0; i < element.length() + 1; ++i)
					buffWord[i] = element[i];//�������� �� ������� ����� string � char[], �� ��� cpp, ������� ���� ������ �����
				if (buffWord[0] != '\0')//"������" ������ � string �������� ���� ������ - \0
				{
					lex = FST::called(buffWord);
					int buffIntWord = element.length() + 1;
					char* altBuffWord = new char[buffIntWord];
					for (int i = 0; i < buffIntWord; ++i)
						altBuffWord[i] = buffWord[i];
					bool buffBoolWord = false;
					if (element == "integer")
						integerWas = true;
					else if (element == "string")
						stringWas = true;
					else if (element == "function")
						isFunc = true;
					else if (element == "main")
						isMain = true;
				//��������� �������, ������������� � ������� � ��������������� �������
					if (lex == LEX_LITERAL)//�������
					{
						LitT::Entry literalEntry;
						for (int i = 0; i < element.length() + 1; ++i)
							literalEntry.literal[i] = buffWord[i];
						literalEntry.actualSize = elementLeng;
						literalEntry.strNumInLT = strNumber;
						literalEntry.posNumInLT = posNumber;
						LitT::Add(littable, literalEntry);
					}
					else if (lex == LEX_ID)//ID
					{
						/*for (int i = 2; i < lextable.size; ++i)
						{
							if (lextable.table[lextable.size - i].lexema[0] == LEX_RIGHTHESIS)
								break;
							else if (lextable.table[lextable.size - i].lexema[0] == LEX_LEFTHESIS)
							{
								buffIntWord = strlen(idtable.table[lextable.table[lextable.size - i - 1].idxTI].id);
								buffBoolWord = true;
								break;
							}
						}
						if (buffBoolWord)
						{
							int i = 0;
							for (; i < strlen(idtable.table[lextable.table[lextable.size - i - 1].idxTI].id); ++i)
								altBuffWord[i] = idtable.table[lextable.table[lextable.size - i - 1].idxTI].id[i];
							for (; i < buffIntWord; ++i)
								altBuffWord[i] = buffWord[i - strlen(idtable.table[lextable.table[lextable.size - i - 1].idxTI].id)];
						}*/
						/*else
						{
							for (int i = 0; i < buffIntWord; ++i)
								altBuffWord[i] = buffWord[i];
						}*/
						if ((buffIntWord > 5) && (element != "substr") && (element != "strlen"))
							ERROR_THROW_IN(118, strNumber, posNumber);
						bool isNew = 1;
						bool isSystem = 0;
						std::string temporary;
						temporary = altBuffWord;
						if ((isFunc)&&((element != "substr") && (element != "strlen")))
						{
							std::string buffStrIfIsFunc = "";
							buffStrIfIsFunc += ifisFunc;
							buffStrIfIsFunc += element;
							buffIntWord = buffStrIfIsFunc.length() + 1;
							for (int i = 0; i < buffIntWord; ++i)
							{
								altBuffWord[i] = buffStrIfIsFunc[i];
							}
						}
						if ((isMain) && ((element != "substr") && (element != "strlen")))
						{
							std::string buffStrIfIsFunc = "";
							buffStrIfIsFunc += "m";
							buffStrIfIsFunc += element;
							buffIntWord = buffStrIfIsFunc.length() + 1;
							for (int i = 0; i < buffIntWord; ++i)
							{
								altBuffWord[i] = buffStrIfIsFunc[i];
							}
						}
						if ((temporary == "substr"))
						{
							isSystem = true;
						}
						else if ((temporary == "strlen"))
						{
							isSystem = true;
						}
						if (IsId(idtable, altBuffWord, buffIntWord) != TI_NULLIDX)
							isNew = false;
						if ((isNew)&&(!isSystem))//���� ������������� ����� �� ���������� � �� �������� ��������� ���������
						{
							Entry idEntry;
							bool isNotP = true;
							bool isNotF = true;
							idEntry.idxfirstLE = strNumber;
							for (int i = 0; i < ID_MAXSIZE; ++i)
								idEntry.id[i] = altBuffWord[i];
							//emptyIDvalue = true;
							if (lextable.table[lextable.size - 1].lexema[0] == LEX_FUNCTION)
							{
								idEntry.idtype = F;
								isNotF = false;
								ifisFunc = element;
								if ((lextable.table[lextable.size - 2].lexema[0] == LEX_INTEGER) && (integerWas))
								{
									idEntry.iddatatype = INT;
									integerWas = false;
								}
								else if ((lextable.table[lextable.size - 2].lexema[0] == LEX_STRING) && (stringWas))
								{
									idEntry.iddatatype = STR;
									stringWas = false;
								}
							}
							else
								for (int i = 2; ((i < lextable.size)&&(lextable.table[lextable.size - i].lexema[0] != LEX_SEMICOLON)); ++i)
								{
									if (lextable.table[lextable.size - i].lexema[0] == LEX_RIGHTHESIS)
										break;
									else if (lextable.table[lextable.size - i].lexema[0] == LEX_LEFTHESIS)
									{
										idEntry.idtype = P;
										isNotP = false;
									}
								}
							if ((lextable.table[lextable.size - 1].lexema[0] == LEX_INTEGER)&&(integerWas))
							{
								idEntry.iddatatype = INT;
								integerWas = false;
							}
							else if ((lextable.table[lextable.size - 1].lexema[0] == LEX_STRING) && (stringWas))
							{
								idEntry.iddatatype = STR;
								stringWas = false;
							}
							if ((isNotP) && (isNotF))
								idEntry.idtype = V;
							Add(idtable, idEntry);//�������� � ������� ������ � ���������� ����� ��������������(����� ��� ���� ��� �������� ��� �������) � ���������
						}
						if ((!((strlenWas) && (substrWas))) && (isSystem))
						{
							if (!(((temporary == "strlen") && (strlenWas)) || ((temporary == "substr") && (substrWas))))
							{
								Entry idSysEntry;
								idSysEntry.idxfirstLE = strNumber;
								for (int i = 0; i < ID_MAXSIZE; ++i)
									idSysEntry.id[i] = temporary[i];
								idSysEntry.id[ID_MAXSIZE] = temporary[temporary.length()];
								idSysEntry.idtype = F;
								idSysEntry.iddatatype = STR;
								Add(idtable, idSysEntry);
								if (temporary == "strlen")
									strlenWas = true;
								else if (temporary == "substr")
									substrWas = true;
							}
						}
					}
					if ((ch == LEX_BRACELET) && (isFunc == true))
					{
						isFunc = false;
					}
					//� ����� ������ � �������, � ������������� �������� ���������, ������� ��� �������� ������
					LT::Entry lexEntry;
					lexEntry.lexema[0] = lex;
					lexEntry.sn = strNumber;
					lexEntry.idxTI = IsId(idtable, altBuffWord, element.length() + 1);
					LT::Add(lextable, lexEntry);

					IT::Entry buffEntr;
					int posInIdTable;
					if (lex == LEX_IS)//���� ������� "=", �� ����������� ��������� �������������, ����� ����� ������ � ���� �������� �������
					{
						buffEntr = IT::GetEntry(idtable, lextable.table[lextable.size - 2].idxTI);
						posInIdTable = lextable.table[lextable.size - 2].idxTI;
						emptyIDvalue = true;
					}
					if ((lex == LEX_SEMICOLON)&&(emptyIDvalue))//�� ���� ���� ";" ����� ����������� �������������� 
					{
						int counter = 0;
						for (int i = 0; lextable.table[lextable.size - 2 - i].lexema[0] != LEX_IS; ++i)
						{
							buffEntr.value.lexstring[i] = &lextable.table[lextable.size - 2 - i];
							++counter;
						}
						idtable.table[posInIdTable].value = buffEntr.value;//������� � �� � ������� �������������� ������� ��� ��������.
						idtable.table[posInIdTable].lexInt = counter;
						emptyIDvalue = false;
					}
				}

				element.clear();//���� ������������ ������
				elementLeng = 0;

				if ((ch != ' ') && (ch != '	') && (ch != '\n'))//���� ��� ����������, �� ��� �������� �� ���� �� �������� �����
				{
					std::string symbol;
					symbol += ch;
					char* buffChar = new char[symbol.length() + 1];
					for (int i = 0; i < symbol.length() + 1; ++i)
						buffChar[i] = symbol[i];
					FST::called(buffChar);
				}
				if (ch == '\n')
				{
					goto badmetka;//�������� ����
				}

				if ((ch == ',') || (ch == ';') || (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '{') || (ch == '}') || (ch == '(') || (ch == ')') || (ch == '='))
				{
					LT::Entry lexEntry;
					if ((ch == '+') || (ch == '-') || (ch == '*') || (ch == '/'))
						lexEntry.lexema[0] = 'v';
					else
						lexEntry.lexema[0] = ch;
					lexEntry.sn = strNumber;
					lexEntry.idxTI = TI_NULLIDX;
					LT::Add(lextable, lexEntry);
				}
			}
			else if ((ch != '\v')&&(ch != '\n'))//������� ������ � ����-������ �������� ������ � �������� ����� ��������� �����, �� �� �������� ���������
			{
				element += ch;
				++elementLeng;
			}
			else if (ch == '\n')//����� ������ else? ����� � ����� �� �����������...
			{
badmetka:		std::ofstream stream;
				stream.open("in.txt.out.txt.LT.log", std::ios_base::app);//������ � ��� �� ������� ������ � ����� ����� ������
				stream.put('\n');
				if (line_ok < 10)
					stream << "00" << line_ok << " ";
				else if (line_ok < 100)
					stream << "0" << line_ok << " ";
				else
					stream << line_ok << " ";
				stream.close();
				++line_ok;
			}
			ch = input.get();//���� ��������� ������
			++posNumber;//������� ������� �������� � ������
		}
		

		input.close();

		return;
	}

	void ShowAll(IdTable& idtable)
	{
		for (int i = 0; i < idtable.size; ++i)
		{
			printf("%5s%4i%2i%2i\n", idtable.table[i].id, idtable.table[i].idxfirstLE, idtable.table[i].idtype, idtable.table[i].iddatatype);

		}
			printf("\n");
		return;
	}
}