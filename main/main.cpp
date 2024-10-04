#include <iostream>
#include <string>
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <bitset>
#include <unistd.h>
#include <fstream>
#include<vector>
#include<limits>
#include<cstring>

// #include <openssl/sha.h>
// install from https://www.openssl.org/source/
//  #include <openssl/evp.h>
using namespace std;

class User
{
private:
    string name;
    bool gender;
    int rollno;
    string dept;
    string email;
    string interests;
    int score;
    bool available;

public:
    User(string name, bool gender, int rollno, string dept, string email, string interests, int score, bool available)
        : name(name), gender(gender), rollno(rollno), dept(dept), email(email), interests(interests), score(score), available(available) {}

    void addScore(int points)
    {
        score += points;
    }

    int getScore()
    {
        return score;
    }

    string getUserInterests()
    {
        return interests;
    }

    string getName()
    {
        return name;
    }

    string getDept()
    {
        return dept;
    }

    int getRollno()
    {
        return rollno;
    }

    string getEmail()
    {
        return email;
    }

    bool isAvailable() // public member function to access the 'available' variable
    {
        return available;
    }

    bool hasSameGender(User otherUser) // public member function to access the 'gender' variable
    {
        return gender == otherUser.gender;
    }
    bool getGender()
    {
        return gender;
    }
};

string hobies[] = {"basketball", "volleyball", "football", "swimming", "running", "cycling", "cricket", "chess", "Music", "Singing", "AI & machine learning", "web & app development", "computer programming", "Blockchain", "Video Games", "Traveling and exploring new places", "painting", "drawing", "photography", "graphic design", "Watching movies", "Watching series"};
bool authenticate(int rollno, const string password);
int login();
void dashboard(int rollno);

void myAccount(int rno);
string getInterest();
int getNewUserData();
void initialScreen();

int insertStudentData(const string name, bool gender, int rollno, const string dept,
                      const string email, const string password, const string interest, bool available);
bool isRollNoExists(int rollno);
void updateInterest(int rollnum, string interests);
string hash_password(string password);
void scoreUsers(vector<User> userList, int roll);
vector<User> getUsersFromDatabase();
pair<string, bool> getUserInfoFromDatabase(int rollno);
void viewMatches(int rollnum);
void updateAvailability(int rno, bool availability);
//
//
//
//
//
//
bool authenticate(int rollno, const string password)
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
    string stored_password((const char *)db_password);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return (password == stored_password);
}

int login()
{
#ifdef _WIN32
    system("cls");
#elif _WIN64
    system("cls");
#else
    system("clear");
#endif
    int rollno;
    string password;
    cout << "*****************************\n\t\tLOGIN\n\n";
    cout << "Roll No: ";
    cin >> rollno;

    cout << "Password: ";
    cin >> password;
    password = hash_password(password);
    if (isRollNoExists(rollno))
    {
        if (authenticate(rollno, password))
        {
            cout << "Login successful!" << endl;
            sleep(1);
            return rollno;
        }
        else
        {
            cout << "Please Check Your Password again." << endl;
            sleep(2);
            return 0;
        }
    }
    else
    {
        cout << "\n\nUser doesnt seem to be on our database..plz check again";
        sleep(2);
        return 0;
    }
    return rollno;
}

string hash_password(string password)
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
void updateInterest(int rollnum, string interests)
{
    sqlite3 *db;
    int rc = sqlite3_open("student.db", &db); // student.db is the name of the database file

    if (rc)
    {
        sqlite3_close(db);
        cout << "Failed to open database" << endl;
        return;
    }

    const char *sql = "UPDATE Students SET Interest = ? WHERE RollNo = ?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        cout << "Failed to prepare statement" << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, interests.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, rollnum);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cout << "Failed to execute statement" << endl;
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    cout << "Interests updated successfully" << endl;
    sleep(1);
}

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
    password = hash_password(password);
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
        sleep(1);
        return rollno;
    }
    else
    {
        cout << "\nFailed to insert";
        sleep(1);
        return 0;
    }
}

void initialScreen()
{
#ifdef _WIN32
    system("cls");
#elif _WIN64
    system("cls");
#else
    system("clear");
#endif
    cout << "**********************\n\tROOMMATE FINDER\n";
    cout << "\n1.login\n2.register\n0.Exit\n:";
    int c;
    cin >> c;
    int rno;
    if (c == 1)
    {
        rno = login();
        if (rno == 0)
        {
            initialScreen();
        }
    }
    else if (c == 2)
    {
        rno = getNewUserData();
    }
    else
    {
        cout << "\n***Thanks for using our software***";
        exit(0);
    }

    if (rno != 0)
    {
        dashboard(rno);
    }
}

