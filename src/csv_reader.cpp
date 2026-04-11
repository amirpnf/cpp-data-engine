#include "csv_reader.h"
#include <fstream>
#include <sstream>

std::vector<Row> CSVReader::read_all() {
    std::vector<Row> rows;
    std::string line;
    std::ifstream lines(filename);

    if(!lines.is_open()) {
        throw std::runtime_error("Error : Could not find or open " + filename);
    }

    if(getline(lines, line)) {
        std::stringstream ss(line);
        std::string cell;
        while(getline(ss, cell, ',')) {
            headers.push_back(cell);
        }
    }

    while(getline(lines, line)) {
        rows.push_back(parse_line(line));
    }
    return rows;
}

Row parse_line(const std::string& line) {
    Row row;
    std::stringstream ss(line);
    std::string cell;
    while(getline(ss, cell, ',')) {
        row.values.push_back(cell);
    }
    return row;
}