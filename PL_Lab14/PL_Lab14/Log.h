#pragma once
#include <fstream>
#include <iostream>
#include <time.h>
#include <thread>
#include "In.h"
#include "Parm.h"
#include "Error.h"
namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE];
		std::ofstream* stream;
	};
	static const LOG INITLOG = {L"", NULL};
	LOG getlog(wchar_t logfile[])
	{
		LOG log;
		log.stream = new std::ofstream;
		log.stream->open(logfile);
		if (!log.stream->is_open())
		{
			throw ERROR_THROW(112);
		}
		wcscpy_s(log.logfile, logfile);
		return log;
	}
	void WriteLine(LOG log, const char* c, ...)
	{
		const char** p = &c;
		while (*p != "")
		{
			*log.stream << *p;
			++p;
		}
		*log.stream << std::endl;
	}
	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		const wchar_t** p = &c;
		while (*p != L"")
		{
			const int SIZE = 10;
			char buff[SIZE];
			size_t tsize = 0;
			wcstombs_s(&tsize, buff, *p, SIZE);
			*log.stream << buff;
			++p;
		}
		*log.stream << std::endl;
	}
	void WriteLog(LOG log)
	{
		time_t t = time(NULL);
		tm buf;
		localtime_s(&buf, &t);
		char str[50];
		strftime(str, 50, "%D %H:%M", &buf);
		printf("--протокол-- Дата: %s --", str);
		*log.stream << "--протокол-- Дата: " << str << " --";
	}
	void WriteParm(LOG log, Parm::PARM parm)
	{
		char buffer[PARM_MAX_SIZE];
		size_t ts = 0;
		wcstombs_s(&ts, buffer, parm.in, PARM_MAX_SIZE);
		std::cout << "\n--параметры--\n-in: " << buffer;
		*log.stream << "\n--параметры--\n-in: " << buffer;
		wcstombs_s(&ts, buffer, parm.out, PARM_MAX_SIZE);
		std::cout << "\n-out: " << buffer;
		*log.stream << "\n-out: " << buffer;
		wcstombs_s(&ts, buffer, parm.log, PARM_MAX_SIZE);
		std::cout << "\n-log: " << buffer << std::endl;
		*log.stream << "\n-log: " << buffer << std::endl;
	}
	void WriteIn(LOG log, In::IN in)
	{
		std::cout << "--исходные данные--\nКоличество символов: " << in.size;
		std::cout << "\nПроигнорировано    : " << in.ignor;
		std::cout << "\nКоличество строк   : " << in.lines << std::endl;
		*log.stream << "--исходные данные--\nКоличество символов: " << in.size << "\nПроигнорировано    : " << in.ignor << "\nКоличество строк   : " << in.lines << std::endl;
	}
	void WriteError(LOG log, Error::ERROR error)
	{
		std::cout << "Ошибка " << error.id << ": ";
		std::cout << error.message << ", строка ";
		std::cout << error.index.line << ", позиция ";
		std::cout << error.index.col << "\n";
		*log.stream << "Ошибка " << error.id << ": " << error.message << ", строка " << error.index.line << ", позиция " << error.index.col << "\n";
	}
	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
	}
};