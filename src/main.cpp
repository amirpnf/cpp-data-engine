#include <iostream>
#include <vector>
#include <string>
#include "csv_reader.h"

int main(int argc, char** argv) {
    //std::cout << "Data Engine started" << std::endl;
    std::vector<Row> rows;
    rows = read_csv("test.csv");
    for (const Row& r : rows) {
        std::cout << r.id << " : ";

        for (const auto& val : r.values) {
            std::cout << val << " ";
        }

        std::cout << std::endl;
    }
    return 0;
}