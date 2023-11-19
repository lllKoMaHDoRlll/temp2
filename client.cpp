#include <iostream> 
#include <string>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)

#define SRV_HOST "localhost"
#define SRV_PORT 1234
#define CLNT_PORT 1235
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
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) < 0) {
        cout << "[ERROR] WSA initialization failed: " << WSAGetLastError() << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else {
        cout << "[INFO] WSA was initialized successfully." << endl;
    }

    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        cout << "[ERROR] Socket initialization failed: " << WSAGetLastError() << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else {
        cout << "[INFO] Socket was initialized successfully." << endl;
    }

    sockaddr_in clntSin, srvSin;

    clntSin.sin_family = AF_INET;
    clntSin.sin_addr.s_addr = 0;
    clntSin.sin_port = htons(CLNT_PORT);

    if (bind(s, (sockaddr*)&clntSin, sizeof(clntSin))) {
        cout << "[ERROR] Socket binding failed: " << WSAGetLastError() << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else {
        cout << "[INFO] Socket was binded successfully." << endl;
    }

    hostent* hp;
    srvSin.sin_family = AF_INET;
    srvSin.sin_port = htons(SRV_PORT);
    hp = gethostbyname(SRV_HOST);
    ((unsigned long*)&srvSin.sin_addr)[0] = ((unsigned long**)hp->h_addr_list)[0][0];

    connect(s, (sockaddr*)&srvSin, sizeof(srvSin));

    int len = 0;
    char buf[BUF_SIZE] = { 0 };
    do {
        len = recv(s, (char*)&buf, BUF_SIZE, 0);
        if (len == SOCKET_ERROR) {
            cout << "[ERROR] Receiving data failed: " << WSAGetLastError() << endl;
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        else {
            cout << "[INFO] Data was received." << endl;
        }

        cout << "SERVER MSG: " << buf << endl;

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
        cout << sizeof(person) << " " << sizeof(Person) << endl;
        send(s, (char*)&msg[0], sizeof(Person), 0);

    } while (true);

    cout << "Conversation is over" << endl;
    closesocket(s);
    return 0;
}
