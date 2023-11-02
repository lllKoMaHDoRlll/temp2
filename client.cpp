#include <fstream>
#include <string>
#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

const char* conn_fn = "C://Users/aleks/source/repos/Nikita_CW_server/Nikita_CW_server/conn.txt";


struct Student {
    char name[25];
    int marks1;
    int marks2;
    int marks3;
    int marks4;
} st;

int main()
{
    ifstream file_input;
    ofstream file_output;


    string username;
    cout << "Enter username: ";
    cin >> username;


    file_input.open(conn_fn);
    string username_;
    bool is_already_exist = false;
    while (getline(file_input, username_)) {
        if (username == username_) {
            is_already_exist = true;
            break;
        }
    }
    file_input.close();

    cout << "Input student's name: ";
    cin >> st.name;
    cout << "Input student's mark1: ";
    cin >> st.marks1;
    cout << "Input student's mark2: ";
    cin >> st.marks2;
    cout << "Input student's mark3: ";
    cin >> st.marks3;
    cout << "Input student's mark4: ";
    cin >> st.marks4;


    string fp = "C://Users/aleks/source/repos/Nikita_CW_server/Nikita_CW_server/" + username + ".bin";
    if (!is_already_exist) {
        file_output.open(conn_fn, ios::app);
        file_output << username << endl;
        file_output.close();
    }

    file_output.open(fp, ios::app | ios::binary);
    file_output.write((char*)&st, sizeof(st));
    file_output.close();


    file_input.open(fp, ios::binary);
    file_input.seekg(0, ios::end);
    int prev_size = file_input.tellg();
    file_input.close();


    while (true) {
        file_input.open(fp, ios::binary);
        file_input.seekg(0, ios::end);
        if (prev_size >= file_input.tellg()) {
            file_input.seekg(0, ios::end);
            file_input.close();
            Sleep(1000);
        }
        else {
            file_input.close();
            break;
        }
    }
    int answer;
    file_input.open(fp, ios::binary);
    file_input.seekg(prev_size, ios::beg);
    file_input.read((char*)&answer, sizeof(answer));

    prev_size = file_input.tellg();
    file_input.close();

    if (answer == 0) {
        cout << "Student has increased scholarship." << endl;
    }
    else if (answer == 1){
        cout << "Student has common scholarship." << endl;
    }
    else if (answer == 2) {
        cout << "Student has no scholarship." << endl;
    }
    else {
        cout << "Invalid result" << endl;
    }
}
