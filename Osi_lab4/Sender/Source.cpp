#include <windows.h>
#include <iostream>
#include <fstream>

#pragma warning(disable : 4996)

using namespace std;

struct Message {
	char name[20];
	char text[20];

	Message() {
		strcpy(name, "default");
		strcpy(text, "default");
	}

	Message(char* name1, char* text1) {
		strcpy(name, name1);
		strcpy(text, text1);
	}
};

HANDLE FULL;
HANDLE EMPTY;
HANDLE MUTEX;
int num_of_massges;

void record(char* filename, char* message, char* name_of_founder) {
	WaitForSingleObject(EMPTY, INFINITE);
	WaitForSingleObject(MUTEX, INFINITE);

	fstream f(filename, ios::binary | ios::in | ios::out);

	int messageWritePos;
	char p[10];
	f.read(p, sizeof(p));
	messageWritePos = atoi(p);
	cout << "Позиция записи сообщения: " << messageWritePos << endl;

	Message* m = new Message(name_of_founder, message);
	int pos = sizeof(p) + sizeof(Message) * messageWritePos;
	f.seekp(pos, ios::beg);
	f.write((char*)m, sizeof(Message));

	messageWritePos++;
	if (messageWritePos == num_of_massges) {
		messageWritePos = 0;
	}
	itoa(messageWritePos, p, 10);
	f.seekp(0, ios::beg);
	f.write(p, sizeof(p));

	f.close();

	ReleaseMutex(MUTEX);
	ReleaseSemaphore(FULL, 1, NULL);
}

int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "Russian");

	FULL = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)"Full");
	EMPTY = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)"Empty");
	MUTEX = OpenMutex(MUTEX_ALL_ACCESS, FALSE, (LPCWSTR)"Mutex");
	
	char* filename = argv[1];
	num_of_massges = atoi(argv[2]);
	cout << "Внесите имя :\n";
	char name[20];
	cin >> name;
	bool doCycle = true;
	while (doCycle) {
		cout << "Выберите:\n1) Запись\n2) Выход\n";
		int answer;
		cin >> answer;

		if (answer == 1) {
			cout << "Введите сообщение :\n";
			char message[30];
			cin >> message;
			record(filename, message, name);
		}
		else if (answer == 2) {
			doCycle = false;
		}
	}
}