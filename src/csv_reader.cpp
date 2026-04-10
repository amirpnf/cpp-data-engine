#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "row.h"
#include "csv_reader.h"
using namespace std;

vector<Row> read_csv(string filename) {
    
    vector<Row> rows;
    string line;
    ifstream lines(filename);

    if(!lines.is_open()) {
        throw runtime_error("Error : Could not find or open " + filename);
    }

    int index = 1;

    getline(lines, line); // skipping the first line
    Row row;

    while(getline(lines, line)) {
        row = parse_line(line);
        rows.push_back(row);
        index++; 
    }
    return rows;
}

Row parse_line(const string& line) {
    Row row;
    stringstream ss(line);
    string cell;

    while(getline(ss, cell, ',')) {
        row.values.push_back(cell);
    }
    return row;
}
