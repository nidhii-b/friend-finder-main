#include <iostream>
#include <string>
#include <functional>
#include <typeinfo>

using namespace std;
string hash_password(const string password)
{
    unsigned int hash_value = 0;
    for (size_t i = 0; i < password.length(); i++)
    {
        hash_value += password[i];
    }
    string hash_string = "";
    char hex_digits[] = "0123456789abcdef";
    while (hash_value > 0)
    {
        int digit = hash_value % 16;
        hash_string = hex_digits[digit] + hash_string;
        hash_value /= 16;
    }
    transform(hash_string.begin(), hash_string.end(), hash_string.begin(), ::tolower);
    return hash_string;
}
int main()
{
    string password;
    cout << "Enter a password to hash: ";
    cin >> password;
    string hashed_password = hash_password(password);
    cout << "Hashed password: " << hashed_password << endl;
    return 0;
}