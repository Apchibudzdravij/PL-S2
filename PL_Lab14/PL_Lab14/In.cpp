#include "stdafx.h"
#include <fstream>
#include <string>
#include "In.h"
#include "Error.h"
#include "FST.h"

#define MAX_STRING_LENGTH 255

namespace In
{
	int lns;
	int gnr;
	char text[1000];
	IN getin(wchar_t infile[])
	{
		setlocale(LC_ALL, ".1251");
		IN inn;
		inn.size = 0;
		inn.lines = 1;
		inn.ignor = 0;
		int start = 0, finish = 0;
		char text[1000] = {};
		char abc[100] = {}; int cabc = 0;
		short position = 0;
		std::ifstream input(infile, std::ios::in);
		if (!input)
		{
			ERROR_THROW(110);
		}
		int positionInLine = 0;//Счётчик, увеличивает значение на 1 с каждым символом, обнуляется при переходе на новую строку
		while(!input.eof())
		{
			char ch = input.get();
			/*if (start == 1)//Это нужно только в 13 лаб.р., но удалять жалко
			{
				if (ch != '\n')
					abc[cabc] = ch;
				cabc++;
			}*/

			++positionInLine;
			if ((((int)ch >= 0)&&(inn.code[(int)ch] == IN::F))||(((int)ch < 0)&&(inn.code[(int)(ch) + 256] == IN::F)))//Проверяет на наличие ошибочных символов и выдаёт ошибку 111, если такие есть
				ERROR_THROW_IN(111, inn.lines, positionInLine);
			if ((int)ch >= 0)//Этот и след. ифы проверяют нужно ли игнорировать символ
			{
				if (inn.code[(int)ch] == IN::I)
				{
					++inn.ignor;
					continue;
				}
			}
			else if ((int)ch < 0)
			{
				if (inn.code[(int)ch + 256] == IN::I)
				{
					++inn.ignor;
					continue;
				}
			}
			/*if (inn.code[(int)ch] == '|') // Эта часть кода нужна лишь для 13 лаб.р., где сепаратор "|" отделял строки. В 14 этот блок не нужен. 
			{
				++inn.size;
				text[position] = '|';
				++position;
				if (start == 1)
				{
					start = 0;
					finish = 1;
					cabc = 0;

				}
				if (start == 0)
				{
					start = 1;
				}
				if (finish == 1)
				{
					if (abc[0] == 'a') {
						FST::called(abc);
					}
					finish = 0;

					for (int i = 0; i < 100; i++) {
						abc[i] = NULL;
					}

				}
			}*/
			if ((position > 0) && (text[position - 1] == '\n'))
			{
				text[position] = (char)11;
				++position;
			}
			if ((ch == ' ') && ((text[position - 1] == '\n') || (text[position - 1] == (char)11)))//Удаляет пробелы в начале строки
			{
				continue;
			}
			if ((ch == '	') && ((text[position - 1] == '\n') || (text[position - 1] == (char)11)))//Удаляет табуляцию в начале строки
			{
				continue;
			}
			if (ch == '\n')
			{
				++inn.lines;
				positionInLine = 0;
			}
			if ((ch == '\n') && (text[position - 1] == ' '))//Удаляет лишние пробелы перед переходом на следующую строку
			{
				for (int ind = 2; ind < 100; ++ind)
					if (text[position - ind] == ' ')
						--position, --ind;
					else
						break;
				text[position - 1] = '\n';
				if (text[position - 2] == ',')
				{
					text[position - 1] = ' ';
					text[position] = (char)11;
					++position;
				}
				continue;
			}
			if ((ch == '\n') && (text[position - 1] == '	'))//Удаляет лишнюю табуляцию перед переходом на следующую строку
			{
				for (int ind = 2; ind < 100; ++ind)
					if (text[position - ind] == '	')
						--position, --ind;
					else
						break;
				text[position - 1] = '\n';
				if (text[position - 2] == ',')
				{
					text[position - 1] = ' ';
					text[position] = (char)11;
					++position;
				}
				continue;
			}
			if ((ch == '\n') && (text[position - 1] == ','))//Убирает переход на новую строку, если прежняя закончилась запятой
			{
				ch = ' ';
				text[position] = (char)11;
				++position;
			}

			bool check = 1;

			for (int i = 1; i < MAX_STRING_LENGTH; ++i)
				if (text[position - i] == (char)(39))
					check = !check;

			if (check)
			{
				if ((ch == ' ') && (text[position - 1] == ' '))//Убирает пробелы, идущие после пробелов
				{
					continue;
				}
				if ((ch == '	') && (text[position - 1] == ' '))//Убирает tab, идущие после пробелов
				{
					continue;
				}
				if ((ch == ' ') && (text[position - 1] == '	'))//Убирает пробелы, идущие после tab
				{
					continue;
				}
				if ((ch == '	') && (text[position - 1] == '	'))//Убирает tab, идущие после tab
				{
					continue;
				}
				if ((ch == '\n') && ((text[position - 1] == '\n') || (text[position - 1] == (char)11)))//Убирает пустые строки
				{
					text[position] = (char)11;
					++position;
					continue;
				}
			}
			
			if ((int)ch >= 0)
			{
				if (inn.code[(int)ch] == IN::T)
				{
					++inn.size;
					if (ch == '\0')
					{
						++inn.lines;
						/*if (start == 1)//Это тоже только для 13 лаб.р., но тоже жалко удалять
						{
							start = 0;
							finish = 0;
							for (int i = 0; i < 100; i++)
							{
								abc[i] = NULL;
							}
						}*/
					}
					text[position] = ch;
					++position;
				}
			}
			else if ((int)ch < 0)
			{
				if (inn.code[(int)ch + 256] == IN::T)
				{
					++inn.size;
					if (ch == '\0')
						++inn.lines;
					text[position] = ch;
					++position;
				}
			}
			inn.text = &text[0];
		}
		std::ofstream stream;
		stream.open("in.txt.out.txt", std::ios_base::out);
		stream << text;
		stream.close();
		return inn;
	}
}