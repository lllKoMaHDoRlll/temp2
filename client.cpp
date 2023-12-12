#include <iostream>
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <WinSock2.h>

#define BUF_SIZE 4096
#define JSONrequest 

using namespace std;

void HTTP_Connection(string host, string request) {
	SOCKET s;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "Error socket! \n";
		return;
	}

	HOSTENT* hn;
	if ((hn = gethostbyname(host.c_str())) == NULL) {
		cout << "Error socket! \n";
		closesocket(s);
		return;
	}

	// Заполняем структуру адреса сервера
	sockaddr_in adr;
	adr.sin_family = AF_INET;
	((unsigned long*)&adr.sin_addr)[0] =
		((unsigned long**)hn->h_addr_list)[0][0];
	adr.sin_port = htons(host == "localhost" ? 8000 : 80);

	// Устанавливаем соединение с сервером
	if (connect(s, (sockaddr*)&adr, sizeof(adr)) == SOCKET_ERROR) {
		cout << "Error connect! \n";
		closesocket(s);
		return;
	}

	// Посылаем запрос
	if (send(s, request.c_str(), request.size(), 0) == SOCKET_ERROR) {
		cout << "Error send! \n";
		closesocket(s);
		return;
	}

	// Получаем ответ
	int len = 0;
	char buf[BUF_SIZE + 1];
	// Получаем все части ответа
	do {
		if ((len = recv(s, (char*)&buf, BUF_SIZE, 0)) == SOCKET_ERROR) {
			cout << "Ошибка send! \n";
			closesocket(s);
			return;
		}
		buf[len] = '\0';
		cout << buf;
	} while (len != 0); // Пока пакеты не закончатся

	// Закрываем соединение
	if (closesocket(s) == SOCKET_ERROR)
		return;
	return;
}

int main() {
	// Оформление окна
	cout << "\t Web-client\n";
	for (int i = 0; i < 30; i++)
		cout << "-";
	cout << endl;
	cout << "Connection type: \n"
		<< "1: Connect to http server \n"
		<< "2: Connect to www.json.org \n";
	for (int i = 0; i < 30; i++)
		cout << "-";
	cout << endl;

	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws)) {
		cerr << "Error WSAStartup! \n" << WSAGetLastError();
		return -1;
	}

	while (true) {
		int type;
		cout << "Enter connection type: ";
		cin >> type;
		if (type == 1) {
			// НТРР-сервер С++
			HTTP_Connection("localhost",
				"GET/ HTTP/1.1\r\n host: localhost \r\n\r\n");
		}
		else {
			// www.json.org
			HTTP_Connection("www.json.org",
				"GET /json-en.html HTTP/1.1\r\nHost: www.json.org\r\nConnection: close\r\n\r\n");
		}
		cout << endl;
	}

	WSACleanup();
	return 0;
}
