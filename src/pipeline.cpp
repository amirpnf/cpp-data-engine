#include "pipeline.h"
#include <vector>
#include "operations.h"

Pipeline::Pipeline(const std::vector<Row>& input) : data(input) {}

Pipeline& Pipeline::map(std::function<Row(const Row&)> func) {
    data = map_rows(data, func);
    return *this;
}

Pipeline& Pipeline::filter(std::function<bool(const Row&)> predicate) {
    data = filter_rows(data, predicate);
    return *this;
}

std::vector<Row> Pipeline::run() {
    return data;
}
