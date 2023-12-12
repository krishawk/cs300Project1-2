//============================================================================
// Name        : HashTable.cpp
// Author      : Kristen Hawkins
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : CS 300 Project Two
//============================================================================

#include <iostream>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <string>
#include <Windows.h>
#include <vector>
#include <iostream>

using namespace std;

const unsigned int DEFAULT_SIZE = 179;

const int GLOBAL_SLEEP_TIME = 5000;



// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold course information
struct  Course {
    string courseId; // unique identifier
    string courseName;
    string courseList;
    double courseNum;

    vector<string> preList;
};


class HashTable {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        };

        // initialize with a course
        Node(Course aCourse) : Node() {
            course = aCourse;
        };

        // initialize with a bid and a key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        };
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Course course);
    void PrintAll();
    void Remove(string courseId);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
   
    nodes.resize(tableSize);
};



HashTable::HashTable(unsigned int size) {
    this->tableSize = size;
    nodes.resize(tableSize);
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    
 
    // erase nodes beginning
    nodes.erase(nodes.begin());
   
}


unsigned int HashTable::hash(int key) {
    
    // return key tableSize

    return key % tableSize;
}


void HashTable::Insert(Course course) {
    // FIXME (5): Implement logic to insert a bid

    //calcuate the key for this bid
    unsigned key = hash(atoi(course.courseId.c_str()));

    // try and retrieve node using key
    Node* oldNode = &(nodes.at(key));
    // if no entry found for the key
    if (oldNode == nullptr) {
        Node* newNode = new Node(course, key);
        nodes.insert(nodes.begin() + key, (*newNode));
    }
    else {
        //node found
         // else if node is not used
        if (oldNode->key == UINT_MAX) {
            oldNode->key = key;
            oldNode->course = course;
            oldNode->next = nullptr;
        }
        else {
            // else if node is not used

            while (oldNode->next != nullptr) {
                oldNode = oldNode->next;
            }

            // add new newNode to end
            oldNode->next = new Node(course, key);
        }

    }

}

/**
 * Print all bids
 */
void HashTable::PrintAll() {

    for (unsigned int i = 0; i < tableSize; i++) {

        Node* currNode = &nodes.at(i);

        if (currNode->key != UINT_MAX) {

            cout << currNode->key << ": " << currNode->course.courseId << " | " << currNode->course.courseNum << " | " << currNode->course.courseList << endl;

            while (currNode->next != nullptr) {

                    currNode = currNode->next;
                cout << currNode->key << ": " << currNode->course.courseId << " | " << currNode->course.courseNum << " | " << currNode->course.courseList << endl;
            }
        }
    }
    return;
}

/**
 * Remove a bid
 *
 * @param courseId The course id to search for
 */
void HashTable::Remove(string courseId) {
   

    unsigned key = hash(atoi(courseId.c_str()));
    
    // erase node begin and key
    nodes.erase(nodes.begin());

}




/**
 * Display the course information to the console (std::out)
 */
void displayCourses(Course course) {
    cout << course.courseId << ": " << course.courseList << " | " << course.courseNum << " | "
            << course.courseName << endl;
    return;
}


vector<string> Split(string lineFeed) {

    char delim = ',';

    lineFeed += delim; 
    vector<string> lineTokens;
    string temp = "";
    for (int i = 0; i < lineFeed.length(); i++)
    {
        if (lineFeed[i] == delim)
        {
            lineTokens.push_back(temp); //store words in token vector
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}
/**
 * Load a CSV file containing bids into a container
 */
void loadCourses(string csvPath, HashTable* courseList) {
    
    ifstream inFS;
    string line;
    vector<string> stringTokens;

    inFS.open(csvPath); //open and reading file

    if(!inFS.is_open()) {
        cout<< "File Error." <<endl;
        return;
    }
    
    while (!inFS.eof()) {

        Course aCourse;

        getline(inFS, line);
        stringTokens = Split(line);

        if (stringTokens.size() < 2) {

            cout << "\nError. Skipping line." << endl;

        }

        else {

            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {

                aCourse.preList.push_back(stringTokens.at(i));        
                
            };

        //pushing this course back
        courseList-> Insert(aCourse);


    };

};

inFS.close(); 

};

void displayCourse(Course aCourse) {

    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";


//if the list is empty then there are no prere
    if (aCourse.courseList.empty()) { 

        cout << "none" << endl;
    }
    else {

        for (unsigned int i = 0; i < aCourse.courseList.size(); i++) {
            
            cout << aCourse.courseList.at(i);
            
            if (aCourse.courseList.size() > 1 && i < aCourse.courseList.size() - 1) {
                cout << ", ";
            }            
        }
    }

    cout << endl;
}







/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, aCourseKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        aCourseKey = argv[2];
        break;
    default:
        csvPath = "CourseList.csv";
    }



    // Define a hash table to hold all the courses
    HashTable* CourseTable;

    Course course;
    CourseTable = new HashTable();

    bool goodInput;
    int choice = 0;
    
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

          aCourseKey = "";        
        string anyKey = " ";
        choice = 0; 

        try {
            cin >> choice;

            if ((choice > 0 && choice < 5) || (choice == 9)) {
                goodInput = true;
            }
            else {
                goodInput = false;
                throw 1;
            }

            switch (choice) {

            case 1:
                
                // Complete the method call to load the courses
           loadCourses(csvPath, courseList);
                cout << courseList->Size() << " courses read" << endl;
                
                Sleep(GLOBAL_SLEEP_TIME);

                break;
                

            case 2:
                courseList->InOrder();

                cout << "\nEnter \'y\' to continue..." << endl;

                cin >> anyKey;

                break;

            case 3:

                cout << "\nWhat course do you want to know about? " << endl;
                cin >> aCourseKey;

                course = courseList->Search(aCourseKey);

                if (!course.courseId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "\nCourse ID " << aCourseKey << " not found." << endl;
                }

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 4:

                cout << "\nWhat course do you want delete? " << endl;
                cin >> aCourseKey;

                courseList->Remove(aCourseKey);

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 9:
                exit;
                break;

            default:

                throw 2;
            }
        }

        catch (int err) {

            std::cout << "\nPlease check your input." << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }

        cin.clear();
        cin.ignore();        

        system("cls");
    }

    cout << "Good bye." << endl;

    Sleep(GLOBAL_SLEEP_TIME);

    return 0;
}