#include <iostream>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
using namespace std;

bool signup(string username, string password, int Phoneno, int Age, char Gender, string Interest1, string Interest2)
{
    mongocxx::instance inst{};                      // Initialize the MongoDB driver
    mongocxx::uri uri("mongodb://localhost:27017"); // Create a MongoDB connection URI
    mongocxx::client client(uri);                   // Connect to the MongoDB server

    mongocxx::database db = client["friendfinder-db"]; // Get a handle to the "mydatabase" database
    mongocxx::collection coll = db["users"];           // Get a handle to the "mycollection" collection

    // Create a BSON document representing the row to be inserted
    bsoncxx::builder::stream::document doc{};
    doc << "username" << username << "password" << password << "Phoneno" << Phoneno << "Age" << Age << "Gender" << Gender << "Interest1" << Interest1 << "Interest2" << Interest2;

    // Insert the document into the collection
    mongocxx::result::insert result = coll.insert_one(doc.view());

    // Check if the insertion was successful
    if (result)
    {
        std::cout << "Signed Up Successfully. Please Login." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Signup Failed. Please contact admin." << std::endl;
        return false;
    }
}

bool signin(string username, string password)
{
    mongo::DBClientConnection conn;
    conn.connect("localhost");

    mongo::BSONObj result = conn.findOne("friendfinder-db.users", QUERY("username" << username));

    string pass = result["password"].toString();

    if (password == pass)
    {
    std:
        cout << "Signed In Successfully.";
        return true;
    }

    else
    {
    std:
        cout << "Invalid Login Credentials.";
        return false;
    }
}

void view(string username)
{
    mongo::DBClientConnection conn;
    conn.connect("localhost");

    mongo::BSONObj result = conn.findOne("friendfinder-db.users", QUERY("username" << username));

std:
    cout << "Username : " << result["username"].toString() << std : endl;
std:
    cout << "PhoneNo : " << result["PhoneNo"].toString() << std : endl;
std:
    cout << "Age : " << result["Age"].toString() << std : endl;
std:
    cout << "Gender : " << result["Gender"].toString() << std : endl;
std:
    cout << "Interest1 : " << result["Interest1"].toString() << std : endl;
std:
    cout << "Interest2 : " << result["Interest2"].toString() << std : endl;
}

void update(string username, string password, int Phoneno, int Age, char Gender, string Interest1, string Interest2)
{
    ongocxx::instance instance{};             // This initializes the MongoDB C++ driver.
    mongocxx::client client{mongocxx::uri{}}; // This creates a client instance for connecting to the MongoDB server.

    mongocxx::database db = client["friendfinder-db"];
    mongocxx::collection coll = db["users"];

    mongocxx::options::update options{};
    options.upsert(true); // If the document doesn't exist, create it.

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "key"
                   << "value"; // This is the condition for selecting the document to update.

    bsoncxx::builder::stream::document update_builder{};
    update_builder << "$set" << bsoncxx::builder::stream::open_document
                   << "Phoneno" << Phoneno
                   << "Age" << Age
                   << "Gender" << Gender
                   << "Interest1" << Interest1
                   << "Interest2" << Interest2
                   << bsoncxx::builder::stream::close_document; // This is the new values for the selected document.

    coll.update_one(filter_builder.view(), update_builder.view(), options);
}
