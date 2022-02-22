#include <windows.h>
#include <iostream>
#include <fstream>

#pragma warning(disable : 4996)

using namespace std;

struct employee
{
	int num; // идентификационный номер сотрудника
	char name[10]; // имя сотрудника
	double hours; // количество отработанных часов

};

char* prepareDataForCreatorProcess(char*& fileOrderName)
{

	char data[200] = "Creater ";

	strcat_s(data, fileOrderName);
	strcat_s(data, " ");

	cout << "How many empls:\n";
	int num;
	cin >> num;

	char* tmp = new char[10];

	_itoa(num, tmp, 10);
	strcat(data, tmp);

	char* str = new char[200];
	strcpy(str, data);

	return str;
}

void runCreatorProcess(char* dataForProcess)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFOA));
	si.cb = sizeof(STARTUPINFOA);

	if (!CreateProcessA(NULL, dataForProcess, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "The new process is not created.\n";
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void checkDataInBinaryFile(char* fileName)
{
	ifstream in(fileName, ios::binary);

	cout << "Empls:\n";
	while (in.peek() != EOF)
	{
		employee empl;
		in.read((char*)&empl, sizeof(employee));
		cout << "empl number:\n" << empl.num << "\nempl name:\n" << empl.name << "\nempl hours:\n" << empl.hours << endl;
	}

	in.close();
}

char* prepareDataForReporterProcess(char* fileOrderName, char* fileReportName)
{
	char data[200] = "Reporter ";

	strcat_s(data, fileOrderName);
	strcat_s(data, " ");
	strcat_s(data, fileReportName);
	strcat_s(data, " ");

	cout << "Enter oplata_za_chas:\n";
	int oplata_za_chas;
	cin >> oplata_za_chas;

	char* tmp = new char[10];

	_itoa(oplata_za_chas, tmp, 10);
	strcat(data, tmp);

	char* str = new char[200];
	strcpy(str, data);

	return str;
}

void runReporterProcess(char* data)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFOA));
	si.cb = sizeof(STARTUPINFOA);

	if (!CreateProcessA(NULL, data, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "The new process is not created.\n";
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void checkDataInFile(char* fileName)
{
	ifstream fin(fileName);

	char line[200];
	while (!fin.eof())
	{
		fin.getline(line, 200);

		cout << line << endl;
	}

	fin.close();
}

int main()
{
	char* fileOrderName = new char[30];
	cout << "Enter file name:\n";
	cin >> fileOrderName;
	cout << endl;

	char* dataForCreator = prepareDataForCreatorProcess(fileOrderName);
	cout << dataForCreator << endl;
	runCreatorProcess(dataForCreator);

	checkDataInBinaryFile(fileOrderName);

	char* fileReportName = new char[30];
	cout << "\nEnter file report name:\n";
	cin >> fileReportName;
	cout << endl;

	char* dataForReporter = prepareDataForReporterProcess(fileOrderName, fileReportName);
	runReporterProcess(dataForReporter);

	checkDataInFile(fileReportName);

	system("pause");
	return 0;
}