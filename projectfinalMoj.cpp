#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
using namespace std;

struct Course {
    string courseName;
    int credits;
    float grade;
};

struct Student {
    string firstName;
    string lastName;
    string studentID;
    string major;
    vector<Course> courses;

    float calculateGPA() const {
        float totalPoints = 0;
        int totalCredits = 0;
        for (const auto& course : courses) {
            totalPoints += course.grade * course.credits;
            totalCredits += course.credits;
        }
        return (totalCredits == 0) ? 0 : totalPoints / totalCredits;
    }
};

vector<Student> students;

// Function prototypes
void registerStudent();
void addCourseToStudent();
void listStudents();
void listStudentsByMajor();
void generateReportCard();
void editStudent();
void deleteStudent();
void editCourse();
void deleteCourse();
void saveDataToFile();
void loadDataFromFile();
bool isValidNumber(const string& str);

int main() {
    loadDataFromFile();
    int choice;
    do {
        cout << "\n--- Student Management System ---\n";
        cout << "1. Register Student\n";
        cout << "2. Add Course to Student\n";
        cout << "3. List All Students\n";
        cout << "4. List Students by Major\n";
        cout << "5. Generate Report Card\n";
        cout << "6. Edit Student\n";
        cout << "7. Delete Student\n";
        cout << "8. Edit Course\n";
        cout << "9. Delete Course\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1: registerStudent(); break;
            case 2: addCourseToStudent(); break;
            case 3: listStudents(); break;
            case 4: listStudentsByMajor(); break;
            case 5: generateReportCard(); break;
            case 6: editStudent(); break;
            case 7: deleteStudent(); break;
            case 8: editCourse(); break;
            case 9: deleteCourse(); break;
            case 0: saveDataToFile(); cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Please try again.\n"; break;
        }
    } while (choice != 0);

    return 0;
}

void registerStudent() {
    Student student;
    cout << "Enter first name: ";
    getline(cin, student.firstName);
    cout << "Enter last name: ";
    getline(cin, student.lastName);
    cout << "Enter student ID: ";
    getline(cin, student.studentID);
    cout << "Enter major: ";
    getline(cin, student.major);

    students.push_back(student);
    cout << "Student registered successfully!\n";
}

void addCourseToStudent() {
    string id;
    cout << "Enter student ID to add courses: ";
    getline(cin, id);

    auto it = find_if(students.begin(), students.end(), [&id](const Student& s) {
        return s.studentID == id;
    });

    if (it != students.end()) {
        Course course;
        cout << "Enter course name: ";
        getline(cin, course.courseName);

        string creditsStr, gradeStr;
        do {
            cout << "Enter course credits: ";
            getline(cin, creditsStr);
        } while (!isValidNumber(creditsStr));
        course.credits = stoi(creditsStr);

        do {
            cout << "Enter course grade (0-20): ";
            getline(cin, gradeStr);
        } while (!isValidNumber(gradeStr) || stof(gradeStr) < 0 || stof(gradeStr) > 20);
        course.grade = stof(gradeStr);

        it->courses.push_back(course);
        cout << "Course added successfully!\n";
    } else {
        cout << "Student not found!\n";
    }
}

void listStudents() {
    if (students.empty()) {
        cout << "No students registered yet.\n";
        return;
    }

    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.calculateGPA() > b.calculateGPA();
    });

    cout << left << setw(20) << "Name" << setw(15) << "Student ID" << setw(15) << "Major" << setw(10) << "GPA" << endl;
    cout << string(60, '-') << endl;

    for (const auto& student : students) {
        cout << left << setw(20) << (student.firstName + " " + student.lastName)
             << setw(15) << student.studentID
             << setw(15) << student.major
             << setw(10) << fixed << setprecision(2) << student.calculateGPA() << endl;
    }
}

void listStudentsByMajor() {
    string major;
    cout << "Enter major to filter: ";
    getline(cin, major);

    vector<Student> filteredStudents;
    copy_if(students.begin(), students.end(), back_inserter(filteredStudents), [&major](const Student& s) {
        return s.major == major;
    });

    if (filteredStudents.empty()) {
        cout << "No students found in this major.\n";
        return;
    }

    sort(filteredStudents.begin(), filteredStudents.end(), [](const Student& a, const Student& b) {
        return a.calculateGPA() > b.calculateGPA();
    });

    cout << left << setw(20) << "Name" << setw(15) << "Student ID" << setw(15) << "Major" << setw(10) << "GPA" << endl;
    cout << string(60, '-') << endl;

    for (const auto& student : filteredStudents) {
        cout << left << setw(20) << (student.firstName + " " + student.lastName)
             << setw(15) << student.studentID
             << setw(15) << student.major
             << setw(10) << fixed << setprecision(2) << student.calculateGPA() << endl;
    }
}

