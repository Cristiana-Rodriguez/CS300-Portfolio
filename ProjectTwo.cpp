//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Cristiana Rodriguez
// Version     : 1.0
// Description : CS 300 Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Course object stores one course and its prerequisites
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node object for the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course newCourse) {
        course = newCourse;
        left = NULL;
        right = NULL;
    }
};

// Binary Search Tree class
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == NULL) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        } else if (course.courseNumber > node->course.courseNumber) {
            if (node->right == NULL) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) {
        if (node == NULL) {
            return;
        }

        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }

    void deleteTree(Node* node) {
        if (node == NULL) {
            return;
        }

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    BinarySearchTree() {
        root = NULL;
    }

    ~BinarySearchTree() {
        deleteTree(root);
    }

    bool isEmpty() {
        return root == NULL;
    }

    void insert(Course course) {
        if (root == NULL) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    Course search(string courseNumber) {
        Node* current = root;

        while (current != NULL) {
            if (current->course.courseNumber == courseNumber) {
                return current->course;
            } else if (courseNumber < current->course.courseNumber) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        Course emptyCourse;
        emptyCourse.courseNumber = "";
        return emptyCourse;
    }

    void printCourseList() {
        cout << "Here is a sample schedule:" << endl << endl;
        inOrder(root);
    }
};

// Removes extra spaces from the beginning and end of a string
string trim(string text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    size_t end = text.find_last_not_of(" \t\r\n");

    if (start == string::npos || end == string::npos) {
        return "";
    }

    return text.substr(start, end - start + 1);
}

// Converts user input to uppercase so csci400 becomes CSCI400
string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

// Splits one CSV line into separate fields
vector<string> splitLine(string line) {
    vector<string> fields;
    string field;
    stringstream lineStream(line);

    while (getline(lineStream, field, ',')) {
        fields.push_back(trim(field));
    }

    return fields;
}

// Loads course data from the file into the binary search tree
void loadCourses(string fileName, BinarySearchTree& courseTree) {
	ifstream file(fileName.c_str());

    if (!file.is_open()) {
        cout << "Unable to open file: " << fileName << endl;
        return;
    }

    string line;

    while (getline(file, line)) {
        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = splitLine(line);

        if (fields.size() < 2) {
            cout << "Invalid course record found. Skipping line." << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(fields[0]);
        course.courseTitle = fields[1];

        for (unsigned int i = 2; i < fields.size(); i++) {
            course.prerequisites.push_back(toUpperCase(fields[i]));
        }

        courseTree.insert(course);
    }

    file.close();
}

// Prints one course and its prerequisite course numbers
void printCourseInformation(BinarySearchTree& courseTree, string courseNumber) {
    courseNumber = toUpperCase(courseNumber);

    Course course = courseTree.search(courseNumber);

    if (course.courseNumber == "") {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites: ";

        for (unsigned int i = 0; i < course.prerequisites.size(); i++) {
            cout << course.prerequisites[i];

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

// Displays the menu options
void displayMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << endl;
    cout << "What would you like to do? ";
}

int main() {
    BinarySearchTree courseTree;
    int choice = 0;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        displayMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Enter the file name: ";
            getline(cin, fileName);

            loadCourses(fileName, courseTree);
            break;

        case 2:
            if (courseTree.isEmpty()) {
                cout << "Please load the course data first." << endl;
            } else {
                courseTree.printCourseList();
            }
            break;

        case 3:
            if (courseTree.isEmpty()) {
                cout << "Please load the course data first." << endl;
            } else {
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);

                printCourseInformation(courseTree, courseNumber);
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}



