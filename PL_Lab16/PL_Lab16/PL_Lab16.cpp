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
#include "MFST.h"
#include "GRB.h"

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
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		LT::LexTable myOwnLexTable = LT::Create(65536);
		IT::IdTable myOwnIdTable = IT::Create(65536);
		LitT::LitTable myLiterals = LitT::Create(65536);

		IT::CreateFromFile(myOwnLexTable, myOwnIdTable, myLiterals, parm.out);
		//LT::ShowLT(myOwnLexTable);

		//LitT::GetEntry(myLiterals, 44444, 555555);
		//IT::GetEntry(myOwnIdTable, 66666);
		//LT::GetEntry(myOwnLexTable, 77777);

		MFST_TRACE_START
			MFST::Mfst mfst(myOwnLexTable, GRB::getGreibach());
		mfst.start();

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