void generateReportCard() {
    string id;
    cout << "Enter student ID to generate report card: ";
    getline(cin, id);

    auto it = find_if(students.begin(), students.end(), [&id](const Student& s) {
        return s.studentID == id;
    });

    if (it != students.end()) {
        cout << "\n--- Report Card ---\n";
        cout << "Name: " << it->firstName << " " << it->lastName << endl;
        cout << "Student ID: " << it->studentID << endl;
        cout << "Major: " << it->major << endl;
        cout << "GPA: " << fixed << setprecision(2) << it->calculateGPA() << "\n";

        cout << left << setw(30) << "Course Name" << setw(10) << "Credits" << setw(10) << "Grade" << endl;
        cout << string(50, '-') << endl;
        for (const auto& course : it->courses) {
            cout << left << setw(30) << course.courseName
                 << setw(10) << course.credits
                 << setw(10) << course.grade << endl;
        }
    } else {
        cout << "Student not found!\n";
    }
}

void editStudent() {
    string id;
    cout << "Enter student ID to edit: ";
    getline(cin, id);

    auto it = find_if(students.begin(), students.end(), [&id](const Student& s) {
        return s.studentID == id;
    });

    if (it != students.end()) {
        cout << "Editing student " << it->firstName << " " << it->lastName << endl;
        cout << "Enter new first name: ";
        getline(cin, it->firstName);
        cout << "Enter new last name: ";
        getline(cin, it->lastName);
        cout << "Enter new major: ";
        getline(cin, it->major);
        cout << "Student updated successfully!\n";
    } else {
        cout << "Student not found!\n";
    }
}

void deleteStudent() {
    string id;
    cout << "Enter student ID to delete: ";
    getline(cin, id);

    auto it = find_if(students.begin(), students.end(), [&id](const Student& s) {
        return s.studentID == id;
    });

    if (it != students.end()) {
        students.erase(it);
        cout << "Student deleted successfully!\n";
    } else {
        cout << "Student not found!\n";
    }
}

void editCourse() {
    string studentID, courseName;
    cout << "Enter student ID to edit course: ";
    getline(cin, studentID);
    
    auto studentIt = find_if(students.begin(), students.end(), [&studentID](const Student& s) {
        return s.studentID == studentID;
    });

    if (studentIt != students.end()) {
        cout << "Enter course name to edit: ";
        getline(cin, courseName);

        auto courseIt = find_if(studentIt->courses.begin(), studentIt->courses.end(), [&courseName](const Course& c) {
            return c.courseName == courseName;
        });

        if (courseIt != studentIt->courses.end()) {
            cout << "Editing course " << courseIt->courseName << endl;
            cout << "Enter new course name: ";
            getline(cin, courseIt->courseName);

            string creditsStr, gradeStr;
            do {
                cout << "Enter new course credits: ";
                getline(cin, creditsStr);
            } while (!isValidNumber(creditsStr));
            courseIt->credits = stoi(creditsStr);

            do {
                cout << "Enter new course grade (0-20): ";
                getline(cin, gradeStr);
            } while (!isValidNumber(gradeStr) || stof(gradeStr) < 0 || stof(gradeStr) > 20);
            courseIt->grade = stof(gradeStr);

            cout << "Course updated successfully!\n";
        } else {
            cout << "Course not found!\n";
        }
    } else {
        cout << "Student not found!\n";
    }
}

void deleteCourse() {
    string studentID, courseName;
    cout << "Enter student ID to delete course: ";
    getline(cin, studentID);

    auto studentIt = find_if(students.begin(), students.end(), [&studentID](const Student& s) {
        return s.studentID == studentID;
    });

    if (studentIt != students.end()) {
        cout << "Enter course name to delete: ";
        getline(cin, courseName);

        auto courseIt = find_if(studentIt->courses.begin(), studentIt->courses.end(), [&courseName](const Course& c) {
            return c.courseName == courseName;
        });

        if (courseIt != studentIt->courses.end()) {
            studentIt->courses.erase(courseIt);
            cout << "Course deleted successfully!\n";
        } else {
            cout << "Course not found!\n";
        }
    } else {
        cout << "Student not found!\n";
    }
}

void saveDataToFile() {
    ofstream file("students.dat", ios::binary);
    if (file.is_open()) {
        for (const auto& student : students) {
            file.write(reinterpret_cast<const char*>(&student), sizeof(student));
        }
        cout << "Data saved to file successfully.\n";
    } else {
        cout << "Failed to open file for saving.\n";
    }
}

void loadDataFromFile() {
    ifstream file("students.dat", ios::binary);
    if (file.is_open()) {
        Student student;
        while (file.read(reinterpret_cast<char*>(&student), sizeof(student))) {
            students.push_back(student);
        }
    }
}

bool isValidNumber(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

