#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

bool authenticate(int rollno, const string &password)
{
    sqlite3 *db;
    int rc = sqlite3_open("student.db", &db); // student.db is the name of the database file

    if (rc)
    {
        sqlite3_close(db);
        return false; // Failed to open database
    }

    const char *sql = "SELECT Password FROM Students WHERE RollNo = ?";
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

    const unsigned char *db_password = sqlite3_column_text(stmt, 0);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return (password == string((const char *)db_password));
}

void login()
{
    int rollno;
    string password;

    cout << "Roll No: ";
    cin >> rollno;

    cout << "Password: ";
    cin >> password;

    if (authenticate(rollno, password))
    {
        cout << "Login successful!" << endl;
    }
    else
    {
        cout << "Login failed." << endl;
    }
}

int main()
{
    return 0;
}
