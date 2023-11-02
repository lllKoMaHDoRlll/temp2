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

int proceedR(Student& B) {
    if ((B.marks1 == 5) && (B.marks2 == 5) && (B.marks3 == 5) && (B.marks4 == 5)) return 0;
    if ((B.marks1 > 3) && (B.marks2 > 3) && (B.marks3 > 3) && (B.marks4 > 3)) return 1;
    return 2;
}

int main()
{
    ifstream file_input;
    ifstream user_file_input;
    ofstream file_output;
    
    vector<string> usernames;
    vector<int> userfiles_prev_sizes;
    file_input.open(conn_fn);
    string username_;
    while(getline(file_input, username_)) {
        usernames.push_back(username_);
    }
    file_input.close();

    for (string usn : usernames) {
        string fp = "C://Users/aleks/source/repos/Nikita_CW_server/Nikita_CW_server/" + usn + ".bin";
        user_file_input.open(fp);
        user_file_input.seekg(0, ios::end);
        userfiles_prev_sizes.push_back(user_file_input.tellg());
        user_file_input.close();
    }

    for (int size_ : userfiles_prev_sizes) {
        cout << size_ << endl;
    }

    int prev_size;

    file_input.open(conn_fn, ios::binary);
    file_input.seekg(0, ios::end);
    prev_size = file_input.tellg();
    file_input.close();


    while (true) {
        file_input.open(conn_fn);
        file_input.seekg(0, ios::end);
        while (prev_size >= file_input.tellg()) {
            Sleep(5000);
            file_input.seekg(0, ios::end);
            cout << "searching for new conn " <<  prev_size << endl;
            
            int index = 0;
            for (string usn : usernames) {
                cout << index << " " << userfiles_prev_sizes[index] << endl;
                string fp = "C://Users/aleks/source/repos/Nikita_CW_server/Nikita_CW_server/" + usn + ".bin";
                user_file_input.open(fp);
                user_file_input.seekg(0, ios::end);

                if (userfiles_prev_sizes[index] >= user_file_input.tellg()) {
                    user_file_input.close();
                    cout << "no requests found" << endl;
                }
                else {
                    cout << "new request" << endl;
                    user_file_input.seekg(userfiles_prev_sizes[index], ios::beg);
                    user_file_input.read((char*)&st, sizeof(st));
                    user_file_input.close();

                    int answer = proceedR(st);
                    file_output.open(fp, ios::binary | ios::app);
                    file_output.write((char*)&answer, sizeof(answer));
                    userfiles_prev_sizes[index] += sizeof(answer) + sizeof(st);
                    file_output.close();
                }

                index++;
            }

        }
        string username;
        file_input.seekg(prev_size, ios::beg);
        getline(file_input, username);
        usernames.push_back(username);
        userfiles_prev_sizes.push_back(0);
        file_input.seekg(0, ios::end);
        prev_size = file_input.tellg();
        file_input.close();
    }

}
