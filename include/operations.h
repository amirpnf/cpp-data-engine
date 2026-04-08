#include "row.h"
#include <functional>
#include <map>

std::vector<Row> map_rows(
    const std::vector<Row>& data,
    std::function<Row(const Row&)> func
);

std::vector<Row> filter_rows(
    const std::vector<Row>& data,
    std::function<bool(const Row&)> predicate
);

std::vector<Row> map_rows_parallel(
    const std::vector<Row>& data,
    std::function<Row(const Row&)> func,
    int num_thread
);

std::vector<Row> filter_rows_parallel(
    const std::vector<Row>& data,
    std::function<bool(const Row&)> predicate,
    int num_thread
);

template<typename T>
T reduce_rows(
    const std::vector<Row>& data,
    std::function<T(T, const Row&)> reducer,
    T initial 
);

size_t count_rows(const std::vector<Row>& data);

std::unordered_map<std::string, std::vector<Row>>
groupby(
    const std::vector<Row>& data,
    std::function<std::string(const Row&)> key_func
);

std::vector<Row> sort_rows(
    const std::vector<Row>& data,
    std::function<bool(const Row&, const Row&)> comp
);

std::vector<Row> distinct_row(
    const std::vector<Row>& data
);