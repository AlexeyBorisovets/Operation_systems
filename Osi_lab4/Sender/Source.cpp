#include <windows.h>
#include <iostream>
#include <fstream>

#pragma warning(disable : 4996)

using namespace std;

HANDLE FULL;
HANDLE EMPTY;
HANDLE MUTEX;
int messagesNumber;

struct Message {
	char name[20];
	char text[20];

	Message() {
		strcpy(name, "111");
		strcpy(text, "111");
	}

	Message(char* name1, char* text1) {
		strcpy(name, name1);
		strcpy(text, text1);
	}
};

void write(char* filename, char* message, char* authorName) {
	WaitForSingleObject(EMPTY, INFINITE);
	WaitForSingleObject(MUTEX, INFINITE);

	fstream f(filename, ios::binary | ios::in | ios::out);

	int messageWritePos;
	char p[10];
	f.read(p, sizeof(p));
	messageWritePos = atoi(p);
	cout << "Message write pos: " << messageWritePos << endl;

	Message* m = new Message(authorName, message);
	int pos = sizeof(p) + sizeof(Message) * messageWritePos;
	f.seekp(pos, ios::beg);
	f.write((char*)m, sizeof(Message));

	messageWritePos++;
	if (messageWritePos == messagesNumber) {
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
	FULL = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)"Full");
	EMPTY = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)"Empty");
	MUTEX = OpenMutex(MUTEX_ALL_ACCESS, FALSE, (LPCWSTR)"Mutex");
	
	char* filename = argv[1];
	messagesNumber = atoi(argv[2]);
	cout << "Enter name:\n";
	char name[20];
	cin >> name;
	bool doCycle = true;
	while (doCycle) {
		cout << "Enter:\n1) Write\n2) Exit\n";
		int answer;
		cin >> answer;

		if (answer == 1) {
			cout << "Enter message:\n";
			char message[30];
			cin >> message;
			write(filename, message, name);
		}
		else if (answer == 2) {
			doCycle = false;
		}
	}
}