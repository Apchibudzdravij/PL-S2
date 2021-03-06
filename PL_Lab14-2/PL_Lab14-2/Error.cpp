#include "stdafx.h"
#include "Error.h"
namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(113, "Запрашивается несуществующая строка таблицы лексем"),
		ERROR_ENTRY(114, "Ошибка при открытии файла с промежуточным кодом (-out)"),
		ERROR_ENTRY(115, "Во входящем файле неясная последовательность символов"),
		ERROR_ENTRY(116, "Слишком много лексем"),
		ERROR_ENTRY(117, "Запрашивается несуществующая строка таблицы идентификаторов"),
		ERROR_ENTRY(118, "Идентификатор имеет длину больше 5 символов"),
		ERROR_ENTRY(119, "Слишком много идентификаторов"),
		ERROR_ENTRY(120, "Запрашивается несуществующий литерал"),
		ERROR_ENTRY(121, "Слишком много литералов"),
		ERROR_ENTRY_NODEF(122), ERROR_ENTRY_NODEF(123), ERROR_ENTRY_NODEF(124), ERROR_ENTRY_NODEF(125),
		ERROR_ENTRY_NODEF(126), ERROR_ENTRY_NODEF(127), ERROR_ENTRY_NODEF(128), ERROR_ENTRY_NODEF(129),
		ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY_NODEF100(600), ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900),
	};
	ERROR geterror(int id)
	{
		if ((id > 0) && (id < ERROR_MAX_ENTRY))
		{
			ERROR err = {errors[id].id};
			for (int i = 0; i < ERROR_MAXSIZE_MESSAGE; i++)
				err.message[i] = errors[id].message[i];
			return err;
		}
		else
		{
			ERROR err = {0};
			for (int i = 0; i < ERROR_MAXSIZE_MESSAGE; i++)
				err.message[i] = errors[0].message[i];
			return err;
		}
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if ((id > 0) && (id < ERROR_MAX_ENTRY))
		{
			ERROR err = { errors[id].id};
			for (int i = 0; i < ERROR_MAXSIZE_MESSAGE; i++)
				err.message[i] = errors[id].message[i];
			err.index.line = line;
			err.index.col = col;
			throw err; 
			return err;
		}
		else
		{
			ERROR err = { 0 };
			for (int i = 0; i < ERROR_MAXSIZE_MESSAGE; i++)
				err.message[i] = errors[0].message[i];
			return err;
		}
	}
}