#include "operations.h"
#include <thread>
#include <algorithm>

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

std::vector<Row> map_rows_parallel(
    const std::vector<Row>& data,
    std::function<Row(const Row&)> func,
    int num_thread
) {
    std::vector<std::thread> threads;
    std::vector<Row> result(data.size());

    if(num_thread <= 0) num_thread = 1;
    num_thread = std::min(num_thread, (int)data.size());

    int chunk_size = data.size() / num_thread;
    for(int i = 0; i < num_thread; i++) {
        int start = i * chunk_size;
        int end = (i == num_thread - 1) ? data.size() : start + chunk_size;

        threads.emplace_back([&, start, end]() {
            for(int j = start; j < end; j++) {
                result[j] = func(data[j]);
            }
        });
    }

    for(auto& t : threads) {
        t.join();
    }
    return result;
}

std::vector<Row> filter_rows_parallel(
    const std::vector<Row>& data,
    std::function<bool(const Row&)> predicate,
    int num_thread
) {
    
    if(num_thread <= 0) num_thread = 1;
    num_thread = std::min(num_thread, (int)data.size());

    std::vector<std::thread> threads;
    std::vector<std::vector<Row>> local_results(num_thread);

    int chunk_size = data.size() / num_thread;
    for(int i = 0; i < num_thread; i++) {
        int start = i * chunk_size;
        int end = (i == num_thread - 1) ? data.size() : (start + chunk_size);

        threads.emplace_back([&, start, end]() {
            for(int j = start; j < end; j++) {
                if(predicate(data[j])) {
                    local_results[i].push_back(data[j]);
                }
            }
        });
    }

    for(auto& t : threads) {
        t.join();
    }

    std::vector<Row> result;
    for(const auto& local : local_results) {
        result.insert(result.end(), local.begin(), local.end());
    }
    return result;
}

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
    int num_thread=1
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