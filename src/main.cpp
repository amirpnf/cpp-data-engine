#include <iostream>
#include <vector>
#include <string>
#include "csv_reader.h"
#include "operations.h"

int main(int argc, char** argv) {
    //std::cout << "Data Engine started" << std::endl;
    std::vector<Row> rows;
    rows = read_csv("test.csv");

    std::vector<Row> new_rows = filter_rows(rows, [](const Row& r) {
        if(r.values[3] == "Paris") return true;
        return false;
    });

    for (const Row& r : new_rows) {
        std::cout << r.id << " : ";

        for (const auto& val : r.values) {
            std::cout << val << " ";
        }

        std::cout << std::endl;
    }
    return 0;
}