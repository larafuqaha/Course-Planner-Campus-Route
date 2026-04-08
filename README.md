# Course Prerequisite Planner & Campus Route Finder

A C project developed as part of the Data Structures and Algorithms course. It combines two features: sorting computer engineering courses by their prerequisites using Topological Sort, and finding the shortest path between campus buildings using Dijkstra's algorithm.

## Overview

The program loads two input files — one for courses and their prerequisites, one for campus buildings and distances — and provides a menu to sort courses and find shortest routes between buildings.

## Features

### Topological Sort (fully implemented)
- Loads courses and their prerequisites from `input_courses.txt`
- Represents courses as a directed graph using an adjacency list
- Sorts courses in a valid order where all prerequisites come before the courses that require them
- Prints the sorted course order on request

### Dijkstra's Algorithm (partially implemented)
- Loads buildings and distances from `input_buildings.txt`
- Represents the campus map as an adjacency matrix
- Calculates the shortest distance between two buildings
- Route printing (menu option 3) is not yet implemented

## Input Files

**`input_courses.txt`** — each line lists a course followed by its prerequisites, separated by `#`:
```
COMP2310#COMP133
ENCS3340#COMP233#COMP133
```

**`input_buildings.txt`** — each line lists a source building, destination building, and distance, separated by `#`:
```
Masri#A.Shaheen#70
A.Shaheen#Aggad#60
```

## Menu Options

1. Load the two input files
2. Calculate the shortest distance between two buildings
3. Print the shortest route between two buildings *(not yet implemented)*
4. Sort the courses using Topological Sort
5. Print the sorted courses
6. Exit

## How to Compile and Run

```bash
gcc project4.c -o project4
./project4
```

Make sure `input_courses.txt` and `input_buildings.txt` are in the same directory as the executable.
