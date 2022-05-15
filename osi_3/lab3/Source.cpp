#include <Windows.h>
#include <iostream>
#include <ctime>

using namespace std;


class CircleQueue
{
private:
	int* queue;
	int size;
	int position_of_pushing;
	int position_of_deleting;
	HANDLE full;
	HANDLE empty;
	CRITICAL_SECTION crit_section;

public:
	CircleQueue(int size)
	{
		this->position_of_pushing = 0;
		this->position_of_deleting = 0;
		this->size = size;
		this->queue = new int[size];

		full = CreateSemaphore(NULL, 0, size, NULL);
		empty = CreateSemaphore(NULL, size, size, NULL);
		InitializeCriticalSection(&crit_section);
	}

	void vstavka(int element)
	{
		WaitForSingleObject(empty, INFINITE);
		EnterCriticalSection(&crit_section);
		queue[position_of_pushing++] = element;
		if (position_of_pushing == size)
		{
			position_of_pushing = 0;
		}
		LeaveCriticalSection(&crit_section);
		ReleaseSemaphore(full, 1, NULL);
	}

	~CircleQueue()
	{
		DeleteCriticalSection(&crit_section);
		CloseHandle(empty);
		CloseHandle(full);
		delete[]queue;
	}

	int deleting()
	{
		WaitForSingleObject(full, INFINITE);
		EnterCriticalSection(&crit_section);
		int element = queue[position_of_deleting++];
		if (position_of_deleting == size)
		{
			position_of_deleting = 0;
		}
		LeaveCriticalSection(&crit_section);

		ReleaseSemaphore(empty, 1, NULL);

		return element;
	}
};

HANDLE start;

struct Creating_info_for_produser
{
	CircleQueue* queue;
	int number;
	int count;
};

struct Creating_info_for_consumer
{
	CircleQueue* queue;
	int count;
};

DWORD WINAPI producer(LPVOID par)
{
	WaitForSingleObject(start, INFINITE);

	Creating_info_for_produser* data = (Creating_info_for_produser*)par;


	for (int i = 0; i < data->count; i++)
	{
		data->queue->vstavka(data->number * i);
		cout << "Поток " << data->number << " положил в очередь число : "<< data->number *  i << "\n";
		Sleep(7);
	}

	return 0;
}

DWORD WINAPI consumer(LPVOID par)
{
	WaitForSingleObject(start, INFINITE);

	Creating_info_for_consumer* data = (Creating_info_for_consumer*)par;

	for (int i = 0; i < data->count; i++)
	{
		int number = data->queue->deleting();
		cout << "\t\tЧисло \"" << number << "\" было удалено из очереди.\n";
	}

	return 0;
}

void main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Внесите размер очереди:\n";
	int queueSize;
	cin >> queueSize;

	start = CreateEvent(NULL, TRUE, FALSE, NULL);

	CircleQueue* queue = new CircleQueue(queueSize);

	cout << "Внесите количество Producers :\n";
	int prodThreadCount;
	cin >> prodThreadCount;

	Creating_info_for_produser* workWithProductCountProducer = new Creating_info_for_produser[prodThreadCount];
	for (int i = 0; i < prodThreadCount; i++)
	{
		cout << "Внесите количество чисел для Produce в " << i + 1 << " потоке:\n";
		int count;
		cin >> count;
		workWithProductCountProducer[i].count = count;
		workWithProductCountProducer[i].number = i + 1;
		workWithProductCountProducer[i].queue = *&queue;
	}

	HANDLE* prodThreadHandls = new HANDLE[prodThreadCount];
	DWORD* prodThreadID = new DWORD[prodThreadCount];

	for (int i = 0; i < prodThreadCount; i++)
	{
		prodThreadHandls[i] = CreateThread(NULL, 0, producer, (void*)&workWithProductCountProducer[i], 0, &prodThreadID[i]);
	}


	cout << "Внесите количество Consumers:\n";
	int consThreadCount;
	cin >> consThreadCount;

	Creating_info_for_consumer* workWithProductCountConsumer = new Creating_info_for_consumer[consThreadCount];
	for (int i = 0; i < consThreadCount; i++)
	{
		cout << "Внесите количество чисел для consume в " << i + 1 << " потоке:\n";
		int count;
		cin >> count;
		workWithProductCountConsumer[i].count = count;
		workWithProductCountConsumer[i].queue = *&queue;
	}

	HANDLE* consThreadHandls = new HANDLE[consThreadCount];
	DWORD* consThreadID = new DWORD[consThreadCount];

	for (int i = 0; i < consThreadCount; i++)
	{
		consThreadHandls[i] = CreateThread(NULL, 0, consumer, (void*)&workWithProductCountConsumer[i], 0, &consThreadID[i]);
	}

	HANDLE* handlesArray = new HANDLE[prodThreadCount + consThreadCount];

	for (int i = 0; i < prodThreadCount; i++)
	{
		handlesArray[i] = prodThreadHandls[i];
	}

	for (int i = 0; i < consThreadCount; i++)
	{
		handlesArray[i + prodThreadCount] = consThreadHandls[i];
	}

	SetEvent(start);

	WaitForMultipleObjects(prodThreadCount + consThreadCount, handlesArray, TRUE, INFINITE);
	system("pause");
}