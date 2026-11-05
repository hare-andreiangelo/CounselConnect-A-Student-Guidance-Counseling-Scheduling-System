// main.cpp - console menu for CounselConnect: A Student Guidance Counseling Scheduling System
#include <limits>
#include <cstdlib>
#include "scheduler.h"

int readInt(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. " << prompt;
    }
    return x;
}

int main() {
    Scheduler s;
    cout << "==============================================\n"
         << "  CounselConnect\n"
         << "  A Student Guidance Counseling Scheduling System\n"
         << "  Office hours: 8:30 AM - 5:30 PM\n"
         << "==============================================\n";
    while (true) {
        cout << "\n1 Book  2 Cancel  3 View day  4 Exit\n";
        int c = readInt("Choice: ");
        if (c == 1) {
            Request r; cout << "Student ID: "; cin >> r.studentId;
            r.date = readInt("Date (YYYYMMDD): ");
            r.start = readInt("Start (24h HHMM, e.g. 1030): ");
            r.start = (r.start / 100) * 60 + r.start % 100;
            r.duration = readInt("Duration (minutes): ");
            string err; int id = s.book(r, err);
            if (id != -1) cout << "Booked. Appointment ID " << id << "\n";
            else {
                cout << "Failed: " << err << "\n";
                if (err.find("Conflicts") != string::npos && readInt("Join waitlist? (1/0): ") == 1) s.addToWaitlist(r);
            }
        } else if (c == 2) {
            cout << (s.cancel(readInt("Appointment ID: ")) ? "Cancelled.\n" : "ID not found.\n");
        } else if (c == 3) {
            s.showDay(readInt("Date (YYYYMMDD): "), false);
        } else if (c == 4) break;
        else cout << "Invalid choice.\n";
    }
}
