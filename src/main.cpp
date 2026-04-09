#include <iostream>
#include <memory>
#include <vector>
#include "pipeline.h"
#include "csv_reader.h" // Assuming this is your reader class

enum Schema {
    ID = 0,
    AREA = 1,
    ROOMS = 2,
    PRICE = 3,
    TYPE = 4
};

int main() {
    // 1. Load Data
    auto data = std::make_shared<std::vector<Row>>(read_csv("test2.csv"));
    Pipeline pipe(data);

    std::cout << "--- Starting Data Engine Test ---" << std::endl;

    auto results = pipe.filter([](const Row& r) {
        // Accessing by index using our enum
        return r.get(Schema::TYPE) == "house"; 
    })
    .map([](const Row& r) {
        Row new_row = r;
        double price = std::stod(r.get(Schema::PRICE));
        
        // ML scaling example: log transformation or normalization
        // Since Row is just a vector, we can append a new "column"
        new_row.values.push_back(std::to_string(price / 1000.0));
        return new_row;
    })
    .run();

    // Verification
    for (const auto& row : results) {
        std::cout << "House ID: " << row.get(Schema::ID) 
                  << " | Price (K): " << row.values.back() << std::endl;
    }

    return 0;
}