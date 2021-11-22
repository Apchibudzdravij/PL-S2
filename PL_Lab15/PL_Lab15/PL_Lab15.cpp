#include "stdafx.h"
#include <locale>
#include <cwchar>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "FST.h"
#include "IT.h"
#include "LT.h"
#include "LitT.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, ".1251");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in);//создал промежуточный файл
		Log::WriteIn(log, in);
		LT::LexTable myOwnLexTable = LT::Create(65536);//пустая таблицу лексем с ограничением 
		IT::IdTable myOwnIdTable = IT::Create(65536);//пустая ТИ с ограничением
		LitT::LitTable myLiterals = LitT::Create(65536);//пустая таблица литералов с ограничением

		IT::CreateFromFile(myOwnLexTable, myOwnIdTable, myLiterals, parm.out);//заполнение таблиц по промежуточному файлу
		
		IT::RemakeLTWithPolishNotation(myOwnLexTable, myOwnIdTable);//изменение таблицы лексем с замещением выражений в инфексном виде на постфиксный
		LT::ShowLT(myOwnLexTable);

		Log::Close(log);
		LT::Delete(myOwnLexTable);
		IT::Delete(myOwnIdTable);
		LitT::Delete(myLiterals);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	};

	system("pause");
	return 0;
}