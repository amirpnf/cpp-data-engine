#include "pipeline.h"
#include <vector>
#include "operations.h"

Pipeline::Pipeline(std::shared_ptr<const std::vector<Row>> input) : input(input) {}

Pipeline& Pipeline::map(std::function<Row(const Row&)> func) {
    ops.push_back([func](const std::vector<Row>& data) {
        return map_rows_parallel(data, func, 4);
    });
    return *this;
}

Pipeline& Pipeline::filter(std::function<bool(const Row&)> predicate) {
    ops.push_back([predicate](const std::vector<Row>& data) {
        return filter_rows_parallel(data, predicate, 4);
    }); 
    return *this;
}

Pipeline& Pipeline::sort(std::function<bool(const Row&, const Row&)> comp) {

    ops.push_back([comp](const std::vector<Row>& data){
        return sort_rows(data, comp);
    }) ;
    return *this;
}

Pipeline& Pipeline::distinct() {
    ops.push_back([](const std::vector<Row>& data) {
        return distinct_rows(data);
    });
    return *this;
}

size_t Pipeline::count() {
    std::vector<Row> final_data = this->run();
    return count_rows(final_data); 
}

std::vector<double> Pipeline::feature_to_double(int column_idx) {
    auto final_rows = this->run();
    std::vector<double> column_data;
    column_data.reserve(final_rows.size());

    for(const auto& row : final_rows) {
        try {
            column_data.push_back(std::stod(row.get(column_idx)));
        } catch(...) {
            column_data.push_back(0.0); // Handling non-numeric data gracefully (for now) 
        }
    }
    return column_data;
}


std::vector<Row> Pipeline::run() {
    std::vector<Row> data = *input;
    for(auto& op : ops) {
        data = op(data);
    }
    return data;
}