void updateAvailability(int rno, bool availability)
{
    sqlite3 *db;
    int rc = sqlite3_open("student.db", &db); // student.db is the name of the database file

    if (rc)
    {
        sqlite3_close(db);
        cout << "Failed to open database" << endl;
        return;
    }

    const char *sql = "UPDATE Students SET Available = ? WHERE RollNo = ?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        cout << "Failed to prepare statement" << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, availability ? 1 : 0);
    sqlite3_bind_int(stmt, 2, rno);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cout << "Failed to execute statement" << endl;
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    cout << "Availability updated successfully!" << endl;
}


void myAccount(int rno)
{
    string hobbies[] = {"basketball", "volleyball", "football", "swimming", "running", "cycling", "cricket", "chess", "Music", "Singing", "AI & machine learning", "web & app development", "computer programming", "Blockchain", "Video Games", "Traveling and exploring new places", "painting", "drawing", "photography", "graphic design", "Watching movies", "Watching series"};

    sqlite3 *db;
    int rc = sqlite3_open("student.db", &db); // student.db is the name of the database file

    if (rc)
    {
        sqlite3_close(db);
        cout << "Failed to open database" << endl;
        sleep(1);
        return;
    }

    const char *sql = "SELECT Name, Gender, RollNo, Dept, Email, Interest, Available FROM Students WHERE RollNo = ?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        cout << "Failed to prepare statement" << endl;
        sleep(1);
        return;
    }

    sqlite3_bind_int(stmt, 1, rno);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cout << "Failed to execute statement" << endl;
        sleep(1);
        return;
    }

    string name = string((const char *)sqlite3_column_text(stmt, 0));
    bool gender = (sqlite3_column_int(stmt, 1) == 1) ? true : false;
    int rollno = sqlite3_column_int(stmt, 2);
    string dept = string((const char *)sqlite3_column_text(stmt, 3));
    string email = string((const char *)sqlite3_column_text(stmt, 4));
    string interests = string((const char *)sqlite3_column_text(stmt, 5));
    int score = 0;
    bool available = (sqlite3_column_int(stmt, 6) == 1) ? true : false;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
#ifdef _WIN32
    system("cls");
#elif _WIN64
    system("cls");
#else
    system("clear");
#endif
    cout << "\t\tMyAccount";
    cout << "Name: " << name << endl;
    cout << "Gender: " << (gender ? "Male" : "Female") << endl;
    cout << "Roll No: " << rollno << endl;
    cout << "Department: " << dept << endl;
    cout << "Email: " << email << endl;
    cout << "Interests: ";
    for (int i = 0; i < interests.length(); i++)
    {
        if (interests[i] == '1')
        {
            cout << hobbies[i] << ", ";
        }
    }
    cout << endl;

    cout << "Availability: " << (available ? "Yes" : "No") << endl;

    cout << "\n\npress 1 edit interest\nPress 2 to Change Availability\npress 0 to exit:\n$";
    string a;
    std::cin>>a;
    if (a == "1")
    {
        string newInterest = getInterest();
        updateInterest(rno, newInterest);
        cout << "updated";
        sleep(1);
        dashboard(rno);
    }
    else if(a == "2"){
        int t;
        cout<<"\n1.Available\n2.Not Available:\n$";
        cin>>t;
        bool k = t==1?(true):(false);
        updateAvailability(rno, k);
        sleep(1);
        dashboard(rno);
    }
    else
    {
        dashboard(rno);
    }
}
void dashboard(int rollno)
{
#ifdef _WIN32
    system("cls");
#elif _WIN64
    system("cls");
#else
    system("clear");
#endif
    cout << "\tDASHBOARD \n\n1.My Account\n2.View match\n3.Signout\n$";
    int choice;
    cin >> choice;
    while(choice<1 && choice>3){
        cout<<"\nPlease enter valid option:";
        cin>>choice;
    }
    if (choice == 1)
    {
        myAccount(rollno);
    }
    else if (choice == 2)
    {
        viewMatches(rollno);
    }
    else if (choice == 3)
    {
        initialScreen();
    }

}
void viewMatches(int rollnum)
{
    vector<User> userList = getUsersFromDatabase();
    scoreUsers(userList, rollnum);
    string p;
    
}
pair<string, bool> getUserInfoFromDatabase(int rollno)
{
    string interests;
    bool gender;

    // connect to database
    sqlite3 *db;
    if (sqlite3_open("student.db", &db) == SQLITE_OK)
    {
        // build query to retrieve user details
        string query = "SELECT gender, interest FROM Students WHERE rollno = " + to_string(rollno);

        // execute query and retrieve results
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                // retrieve values from database row
                gender = sqlite3_column_int(stmt, 0);
                const char *interestsChars = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
                interests = interestsChars ? interestsChars : "";
                interests.resize(strlen(interestsChars));
            }
            else
            {
                // handle error if user not found
                cout << "User with roll number " << rollno << " not found in database." << endl;
            }
        }
        else
        {
            // handle error if query execution fails
            cout << "Error executing query: " << sqlite3_errmsg(db) << endl;
        }

        // finalize statement and close database
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    else
    {
        // handle error if database connection fails
        cout << "Error connecting to database: " << sqlite3_errmsg(db) << endl;
    }

    return make_pair(interests, gender);
}

