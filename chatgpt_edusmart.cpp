#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

enum Role { ADMIN, INSTRUCTOR, STUDENT, GUEST };

struct User {
    string username, password, fullname;
    Role role;
};

struct Course {
    string code, title, instructor;
    vector<string> materials;
    vector<string> assignments;
};

struct Submission {
    string student, assignment, content;
    double grade = -1; // -1 = not graded
    string feedback;
};

vector<User> users;
vector<Course> courses;
vector<Submission> submissions;

// Utility: current time string
string now() {
    time_t t = time(nullptr);
    string s = ctime(&t);
    if(!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

// Seed demo data
void seedData() {
    users.push_back({"admin1","adminpass","Alice Admin",ADMIN});
    users.push_back({"ins_john","teach123","John Doe",INSTRUCTOR});
    users.push_back({"stu_tom","stud123","Tom Lee",STUDENT});
    users.push_back({"guest","", "Guest", GUEST});
    courses.push_back({"IT101","Intro to Programming","ins_john"});
}

// --- Admin Menu ---
void adminMenu(User &u) {
    while(true) {
        cout << "\n[ADMIN MENU]\n";
        cout << "1. List Users\n2. List Courses\n3. Logout\nChoice: ";
        int c; cin >> c;
        if(c==1) {
            cout << "\n-- Users --\n";
            for(auto &x: users)
                cout << x.fullname << " (" << x.username << ")\n";
        } else if(c==2) {
            cout << "\n-- Courses --\n";
            for(auto &c: courses)
                cout << c.code << " - " << c.title << " (Instructor: " << c.instructor << ")\n";
        } else if(c==3) {
            cout << "Logging out...\n"; break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

// --- Instructor Menu ---
void instructorMenu(User &u) {
    while(true) {
        cout << "\n[INSTRUCTOR MENU]\n";
        cout << "1. Add Material\n2. Add Assignment\n3. Grade Submission\n4. Logout\nChoice: ";
        int c; cin >> c; cin.ignore();
        if(c==1) {
            string mat; 
            cout << "Material title (or 0 to cancel): "; 
            getline(cin, mat);
            if(mat == "0" || mat.empty()) {
                cout << "Cancelled.\n";
            } else {
                courses[0].materials.push_back(mat);
                cout << "Uploaded.\n";
            }
        } else if(c==2) {
            string asg; 
            cout << "Assignment title (or 0 to cancel): "; 
            getline(cin, asg);
            if(asg == "0" || asg.empty()) {
                cout << "Cancelled.\n";
            } else {
                courses[0].assignments.push_back(asg);
                cout << "Assignment created.\n";
            }
        } else if(c==3) {
            vector<int> pending;
            for (int i = 0; i < (int)submissions.size(); ++i) {
                if (submissions[i].grade < 0) pending.push_back(i);
            }
            if (pending.empty()) {
                cout << "No pending submissions to grade.\n";
                continue;
            }
            cout << "\n-- Pending submissions --\n";
            for (int i = 0; i < (int)pending.size(); ++i) {
                int idx = pending[i];
                cout << i+1 << ") Student: " << submissions[idx].student
                     << " | Assignment: " << submissions[idx].assignment
                     << " | Content: " << submissions[idx].content << "\n";
            }
            cout << "Choose submission number to grade (0 = cancel): ";
            int sel; cin >> sel; cin.ignore();
            if (sel <= 0 || sel > (int)pending.size()) {
                cout << "Cancelled.\n";
                continue;
            }
            int idx = pending[sel-1];
            cout << "Enter grade (0-100): ";
            double g; cin >> g; cin.ignore();
            submissions[idx].grade = g;
            cout << "Enter feedback: ";
            string fb; getline(cin, fb);
            submissions[idx].feedback = fb;
            cout << "Submission graded.\n";
        } else if(c==4) {
            cout << "Logging out...\n"; break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

// --- Student Menu ---
void studentMenu(User &u) {
    while(true) {
        cout << "\n[STUDENT MENU]\n";
        cout << "1. View Materials\n2. Submit Assignment\n3. View Grades\n4. Logout\nChoice: ";
        int c; cin >> c; cin.ignore();
        if(c==1) {
            cout << "\n-- Materials --\n";
            for(auto &m: courses[0].materials) cout << "- " << m << endl;
        } else if(c==2) {
            if(courses[0].assignments.empty()) { 
                cout << "No assignments yet.\n"; 
                continue; 
            }
            cout << "Submit for: " << courses[0].assignments.back() << endl;
            string txt; 
            cout << "Content (or 0 to cancel): "; 
            getline(cin, txt);
            if(txt == "0" || txt.empty()) {
                cout << "Cancelled.\n";
            } else {
                submissions.push_back({u.username, courses[0].assignments.back(), txt, -1, ""});
                cout << "Submitted at " << now() << endl;
            }
        } else if(c==3) {
            cout << "\n-- Your Grades --\n";
            for(auto &s: submissions)
                if(s.student==u.username) {
                    cout << s.assignment << ": ";
                    if(s.grade>=0) cout << s.grade << " | Feedback: " << s.feedback << endl;
                    else cout << "Not graded\n";
                }
        } else if(c==4) {
            cout << "Logging out...\n"; break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

// --- Guest Menu ---
void guestMenu(User &u) {
    while(true) {
        cout << "\n[GUEST MENU]\n";
        cout << "1. View Courses\n2. Logout\nChoice: ";
        int c; cin >> c;
        if(c==1) {
            cout << "\n-- Courses --\n";
            for(auto &c: courses)
                cout << c.code << " - " << c.title << endl;
        } else if(c==2) {
            cout << "Returning to main menu...\n"; break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

// --- Login ---
User* login() {
    cout << "Username: "; string un; cin >> un;
    cout << "Password: "; string pw; cin >> pw;
    for(auto &u: users) {
        if(u.username==un && u.password==pw) return &u;
    }
    cout << "Login failed.\n"; return nullptr;
}

// --- Main ---
int main() {
    seedData();
    cout << "=== EduSmart360 Prototype ===\n"; // Change name here if you like
    while(true) {
        cout << "\n1) Login\n2) Continue as Guest\n3) Exit\nChoice: ";
        int ch; cin >> ch;
        if(ch==1) {
            User* u = login();
            if(!u) continue;
            if(u->role==ADMIN) adminMenu(*u);
            else if(u->role==INSTRUCTOR) instructorMenu(*u);
            else if(u->role==STUDENT) studentMenu(*u);
        } else if(ch==2) {
            guestMenu(users.back()); // guest is last in vector
        } else if(ch==3) {
            cout << "Goodbye!\n"; break;
        }
    }
    return 0;
}
