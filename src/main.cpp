#include <iostream>
#include <vector>
#include <string>
#include "csv_reader.h"
#include "pipeline.h"
#include "operations.h"

int main(int argc, char** argv) {
    std::vector<Row> rows;
    rows = read_csv("test.csv");

    auto normal = filter_rows(rows, [](const Row& r) {
        return r.values.size() > 3 && r.values[3] == "Paris";
    });

    auto parallel = filter_rows_parallel(rows, [](const Row& r) {
        return r.values.size() > 3 && r.values[3] == "Paris";
    }, 4);

    std::cout << "Normal size: " << normal.size() << std::endl;
    std::cout << "Parallel size: " << parallel.size() << std::endl;


    bool same = (normal.size() == parallel.size());

    for (size_t i = 0; i < normal.size() && same; i++) {
        if (normal[i].id != parallel[i].id) {
            same = false;
        }
    }

    std::cout << "Same result? " << same << std::endl;
    
    return 0;
}