#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cctype>
#include "user.h"
using namespace std;

class Authentication{
    private:
        vector<User> users;
        bool loginSuccessful;
        void loadUsers(){
            users.clear();
            string user;
            ifstream MyFile("Database.txt");
            if (!MyFile.is_open()){
                cout<<"Failed to open Database.txt"<<endl;
                return;
            }
            while (getline(MyFile,user)){
                string username,password;
                stringstream ss(user);
                if (getline(ss,username,',')&&getline(ss,password)&&!username.empty()&&!password.empty()){
                    users.push_back(User(username,password));
                }
            }
        }
    public:
        Authentication(){
            loginSuccessful=false;
            loadUsers();
        };

        bool isLoggedIn() const{
            return loginSuccessful;
        }

        bool validateName(string username){
            for (char c:username){
                if (!isalpha(static_cast<unsigned char>(c))&&!isdigit(static_cast<unsigned char>(c))&&c!='_'){
                    return false;
                }
            }
            return true;
        }

        bool validatePassword(string password){
            bool digit=false;
            bool alpha=false;
            for (char c:password){
                if (isdigit(static_cast<unsigned char>(c))){
                    digit=true;
                }else if (isalpha(static_cast<unsigned char>(c))){
                    alpha=true;
                }
            }
            return password.length()>=8&&digit&&alpha;
        }

        bool validateLogin(string username, string password){
            for (const User& user:users){
                if (user.getUsername()==username&&user.getPassword()==password){
                    return true;
                }
            }
            return false;
        }

        bool findUser(string username){
            for (const User& user:users){
                if (user.getUsername()==username){
                    return true;
                }
            }
            return false;
        };

        void storeUser(string username, string password){
            users.push_back(User(username,password));
            ofstream MyFile("Database.txt",ios::app);
            if (!MyFile.is_open()){
                cout<<"Failed to open Database.txt"<<endl;
                return;
            }
            MyFile<<username<<","<<password<<'\n';
            MyFile.close();
        };

        void login(){
            while (true){
                cout<<"Username:"<<endl;
                string username;
                cin>>username;
                cout<<"Password:"<<endl;
                string password;
                cin>>password;
                bool res=validateLogin(username,password);
                if (res){
                    cout<<"Login successful"<<endl;
                    loginSuccessful=true;
                    return;
                }else{
                    cout<<"Username or Password incorrect"<<endl;
                }
            }
            
        }

        void registration(){
            cout<<"Please enter your username:"<<endl;
            string username;
            while (true){
                cin>>username;
                bool exist=findUser(username);
                if (exist){
                    cout<<"User exist!"<<endl;
                }else if (!validateName(username)){
                    cout<<"Invalid name!"<<endl;
                }else{
                    break;
                }
            }
            cout<<"Your password should include:\n";
            cout<<"at least 1 digit\n";
            cout<<"at least 1 alphabet\n";
            cout<<"at least 8 characters\n";
            cout<<"Please enter your password:"<<endl;
            string password;
            while (true){
                cin>>password;
                bool validate=validatePassword(password);
                if (validate){
                    break;
                }
                cout<<"Please follow these constrains: "<<endl;
                cout<<"atleast 1 digit\n";
                cout<<"atleast 1 alphabet\n";
                cout<<"atleast 8 characters\n";
            }
            storeUser(username,password);
            cout<<"Registration successful"<<endl;
        }
};
#endif