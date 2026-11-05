// benchmark.cpp - experimental timing for Section 5 (book and cancel vs. input size n)
#include <chrono>
#include "scheduler.h"
using namespace std::chrono;

int main() {
    const int sizes[] = {100, 1000, 10000, 100000};
    const int REPS = 5, OPS = 100;
    cout << "n,book_ms,cancel_ms\n";
    for (int n : sizes) {
        double bookTotal = 0, cancelTotal = 0;
        for (int rep = 0; rep < REPS; rep++) {
            Scheduler s;
            string err;
            // Preload n appointments, 10 per day (dates are plain integers here)
            for (int i = 0; i < n; i++) {
                Request r{"S" + to_string(i), 20000000 + i / 10, OPEN + (i % 10) * STEP, STEP};
                s.book(r, err);
            }
            // Time OPS bookings on brand-new days
            auto t0 = steady_clock::now();
            for (int j = 0; j < OPS; j++) {
                Request r{"X" + to_string(j), 30000000 + j, OPEN, STEP};
                s.book(r, err);
            }
            auto t1 = steady_clock::now();
            // Time OPS cancels of the most recently preloaded IDs (late in the search order)
            for (int j = 0; j < OPS; j++) s.cancel(n - j);
            auto t2 = steady_clock::now();
            bookTotal += duration<double, milli>(t1 - t0).count();
            cancelTotal += duration<double, milli>(t2 - t1).count();
        }
        cout << n << "," << bookTotal / REPS << "," << cancelTotal / REPS << "\n";
    }
}
