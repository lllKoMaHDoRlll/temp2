#include <iostream>
#include <string>

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <WinSock2.h>
#include <Windows.h>
#pragma warning(disable: 4996)

#define BUF_SIZE 2048

#define SRV_PORT 666
#define SRV_ADDR "127.0.0.1"

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


int main() {
	// Оформление окна
	cout << "\t Client\n";
	cout << "Server connection type: " << endl;
	cout << "1: echo" << endl;
	cout << "2: Person" << endl;
	cout << "Command to exit: exit \n";
	for (int i = 0; i < 30; i++)
		cout << "-";
	cout << endl;

	int type;
	cout << "Server connection type: ";
	cin >> type;
	string str;
	getline(cin, str); // Считываем перевод строки

	// Инициализируем библиотеку сокетов
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws)) {
		cout << "Error WSAStartup! \n" << WSAGetLastError();
		return -1;
	}

	// Создание сокета
	SOCKET s;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "Error socket! \n" << WSAGetLastError();
		WSACleanup();
		return -1;
	}

	// Указание адреса и порта сервера
	sockaddr_in srvAddr;
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(SRV_PORT);

	HOSTENT* hst;

	// Преобразование IP-адреса из символьного в сетевой формат
	if (inet_addr(SRV_ADDR) != INADDR_NONE)
		srvAddr.sin_addr.s_addr = inet_addr(SRV_ADDR);
	else {
		// Попытка получить IP-адрес по доменному имени
		if (hst = gethostbyname(SRV_ADDR))
			((unsigned long*)&srvAddr.sin_addr)[0] =
			((unsigned long**)hst->h_addr_list)[0][0];
		else {
			cout << "Invalid web address! \n" << WSAGetLastError();
			closesocket(s);
			WSACleanup();
			return -1;
		}
	}

	// Установление соединения
	if (connect(s, (sockaddr*)&srvAddr, sizeof(srvAddr))) {
		cout << "Error connect! \n" << WSAGetLastError();
		closesocket(s);
		WSACleanup();
		return -1;
	}

	int len = 0;
	char buf[BUF_SIZE + 1] = { 0 };
	// Цикл получения сообщений
	while (true) {
		len = recv(s, &buf[0], BUF_SIZE, 0);
		if (len == SOCKET_ERROR) {
			cout << "Error recv! \n" << WSAGetLastError();
			closesocket(s);
			WSACleanup();
			return -1;
		}

		// Вывод сообщения от сервера
		buf[len] = '\0';
		cout << "(Server): " << buf << endl;


		Message msg;
		msg.type = type;

		if (type == 1) {
			cout << "Enter message: " << endl;
			// Ввод сообщения пользователя
			cin.getline(msg.message, 255);
			if (string(msg.message) == "exit") {
				// Отправка сообщения
				send(s, (char*)&msg, sizeof(msg), 0);
				cout << "Exit... \n";
				break;
			}
		}
		if (type == 2) {
			cout << "Enter surname: ";
			cin.getline(msg.person.surname, 20);
			if (string(msg.person.surname) == "exit") {
				// Отправка сообщения
				send(s, (char*)&msg, sizeof(msg), 0);
				cout << "Exit... \n";
				break;
			}
			cout << "Enter name: ";
			cin.getline(msg.person.name, 20);
			cout << "Enter middle name: ";
			cin.getline(msg.person.middlename, 20);
			cout << "Enter address: ";
			cin.getline(msg.person.address, 30);
			cout << "Enter gender: ";
			cin.getline(msg.person.gender, 10);
			cout << "Enter education (higher | secondary | primary): ";
			cin.getline(msg.person.education, 20);
			cout << "Enter birthday date: ";
			cin >> msg.person.bdate;

			getline(cin, str); // Перевод строки
		}

		// Отправка сообщения
		send(s, (char*)&msg, sizeof(msg), 0);

	}

	closesocket(s);
	WSACleanup();
	return 0;
}
