#ifndef USER_H
#define USER_H
#include <string>

using namespace std;

class User{
    private:
        string username;
        string password;
    public:
        User(string name,string psd):username(name),password(psd){};

        string getUsername() const{
            return username;
        }

        string getPassword() const {
            return password;
        }

        void setUsername(string name){
            username=name;
        }

        void setPassword(string psd){
            password=psd;
        }

};
#endif