vector<User> getUsersFromDatabase()
{
    vector<User> userList;

    // connect to database
    sqlite3 *db;
    if (sqlite3_open("student.db", &db) == SQLITE_OK)
    {
        // build query to retrieve user details
        string query = "SELECT name, gender, rollno, dept, email, interest, available FROM Students";

        // execute query and retrieve results
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                // retrieve values from database row
                string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                int gender = sqlite3_column_int(stmt, 1);
                int rollno = sqlite3_column_int(stmt, 2);
                string dept = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
                string email = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
                string interests = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
                int available = sqlite3_column_int(stmt, 6);

                // create User object with score = 0 and add to vector
                User user(name, gender, rollno, dept, email, interests, 0, available);
                userList.push_back(user);
            }
        }
        else
        {
            // handle error if query execution fails
            cout << "Error executing query: " << sqlite3_errmsg(db) << endl;
        }

        // finalize statement and close database
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    else
    {
        // handle error if database connection fails
        cout << "Error connecting to database: " << sqlite3_errmsg(db) << endl;
    }

    return userList;
}

// function to get the interest,gender of a user from database by giing rollnum
void scoreUsers(vector<User> userList, int roll)
{
    vector<pair<int, string> > scoreList; // vector to store scores and corresponding user name
    string iInterests;
    int iGender;
    // Get user info for the specified roll number
    pair<string, int> user_info = getUserInfoFromDatabase(roll);
    iInterests = get<0>(user_info);
    iGender = get<1>(user_info);

    for (int j = 0; j < userList.size(); j++) // compare with all other users after i
    {

        if (iGender == userList[j].getGender() && userList[j].isAvailable())
        { // check if users have same gender and are available
            int commonInterests = 0;
            string jInterests = userList[j].getUserInterests();
            for (int k = 0; k < 22; k++) // count number of common interests
            {
                ;
                if (iInterests[k] == '1' && jInterests[k] == '1')
                {
                    
                    commonInterests = commonInterests + 1;
                }
            }
            if (commonInterests > 0) // if common interests are found, add score to both users and add the other user's name to the vector
            {
                userList[j].addScore(commonInterests);
                scoreList.push_back(make_pair(commonInterests, userList[j].getName() + " (" + userList[j].getEmail() + ", " + to_string(userList[j].getScore()) + ")")); // add other user's name, email, and score to vector
            }
        }
    }

    sort(scoreList.begin(), scoreList.end(), greater<pair<int, string> >()); // sort vector in descending order of score
    ofstream outFile("matches.txt");
    if (scoreList.size() == 1)
    {
        cout << "Sorry!....no matches\nreturning to dashboard in 3..2..1";
        sleep(3);
        dashboard(roll);
    }

#ifdef _WIN32
    system("cls");
#elif _WIN64
    system("cls");
#else
    system("clear");
#endif
    for (int i = 1; i < scoreList.size(); i++) // print results
    {
        cout << scoreList[i].second << endl;
    }
    if (outFile.is_open())
    {
        for (int i = 1; i < scoreList.size(); i++)
        {
            outFile << scoreList[i].second << endl;
        }
        outFile.close();
        cout << "Results written to file: matches.txt" << endl;
    }

    
    cout<<"\nPress 1 to go back";
    string p;
    cin>>p;
    if(p=="1"){
        dashboard(roll);
    }
    else{
         dashboard(roll);
    }
}
int main()
{

    initialScreen();

    return 0;
}
