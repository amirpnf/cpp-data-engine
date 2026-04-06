#include "operations.h"

std::vector<Row> map_rows(
    const std::vector<Row>& data,
    std::function<Row(const Row&)> func
) {
    std::vector<Row> result;
    
    for(const Row& r : data) {
        result.push_back(func(r));
    }
    return result;
}

std::vector<Row> filter_rows(
    const std::vector<Row>& data,
    std::function<bool(const Row&)> predicate
) {
    std::vector<Row> result;

    for(const Row& r : data) {
        if(predicate(r)) {
            result.push_back(r);
        }
    }
    return result;
}