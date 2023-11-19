#include <iostream>  
#include <winsock2.h> 
#include <windows.h> 
#include <string> 
#include <vector>

#define SRV_PORT 1234
#define BUF_SIZE 1024

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

const string greeting = "It's Server. Hello!";
vector<Person> persons;

int main() {

	char buff[1024];
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) < 0) {
		cout << "[ERROR] WSA initialization failed: " << WSAGetLastError() << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "[INFO] WSA was initialized successfully." << endl;
	}

	SOCKET sListener, sNew;
	sockaddr_in sin, clntSin;

	sListener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListener == INVALID_SOCKET) {
		cout << "[ERROR] Socket initialization failed: " << WSAGetLastError() << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "[INFO] Socket was initialized successfully." << endl;
	}


	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(SRV_PORT);

	if (bind(sListener, (sockaddr*)&sin, sizeof(sin))) {
		cout << "[ERROR] Socket binding failed: " << WSAGetLastError() << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "[INFO] Socket was binded successfully." << endl;
	}

	int len;
	char buf[BUF_SIZE] = { 0 };
	string msg;
	listen(sListener, 3);
	while (true) {
		len = sizeof(clntSin);
		sNew = accept(sListener, (sockaddr*)&clntSin, &len);
		cout << "[INFO] New connection. " << endl;
		msg = greeting;
		while (true) {
			send(sNew, (char*)&msg[0], msg.size(), 0);
			len = recv(sNew, (char*)buf, BUF_SIZE, 0);
			if (len == SOCKET_ERROR) {
				cout << "[ERROR] Data receiving failed: " << WSAGetLastError() << endl;
				break;
			}

			Person* y = (Person*)(&buf[0]);

			cout << "CLIENT: ";
			cout << (*y).surname << " " << (*y).name << " " << (*y).middlename << " " << (*y).address << " " << (*y).gender << " " << (*y).education << " " << (*y).bdate << endl;
			Person c_person;
			strcpy_s(c_person.surname, (*y).surname);
			strcpy_s(c_person.name, (*y).name);
			strcpy_s(c_person.middlename, (*y).middlename);
			strcpy_s(c_person.address, (*y).address);
			strcpy_s(c_person.gender, (*y).gender);
			strcpy_s(c_person.education, (*y).education);
			c_person.bdate = (*y).bdate;

			persons.push_back(c_person);
		}

		cout << "[INFO] Client disconnected." << endl;
		closesocket(sNew);


		cout << "People with higher education: " << endl;
		for (const Person person : persons) {
			
			if ((string)person.education == "higher") cout << person.surname << " " << person.name << " " << person.middlename << " " << person.address << " " << person.gender << " " << person.education << " " << person.bdate << endl;
		}

		cout << endl << "Womans: " << endl;
		for (const Person person : persons) {
			if ((string)person.gender == "woman") cout << person.surname << " " << person.name << " " << person.middlename << " " << person.address << " " << person.gender << " " << person.education << " " << person.bdate << endl;
		}

	}
	return 0;
}
