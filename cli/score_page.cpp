#include <iostream>
#include <vector>
#include <algorithm>
#include <sqlite3.h>
#include <tuple>
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

    bool isAvailable()
    {
        return available;
    }

    bool hasSameGender(User otherUser)
    {
        return gender == otherUser.gender;
    }
    bool getGender()
    {
        return gender;
    }
};

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
    vector<pair<int, string>> scoreList; // vector to store scores and corresponding user name
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
                cout << "\nim in";
                if (iInterests[k] == '1' && jInterests[k] == '1')
                {
                    cout << "\t" << commonInterests;
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

    sort(scoreList.begin(), scoreList.end(), greater<pair<int, string>>()); // sort vector in descending order of score

    for (int i = 0; i < scoreList.size(); i++) // print results
    {
        cout << scoreList[i].second << endl;
    }
}

int main()
{
    // vector<User> userList;
    string iInterest;
    bool iGender;
    int rollnum = 9; // example rollnum

    // cout << userList[0].getUserInterests() << endl;

    return 0;
}

void viewMatches(int rollnum)
{
    vector<User> userList = getUsersFromDatabase();
    scoreUsers(userList, rollnum);
}