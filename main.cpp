#include "authentication.h"
using namespace std;

int main(){
    Authentication auth;
    if (auth.isLoggedIn()){
        cout<<"1. Change Username\n";
        cout<<"2. Change Password\n";
    }else{
        cout<<"1. Login\n";
        cout<<"2. Register\n";
    }
    cout<<"3. Exit\n";
    int input;
    cout<<"Please select your action: \n";
    while (true){
        cin>>input;
        if (cin.fail()){
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid input! Please only enter 1 number\n";
            continue;
        } else if (input<0||input>3){
            cout<<"Please select the numbers within the range!\n";
            continue;
        }

        if (input==3){
            cout<<"Leaving...\n";
            break;
        }

        if (auth.isLoggedIn()){
            if (input==1){
                auth.changeUsername();
            }else if (input==2){
                auth.changePassword();
            }
        }else {
            if (input==1){
                auth.login();
            }else if (input==2){
                auth.registration();
            }
        }
    }
    return 0;
}