#include <iostream> 
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <WinSock2.h>
#include <Windows.h>

#pragma warning(disable: 4996)

#define PORT 666
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
		cout << "WSA init error \n" << WSAGetLastError();
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "Socket creating error \n" << WSAGetLastError();
		return -1;
	}

	sockaddr_in sAddr;

	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = INADDR_ANY;
	sAddr.sin_port = htons(PORT);


	
	if (bind(s, (sockaddr*)&sAddr, sizeof(sAddr))) {
		cout << "Socket binding error \n" << WSAGetLastError();
		return -1;
	}

	

	char buf[BUF_SIZE] = { 0 };
	int k = 1;
	while (true) {
		sockaddr_in from;
		int len = sizeof(from);
		int bsz = recvfrom(s, &buf[0], BUF_SIZE - 1, 0, (sockaddr*)&from, &len);
		if (bsz == SOCKET_ERROR) {
			cout << "Message receiving error \n" << WSAGetLastError();
			return -1;
		}

		HOSTENT* hst;
		hst = gethostbyaddr((char*)&from.sin_addr, 4, AF_INET);
		cout << "New Datagram: \n" << ((hst) ? hst->h_name : "Unknown host") << endl
			<< inet_ntoa(from.sin_addr) << endl << ntohs(from.sin_port) << endl;

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
		
		cout << "Datagram: " << c_person.surname << " " << c_person.name << " " << c_person.middlename << " " << c_person.address << " " << c_person.gender << " " << c_person.education << " " << c_person.bdate << endl;

		string msg = "Datagramm " + to_string(k) + " is received.";
		k++;
		sendto(s, (char*)&msg[0], msg.size(), 0, (sockaddr*)&from, sizeof(from));
	}
	return 0;
}
