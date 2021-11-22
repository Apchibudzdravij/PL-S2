#pragma once

#define LIT_MAXLEN 255
#define LIT_ERRORLIT ENTRY errlitt = {errlit.literal = "Err_NoPos"; errlit.strNumInLT = -1; errlit.posNumInLT = -1;};

namespace LitT
{
	struct Entry
	{
		char literal[LIT_MAXLEN];	//��������� ���������� ��������
		int actualSize;
		int strNumInLT;				//��������� ������ � ��
		int posNumInLT;				//��������� ������� � ������ � ��
	};

	struct LitTable
	{
		int maxsize;				//������� ������� ���������
		int size;					//������� ������ ������� ���������
		Entry* table;
	};

	LitTable Create(int size);					//������� �������, ���� �������
	void Add(LitTable& lextable, Entry entry);	//�������� ������ � �������
	Entry GetEntry(LitTable& littable, int str, int pos);	//�������� ������ ������� �� ������ ������ �� � ������� � ���
	void Delete(LitTable& littable);			//������� �������
}