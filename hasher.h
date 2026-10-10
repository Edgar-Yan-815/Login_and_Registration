#ifndef HASHER_H
#define HASHER_H
#include <string>
#include <sodium.h>
using namespace std;

class Hasher{
    public:
        static bool hashPassword(const string& password, string& hashedPassword){
            char hash[crypto_pwhash_STRBYTES];

            if (crypto_pwhash_str_alg(
                hash,
                password.c_str(),
                static_cast<unsigned long long>(password.size()),
                crypto_pwhash_OPSLIMIT_INTERACTIVE,
                crypto_pwhash_MEMLIMIT_INTERACTIVE,
                crypto_pwhash_ALG_ARGON2ID13
            )!=0){
                return false;
            }
            hashedPassword=hash;
            return true;
        }

        static bool verifyPassword(const string& password, const string& hashedPassword){
            return crypto_pwhash_str_verify(
                hashedPassword.c_str(),
                password.c_str(),
                static_cast<unsigned long long>(password.size())
            )==0;
        }
};

#endif