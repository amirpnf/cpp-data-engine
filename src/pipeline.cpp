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

std::vector<Row> Pipeline::run() {
    std::vector<Row> data = *input;
    for(auto& op : ops) {
        data = op(data);
    }
    return data;
}
