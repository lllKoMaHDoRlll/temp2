#include <iostream>
#include <string>

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <WinSock2.h>
#include <Windows.h>

#pragma warning(disable: 4996)

#define LISTENPORT 666
#define BUF_SIZE 2048

using namespace std;

struct Person {
	char surname[20];
	char name[20];
	char middlename[20];
	char address[30];
	char gender[10];
	char education[20];
	int bdate;
};

struct Message {//структура данных для сообщения
	int type;
	char message[256];
	Person person;
};

int nClients = 0;//начальное число клиентов

void printUsers() {
	if (nClients > 0)
		cout << nClients << " users online" << endl;
	else
		cout << "No users online" << endl;
	cout << endl;
}

// Прототип функции потока, обслуживающей подключившихся пользователей
DWORD WINAPI connectToClient(LPVOID clientSocket) {
	SOCKET cSocket = ((SOCKET*)clientSocket)[0];
	Message msg;
	char greeting[] = "Hello! Server is ready to work \r\n";
	int len = 0;

	// Отправляем сообщение клиенту
	send(cSocket, greeting, sizeof(greeting), 0);

	while ((len = recv(cSocket, (char*)&msg, sizeof(msg), 0)) != SOCKET_ERROR) {
		if (msg.type == 1) {
			cout << "(Client): " << msg.message << endl << endl;
			string ans = "Message received: ";
			ans += msg.message;
			send(cSocket, ans.c_str(), ans.length(), 0);
		}
		if (msg.type == 2) {
			Person y = msg.person;

			if (string(y.surname) == "exit")
				break;

			cout << "Getting info about person " << y.surname << endl;
			string ed = (string)y.education;
			int mnv = 5;

			string ans;
			if (ed == "higher") {
				ans = "This person has higher education";
			}
			else if (ed == "secondary") {
				ans = "This person has secondary education";
			}
			else if (ed == "primary") {
				ans = "This person has primary education";
			}
			else {
				ans = "Incorrect education value!";
			}

			//ans = "Server received Person struct: " + (string)y.surname + " " + (string)y.name + " " + (string)y.middlename + " " + (string)y.address + " " + (string)y.gender + " " + (string)y.education + " " + to_string(y.bdate);
			send(cSocket, ans.c_str(), ans.length(), 0);
		}
	}

	// Клиент вышел
	nClients--;
	cout << "Client left" << endl;
	printUsers();
	closesocket(cSocket);
	return 0;
}

int main() {
	// Оформление окна
	cout << "\t Server\n";
	for (int i = 0; i < 30; i++)
		cout << "-";
	cout << endl;

	// Инициализируем библиотеку сокетов
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws)) {
		cout << "Error WSAStartup! \n" << WSAGetLastError();
		return -1;
	}

	// Создание сокета
	SOCKET sListener;
	if ((sListener = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "Error socket! \n" << WSAGetLastError();
		WSACleanup();
		return -1;
	}

	// Создаем локальный адрес сервера
	sockaddr_in sLocal;
	sLocal.sin_family = AF_INET;
	sLocal.sin_port = htons(LISTENPORT);
	sLocal.sin_addr.s_addr = 0;

	// Связываем сокет с адресом сервера
	if (bind(sListener, (sockaddr*)&sLocal, sizeof(sLocal))) {
		cout << "Error bind! \n" << WSAGetLastError();
		closesocket(sListener);
		WSACleanup();
		return -1;
	}

	// Инициализируем слушающий сокет
	if (listen(sListener, 0x100)) {
		cout << "Error listen! \n" << WSAGetLastError();
		closesocket(sListener);
		WSACleanup();
		return -1;
	}

	cout << "Waiting for connection..." << endl;

	// Создаем сокет и адрес клиента
	SOCKET sClient;		// Сокет клиента
	sockaddr_in adrClient;	// Адрес клиента (заполнится автоматически при вызове)
	char buf[BUF_SIZE + 1] = { 0 };
	int len = sizeof(adrClient);

	// Цикл приема клиентов
	while (true) {
		// Принимаем входящее соединение
		if ((sClient = accept(sListener, (sockaddr*)&adrClient, &len)) == INVALID_SOCKET) {
			cout << "Error accept! \n" << WSAGetLastError();
			closesocket(sListener);
			WSACleanup();
			return -1;
		}

		// Увеличиваем число клиентов онлайн
		nClients++;
		cout << "New connection! \n";

		// Получаем сведения о подключившемся клиенте
		HOSTENT* hst = gethostbyaddr((char*)&adrClient.sin_addr.s_addr, 4, AF_INET);

		// Выводим сведения о подключившемся клиенте и общее число клиентов
		if (hst)
			cout << hst->h_name << ' ';
		cout << inet_ntoa(adrClient.sin_addr) << endl;
		printUsers();

		//Создаем новый поток для обслуживания клиента
		DWORD threadId;
		CreateThread(NULL, NULL, connectToClient, &sClient, NULL, &threadId);
	}

	closesocket(sListener);
	WSACleanup();
	return 0;
}
