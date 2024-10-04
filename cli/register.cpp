#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <bitset>
// #include <openssl/sha.h>
// install from https://www.openssl.org/source/
//  #include <openssl/evp.h>

using namespace std;

// unsigned int simple_hash(const std::string& str) {
//     unsigned int hash = 5381;
//     for (char c : str) {
//         hash = ((hash << 5) + hash) + c;
//     }
//     return hash;
// }

string getInterest()
{
    string hobies[] = {"basketball", "volleyball", "football", "swimming", "running", "cycling", "cricket", "chess", "Music", "Singing", "AI & machine learning", "web & app development", "computer programming", "Blockchain", "Video Games", "Traveling and exploring new places", "painting", "drawing", "photography", "graphic design", "Watching movies", "Watching series"};
    int num_hobies = sizeof(hobies) / sizeof(hobies[0]);
    int choices[22] = {0};
    int c;

    // to clear the terminal screen
#ifdef _WIN32
    system("cls");
#elif _WIN64
    system("cls");
#else
    system("clear");
#endif

    for (int i = 0; i < num_hobies; i++)
    {
        cout << "\n"
             << i + 1 << "." << hobies[i];
    }

    cout << "\nEnter your Choices(one by one - Press 0. to exit):\n";
    do
    {
        cout << "\nEnter choice num(0 to exit):";
        cin >> c;
        if (c > 0 && c <= num_hobies)
        {
            choices[c - 1] = 1;
        }
        else if (c == 0)
        {
            break;
        }
        else
        {
            cout << "\nenter valid choice";
        }
        //
    } while (c != 0);
    string result = "";
    for (int i = 0; i < num_hobies; i++)
    {
        char k = choices[i] == 1 ? ('1') : ('0');
        result = result + k;
    }
    return result;
}

int insertStudentData(const string name, bool gender, int rollno, const string dept,
                      const string email, const string password, const string interest, bool available)
{

    sqlite3 *db;
    int rc = sqlite3_open("student.db", &db); // student.db is the name of the database file

    if (rc)
    {
        sqlite3_close(db);
        return 0; // Failed to open database
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS Students (Name TEXT, Gender INT, RollNo INT PRIMARY KEY, Dept TEXT, Email TEXT, Password TEXT, Interest TEXT, Available INT)";
    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return 0; // Failed to create table
    }

    sql = "INSERT INTO Students (Name, Gender, RollNo, Dept, Email, Password, Interest, Available) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return 0; // Failed to prepare statement
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, gender ? 1 : 0);
    sqlite3_bind_int(stmt, 3, rollno);
    sqlite3_bind_text(stmt, 4, dept.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, interest.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, available ? 1 : 0);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0; // Failed to execute statement
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1; // Success
}

bool isRollNoExists(int rollno)
{
    sqlite3 *db;
    int rc = sqlite3_open("student.db", &db); // student.db is the name of the database file

    if (rc)
    {
        sqlite3_close(db);
        return false; // Failed to open database
    }

    const char *sql = "SELECT COUNT(*) FROM Students WHERE RollNo = ?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return false; // Failed to prepare statement
    }

    sqlite3_bind_int(stmt, 1, rollno);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false; // Failed to execute statement
    }

    int count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return (count > 0);
}

int getNewUserData()
{

    cout << "*********************************************************";
    cout << "\n\n\t\tSIGN UP";
    string name;
    bool gender;
    int rollno;
    string dept;
    string email;
    int phoneNum;
    string interest = "";
    bool available = true;

    cout << "\nName: ";
    cin >> name;

    // Input validation for gender (allow only 0 or 1)
    cout << "\nGender (1 for male, 0 for female): ";
    while (true)
    {
        if (!(cin >> gender))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter 1 for male or 0 for female: ";
        }
        else if (gender != 0 && gender != 1)
        {
            cout << "Invalid input. Enter 1 for male or 0 for female: ";
        }
        else
        {
            break;
        }
    }

    // Input validation for rollno and phoneNum
    cout << "\nRollNO: ";
    while (!(cin >> rollno))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a valid integer: ";
    }
    if (isRollNoExists(rollno))
    {
        cout << "User with same rollnum exists";
        return 0;
    }

    cout << "\nDept: ";
    cin >> dept;

    cout << "\nEmail: ";
    cin >> email;

    string password, password_confirmation;

    do
    {
        cout << "\nPlease enter your password: ";
        cin >> password;

        cout << "\nPlease confirm your password: ";
        cin >> password_confirmation;

        if (password != password_confirmation)
        {
            cout << "\tPasswords do not match. Please try again." << endl;
        }
    } while (password != password_confirmation);

    cout << "Password confirmed!" << endl;

    cout << "*********************************************************";

#ifdef _WIN32 // to clear the terminal screen
    system("cls");
#else
    system("clear");
#endif

    interest = getInterest(); // getting a bit stream of interest
    if (insertStudentData(name, gender, rollno, dept, email, password, interest, available) != 0)
    {
        cout << "\nsuccessfully inserted";
        return rollno;
    }
    else
    {
        cout << "\nFailed to insert";
        return 0;
    }
}
int main()
{
    
}