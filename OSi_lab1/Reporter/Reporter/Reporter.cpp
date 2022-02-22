#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

struct employee
{
	int num; // идентификационный номер сотрудника
	char name[10]; // имя сотрудника
	double hours; // количество отработанных часов

};

int main(int argc, char* argv[])
{
	char* fileOrderName = argv[1];
	char* fileReportName = argv[2];
	int oplata_za_chas = atoi(argv[3]);


	ifstream in(fileOrderName, ios::binary);
	employee* empl = new employee[100];
	int size = 0;
	while (in.peek() != EOF)
	{
		employee emp;
		in.read((char*)&emp, sizeof(employee));

		empl[size++] = emp;

	}
	in.close();



	ofstream out(fileReportName);



	out << "Report about file \"" << fileOrderName << "\":\n";

	if (size != 0)
	{
		for (int i = 0; i < size; i++)
		{
			out << "empl number: " << empl[i].num << ", empl name: " << empl[i].name << ", empl hours: " << empl[i].hours << ",empl money: " << empl[i].hours * oplata_za_chas << endl;
		}
	}
	else
	{
		out << "-empty-\n";
	}

	out.close();

	system("pause");
	return 1;
}