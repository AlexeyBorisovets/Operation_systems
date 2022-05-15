#include <Windows.h>
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

	Message(const char* name1, const char* text1) {
		strcpy(name, name1);
		strcpy(text, text1);
	}
};

HANDLE FULL, EMPTY, MUTEX;
int position_of_read_mass = 0;
int num_of_massges;

void file_creating(char* filename, int messagesNumber) {
	ofstream fout(filename, ios::binary);
	int pos = 0;
	char p[10];
	itoa(pos, p, 10);
	fout.write(p, sizeof(p));
	Message* m = new Message("empty", "empty");
	for (int i = 0; i < messagesNumber; i++) {
		fout.write((char*)&m, sizeof(Message));
	}
	fout.close();
}

void read(char* filename) {
	WaitForSingleObject(FULL, INFINITE);
	WaitForSingleObject(MUTEX, INFINITE);
	cout << "Позиция для чтения: " << position_of_read_mass << endl;

	fstream f(filename, ios::binary | ios::in | ios::out);
	if (!f.is_open()) {
		cout << "error\n";
		return;
	}

	Message* m = new Message();
	char writeIter[10];
	int pos = sizeof(writeIter) + sizeof(Message) * position_of_read_mass;

	f.seekg(pos, ios::beg);
	f.read((char*)m, sizeof(Message));
	cout << "Сообщение от автора: " << m->name << ", сообщение: " << m->text << endl;

	f.seekp(pos, ios::beg);
	m = new Message("deleted", "deleted");
	f.write((char*)m, sizeof(Message));

	position_of_read_mass++;
	if (position_of_read_mass == num_of_massges) {
		position_of_read_mass = 0;
	}

	f.close();

	ReleaseMutex(MUTEX);
	ReleaseSemaphore(EMPTY, 1, NULL);
}

void main() {
	setlocale(LC_ALL, "Russian");

	cout << "Введите название файла:\n";
	char filename[20];
	cin >> filename;

	cout << "Введите номер сообщения:\n";
	cin >> num_of_massges;
	file_creating(filename, num_of_massges);

	cout << "Введите число отправителей: \n";
	int senderProcessCount;
	cin >> senderProcessCount;

	FULL = CreateSemaphore(NULL, 0, num_of_massges, (LPCWSTR)"Full");
	EMPTY = CreateSemaphore(NULL, num_of_massges, num_of_massges, (LPCWSTR)"Empty");
	MUTEX = CreateMutex(NULL, FALSE, (LPCWSTR)"Mutex");

	STARTUPINFO* si = new STARTUPINFO[senderProcessCount];
	PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[senderProcessCount];

	char data[110] = "Sender ";
	strcat(data, filename);
	strcat(data, " ");
	char num[10];
	itoa(num_of_massges, num, 10);
	strcat(data, num);
	wchar_t wtext[100];
	mbstowcs(wtext, data, strlen(data) + 1);
	LPWSTR ptr = wtext;
	for (int i = 0; i < senderProcessCount; i++) {
		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);

		if (!CreateProcess(NULL, ptr, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i]))
		{
			cout << "The new process is not created.\n";
			return;
		}
	}

	system("cls");
	bool doCycle = true;
	while (doCycle) {
		cout << "Выберите:\n1) Читать\n2) Выход\n";
		int answer;
		cin >> answer;

		if (answer == 1) {
			read(filename);
		}
		else if (answer == 2) {
			doCycle = false;
		}
	}

}