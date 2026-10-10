#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cctype>
#include "hasher.h"
#include "user.h"
using namespace std;

class Authentication{
    private:
        vector<User> users;
        bool loginSuccessful;
        string currentUsername;
        void loadUsers(){
            users.clear();
            string user;
            ifstream MyFile("Database.txt");
            if (!MyFile.is_open()){
                cout<<"Failed to open Database.txt"<<endl;
                return;
            }
            while (getline(MyFile,user)){
                string username,hashedPassword;
                stringstream ss(user);
                if (getline(ss,username,',')&&getline(ss,hashedPassword)&&!username.empty()&&!hashedPassword.empty()){
                    users.push_back(User(username,hashedPassword));
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
                if (user.getUsername()==username){
                    return Hasher::verifyPassword(password,user.gethashedPassword());
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

        bool saveUser(){
            ofstream MyFile("Database.txt");
            if (!MyFile.is_open()){
                cout<<"Unable to open database!\n";
                return false;
            }

            for (const User&user:users){
                MyFile<<user.getUsername()<<","<<user.gethashedPassword()<<endl;
            }

            if (!MyFile){
                cout<<"Failed to save database!\n";
                return false;
            }
            MyFile.close();
            return true;
        }
        bool storeUser(string username, string password){
            string hashedPassword;

            if (!Hasher::hashPassword(password,hashedPassword)){
                cerr<<"Failed to hash password.\n";
                return false;
            }

            users.push_back(User(username,hashedPassword));
            ofstream MyFile("Database.txt",ios::app);
            if (!MyFile.is_open()){
                cerr<<"Failed to open Database.txt"<<endl;
                return false;
            }
            MyFile<<username<<","<<password<<'\n';

            if (!MyFile){
                cerr<<"Failed to save account\n";
                return false;
            }

            MyFile.close();
            return true;
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
                    currentUsername=username;
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
            if (storeUser(username,password)){
                currentUsername=username;
                cout<<"Registration successful"<<endl;
            }     
        }

        bool changePassword(){
            string current;
            cout<<"Please enter your current Password:\n";
            cin>>current;
            if (!validateLogin(currentUsername,current)){
                cout<<"Incorrect current password!\n";
                return false;
            }
            string newPassword;
            while (true){
                cout<<"Please enter the new Password:\n";
                cin>>newPassword;
                if (!validatePassword(newPassword)){
                    cout<<"Password not met requirements.\n";
                    cout<<"atleast 1 digit\n";
                    cout<<"atleast 1 alphabet\n";
                    cout<<"atleast 8 characters\n";
                    cout<<"Please re-enter the new Password:\n";
                    continue;
                }
                string temp;
                cout<<"Please re-enter the new Password:\n";
                cin>>temp;
                if (newPassword!=temp){
                    cout<<"Passwords do not match!\n";
                    continue;
                }
                string hashedPassword;
                if (!Hasher::hashPassword(newPassword,hashedPassword)){
                    cerr<<"Failed to hash password!\n";
                    return false;
                }

                for (User& user:users){
                    if (user.getUsername()==currentUsername){
                        string oldHash=user.gethashedPassword();
                        user.sethashedPassword(hashedPassword);
                        if (saveUser()){
                            cout<<"Password changed successfully!\n";
                            return true;
                        }else{
                            user.sethashedPassword(oldHash);
                            cout<<"Password change failed!\n";
                            return false;
                        }
                        break;
                    }
                }
                   
            }
        }

        bool changeUsername(){
            cout<<"Your current username: "<<currentUsername<<endl;
            string newUsername;
            cout<<"Please enter your new Username:\n";
            while (true){
                cin>>newUsername;
                if (!validateName(newUsername)){
                    cout<<"Invalid Username! Please only input letters, digits and _\n";
                    continue;
                }

                if (findUser(newUsername)){
                    cout<<"Username already exist!\n";
                    continue;
                }
                cout<<"Are you sure you want to change Username[Y/N]?\n";
 
                char input;
                while (true){
                    cin>>input;
                    if (toupper(input)=='Y'||toupper(input)=='N'){
                        break;
                    }
                    cout<<"Incorrect input! Please only enter Y or N\n";
                }

                input=toupper(static_cast<unsigned char>(input));
                if (input=='N'){
                    return false;
                }  

                for (User& user:users){
                    if (user.getUsername()==currentUsername){
                        string oldName=currentUsername;
                        user.setUsername(newUsername);
                        if (saveUser()){
                            currentUsername=newUsername;
                            cout<<"Username changed successfully!\n";
                            return true;
                        }else{
                            user.setUsername(oldName);
                            cout<<"Username change could not be saved.\n";
                            return false;
                        }

                    }

                }
                
            }
            
        }
};
#endif