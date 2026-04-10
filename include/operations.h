#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "row.h"
#include <functional>
#include <thread>
#include <vector>
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
) {

    T acc = initial;
    for(const auto& val : data) {
        acc = reducer(acc, val);
    }
    return acc;
}

template<typename T>
T reduce_rows_parallel(
    const std::vector<Row>& data,
    std::function<T(T, const Row&)> reducer,
    std::function<T(T, T)> combine,
    T initial,
    int num_thread
) {

    if(data.empty()) return initial;

    num_thread = std::min(num_thread, (int)data.size());

    std::vector<std::thread> threads;
    std::vector<T> partial_results(num_thread, initial);

    int chunk_size = data.size() / num_thread;
    
    for(int i = 0; i < num_thread; i++) {
        int start = i * chunk_size;
        int end = (i == num_thread - 1) ? data.size() : start + chunk_size;

        threads.emplace_back([&, start, end, i]() {
            T acc = initial;
            for(int j = start; j < end; j++) {
                acc = reducer(acc, data[j]);
            }
            partial_results[i] = acc;
        });
    }

    for(auto& t : threads) {
        t.join();
    }

    T result = initial;
    for(const T& val : partial_results) {
        result = combine(result, val);
    }
    return result;
}

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

std::vector<Row> distinct_rows(
    const std::vector<Row>& data
);

double safe_stod(const std::string& str);

#endif