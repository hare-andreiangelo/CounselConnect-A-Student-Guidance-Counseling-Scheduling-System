# CounselConnect: A Student Guidance Counseling Scheduling System

A C++ console program that emulates how a school Guidance Counseling Office schedules student appointments, built as the Final Project for **CPE 010 - Data Structures and Algorithms**.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue) ![Platform](https://img.shields.io/badge/app-console-lightgrey) ![Course](https://img.shields.io/badge/course-CPE%20010-green)

## Course Information

| Item | Details |
|---|---|
| Course | CPE 010 - Data Structures and Algorithms |
| Section / Group | [e.g., CPE21S3 / Group 2] |
| Instructor | [Prof. Full Name] |
| School / Department | [University Name, Department] |
| Date Submitted | [Month Day, Year] |

## Team Members

| # | Full Name (Surname, First Name M.I.) | Student No. | Role | Main Contributions |
|---|---|---|---|---|
| 1 | [Surname, First Name M.I.] | [00-0000] | Project Leader | [Scheduler core, repository setup] |
| 2 | [Surname, First Name M.I.] | [00-0000] | [Role] | [Waitlist queue, flowcharts] |
| 3 | [Surname, First Name M.I.] | [00-0000] | [Role] | [Benchmark code, Section 5] |
| 4 | [Surname, First Name M.I.] | [00-0000] | [Role] | [Documentation, testing] |

## Table of Contents
1. [Overview](#overview)
2. [Features](#features)
3. [Data Structures and Algorithms](#data-structures-and-algorithms)
4. [Performance](#performance)
5. [Getting Started](#getting-started)
6. [Sample Run](#sample-run)
7. [Project Structure](#project-structure)
8. [Limitations](#limitations)
9. [Future Improvements](#future-improvements)
10. [AI Use Disclosure](#ai-use-disclosure)

## Overview

Students book guidance counseling sessions of 30 to 120 minutes (in 30-minute steps) between 8:30 AM and 5:30 PM. The system keeps each day's appointments sorted by start time, rejects overlapping bookings, and places students on a first-in, first-out waitlist when their requested time is taken. When an appointment is cancelled, waiting requests are automatically promoted.

**Overlap rule:** two appointments conflict when `NewStart < ExistingEnd AND NewEnd > ExistingStart`. Back-to-back sessions (for example 11:00-12:00 then 12:00-1:00) are allowed.

## Features

- Book an appointment with date, start time, and duration
- Validation of duration, 30-minute grid, and office hours
- Conflict detection against neighboring appointments
- Waitlist with automatic promotion on cancellation
- Cancel an appointment by ID
- View a day's schedule (student view shows only "Booked" to protect privacy)
- Invalid input handling (non-numeric input, bad menu choice, out-of-range times)

## Data Structures and Algorithms

| Topic | Where it is used |
|---|---|
| Dynamic array (`vector`) | Each day's appointments, kept sorted by start time |
| Binary search | Finds the insert position and the two neighbors to check for overlap |
| Linear search | Finds a day record or an appointment by ID |
| Queue | FIFO waitlist for requests that hit a conflict |
| Sorting (sorted insertion) | Keeps each day ordered as new bookings are added |

## Performance

| Operation | Technique | Time complexity |
|---|---|---|
| Book | Linear day lookup + binary search + sorted insert | O(d + log k + k) |
| Cancel | Linear search by ID, then erase | O(n) |
| View day | Linear day lookup + traversal | O(d + k) |
| Join waitlist | Queue enqueue | O(1) |

`n` = total appointments, `d` = number of days with bookings, `k` = appointments in one day (at most 18).

Experimental timings (n = 100 to 100,000) come from `src/benchmark.cpp`. See Section 5 of the project document for the full analysis and graph.

## Getting Started

**Requirements:** a C++17 compiler (g++ 9 or newer recommended).

```bash
# Clone
git clone https://github.com/[username]/CounselConnect-A-Student-Guidance-Counseling-Scheduling-System.git
cd CounselConnect-A-Student-Guidance-Counseling-Scheduling-System

# Build and run the program
g++ -std=c++17 -O2 src/main.cpp -o counselconnect
./counselconnect

# Build and run the benchmark
g++ -std=c++17 -O2 src/benchmark.cpp -o benchmark
./benchmark
```

On Windows, run `counselconnect.exe` and `benchmark.exe` instead.

## Sample Run

Input (book a 1-hour session, try an overlapping one, then view the day):

```
1
S2024-001
20261104
1030
60
1
S2024-002
20261104
1100
60
1
3
20261104
4
```

Expected result: the first booking succeeds, the second reports a conflict and offers the waitlist, and the day view lists `10:30 AM - 11:30 AM`.

Screenshots of each feature are in the [`docs/`](docs/) folder.

## Project Structure

```
CounselConnect-A-Student-Guidance-Counseling-Scheduling-System/
├── src/
│   ├── scheduler.h      # core scheduling logic
│   ├── main.cpp         # console menu
│   └── benchmark.cpp    # experimental timing code
├── docs/                # flowcharts and output screenshots
├── README.md
└── .gitignore
```

## Limitations

- Console program only; data is kept in memory and lost when the program exits
- Single user, no login or network
- Dates are entered as plain `YYYYMMDD` numbers without calendar validation
- Time zone and holidays are not handled

## Future Improvements

- Save and load appointments from a text file
- Keep the day list sorted by date so day lookup becomes O(log d)
- Add reschedule as an atomic cancel-and-rebook
- Add a student login and a counselor view with blocked periods (lunch, meetings)
- Add a per-student daily booking limit

## AI Use Disclosure

[Per the course AI policy, state which AI tool was used, what it was used for, and which parts of the project it affected. Only include this after obtaining the instructor's approval.]

## Acknowledgments

Thanks to [Instructor name] and the [Department name] for guidance throughout the course.
