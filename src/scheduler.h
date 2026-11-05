// scheduler.h - CounselConnect core: sorted daily lists, binary search, linear search, waitlist queue
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

const int OPEN = 8 * 60 + 30, CLOSE = 17 * 60 + 30, STEP = 30, MAX_DUR = 120;

struct Appointment { int id; string studentId; int date; int start; int end; };
struct Request { string studentId; int date; int start; int duration; };
struct Day { int date; vector<Appointment> appts; };   // appts kept sorted by start time

inline string fmt(int m) {
    int h = m / 60, mm = m % 60, h12 = (h % 12 == 0) ? 12 : h % 12;
    return to_string(h12) + ":" + (mm < 10 ? "0" : "") + to_string(mm) + (h >= 12 ? " PM" : " AM");
}

class Scheduler {
    vector<Day> days;            // dynamic array of day records
    queue<Request> waitlist;     // FIFO for requests that hit a conflict
    int nextId = 1;

    // Linear search: find a day record by date. O(d)
    Day* findDay(int date) {
        for (Day& d : days) if (d.date == date) return &d;
        return nullptr;
    }

    // Binary search: first index whose start >= given start. O(log n)
    static int lowerBound(const vector<Appointment>& v, int start) {
        int lo = 0, hi = (int)v.size();
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (v[mid].start < start) lo = mid + 1; else hi = mid;
        }
        return lo;
    }

public:
    // Returns appointment ID, or -1 with err set.
    int book(const Request& r, string& err) {
        int end = r.start + r.duration;
        if (r.duration <= 0 || r.duration > MAX_DUR || r.duration % STEP != 0) { err = "Duration must be 30-120 min in 30-min steps."; return -1; }
        if (r.start % STEP != 0) { err = "Start time must be on a 30-min boundary."; return -1; }
        if (r.start < OPEN || end > CLOSE) { err = "Session must fit within 8:30 AM - 5:30 PM."; return -1; }

        Day* day = findDay(r.date);
        if (!day) { days.push_back({r.date, {}}); day = &days.back(); }
        vector<Appointment>& v = day->appts;

        int pos = lowerBound(v, r.start);
        // Sorted + non-overlapping, so only the two neighbors need checking.
        // Overlap rule: NewStart < ExistingEnd AND NewEnd > ExistingStart
        if (pos > 0 && v[pos - 1].end > r.start) { err = "Conflicts with an earlier booking."; return -1; }
        if (pos < (int)v.size() && v[pos].start < end) { err = "Conflicts with a later booking."; return -1; }

        Appointment a{nextId++, r.studentId, r.date, r.start, end};
        v.insert(v.begin() + pos, a);            // keeps the day sorted. O(n) shift
        return a.id;
    }

    // Linear search by appointment ID across all days, then erase. O(n)
    bool cancel(int id) {
        for (Day& d : days) {
            for (size_t i = 0; i < d.appts.size(); i++) {
                if (d.appts[i].id == id) {
                    d.appts.erase(d.appts.begin() + i);
                    promoteWaitlist();
                    return true;
                }
            }
        }
        return false;
    }

    void addToWaitlist(const Request& r) { waitlist.push(r); }

    // One FIFO pass: try each waiting request; re-queue the ones that still don't fit.
    void promoteWaitlist() {
        int n = (int)waitlist.size();
        while (n--) {
            Request r = waitlist.front(); waitlist.pop();
            string err;
            int id = book(r, err);
            if (id != -1) cout << "[Waitlist] " << r.studentId << " booked, ID " << id << "\n";
            else waitlist.push(r);
        }
    }

    void showDay(int date, bool privateView) {
        Day* d = findDay(date);
        if (!d || d->appts.empty()) { cout << "No bookings.\n"; return; }
        for (const Appointment& a : d->appts) {
            cout << fmt(a.start) << " - " << fmt(a.end) << "  ";
            if (privateView) cout << "Booked\n";      // students never see other students' details
            else cout << a.studentId << " (ID " << a.id << ")\n";
        }
    }
};

#endif
