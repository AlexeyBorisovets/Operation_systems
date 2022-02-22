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
	char* filename = argv[1];
	int num= atoi(argv[2]);

	cout << num;
	ofstream out(filename, ios::binary);

	


	for (int i = 0; i < num; i++)
	{
		system("cls");
		employee empl;
		cout << "Enter empl number:\n";
		cin >> empl.num;

		cout << "Enter empl name:\n";
		cin >> empl.name;

		cout << "Enter empl hours:\n";
		cin >> empl.hours;


		out.write((char*)&empl, sizeof(struct employee));
	}

	out.close();

	system("pause");
	return 1;
}