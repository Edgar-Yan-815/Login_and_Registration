#ifndef USER_H
#define USER_H
#include <string>

using namespace std;

class User{
    private:
        string username;
        string hashedPassword;
    public:
        User(string name,string psd):username(name),hashedPassword(psd){};

        string getUsername() const{
            return username;
        }

        string gethashedPassword() const {
            return hashedPassword;
        }

        void setUsername(string name){
            username=name;
        }

        void sethashedPassword(string psd){
            hashedPassword=psd;
        }

};
#endif