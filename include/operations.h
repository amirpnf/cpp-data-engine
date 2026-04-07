#include "row.h"
#include <functional>

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