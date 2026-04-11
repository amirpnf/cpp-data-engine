#pragma once
#include <vector>
#include <string>
#include "row.h"

class CSVReader {
private:
    std::string filename;
    std::vector<std::string> headers;

public:
    CSVReader(std::string name) : filename(name) {}
    
    std::vector<Row> read_all();
    
    std::vector<std::string> get_headers() { return headers; }
};

Row parse_line(const std::string& line);