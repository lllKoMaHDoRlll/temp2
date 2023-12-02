#include <iostream> 
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <WinSock2.h>
#include <Windows.h>

#pragma warning(disable: 4996)

#define PORT 666
#define SRV_ADDR "127.0.0.1"
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

int main() {
	char buff[1024];
	if (WSAStartup(0x0202, (WSADATA*)&buff[0])) {
		cout << "WSA Init Error \n" << WSAGetLastError();
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "Socket creating error \n" << WSAGetLastError();
		return -1;
	}


	HOSTENT* hst;
	sockaddr_in srvAddr;
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(PORT);

	if (inet_addr(SRV_ADDR)) {
		srvAddr.sin_addr.s_addr = inet_addr(SRV_ADDR);
	}
	else {
		if (hst = gethostbyname(SRV_ADDR)) {
			srvAddr.sin_addr.s_addr = ((unsigned long**)hst->h_addr_list)[0][0];
		}
		else {
			cout << "Unknown Host: " << WSAGetLastError() << "\n";
			closesocket(s);
			WSACleanup();
			return -1;
		}
	}

	while (true) {

		string surname, name, middlename, address, gender, education;
		int bdate;

		Person person;

		cout << "Enter surname: ";
		cin >> surname;
		strcpy(person.surname, surname.c_str());
		cout << "Enter name: ";
		cin >> name;
		strcpy(person.name, name.c_str());
		cout << "Enter middlename: ";
		cin >> middlename;
		strcpy(person.middlename, middlename.c_str());
		cout << "Enter address: ";
		cin >> address;
		strcpy(person.address, address.c_str());
		cout << "Enter gender (man | woman): ";
		cin >> gender;
		strcpy(person.gender, gender.c_str());
		cout << "Enter education (higher | secondary | primary): ";
		cin >> education;
		strcpy(person.education, education.c_str());
		cout << "Enter birthday date: ";
		cin >> bdate;
		person.bdate = bdate;

		char* msg = (char*)&person;

		sendto(s, (char*)&msg[0], sizeof(person), 0, (sockaddr*)&srvAddr, sizeof(srvAddr));

		sockaddr_in ansAddr;
		char buf[BUF_SIZE] = { 0 };
		int len = sizeof(ansAddr);
		int bsz = recvfrom(s, buf, sizeof(buf), 0, (sockaddr*)&ansAddr, &len);
		if (bsz == SOCKET_ERROR) {
			cout << "Message receiving error \n" << WSAGetLastError() << "\n";
			closesocket(s);
			WSACleanup();
			return -1;
		}
		buf[bsz] = '\0';
		cout << "Answer: " << buf << endl;
	}
	closesocket(s);
	WSACleanup();
	return 0;
}
