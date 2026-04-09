#include "operations.h"
#include <thread>
#include <algorithm>
#include <unordered_set>

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

std::vector<Row> filter_rows_parallel(const std::vector<Row>& data, std::function<bool(const Row&)> predicate, int num_thread) {
    // TO BE READ AND UNDERSTOOD
    if (data.empty()) return {};
    
    int threads_to_use = std::max(1, std::min(num_thread, (int)data.size()));
    
    std::vector<std::vector<Row>> buckets(threads_to_use);
    std::vector<std::thread> threads;
    int chunk_size = data.size() / threads_to_use;

    for (int i = 0; i < threads_to_use; ++i) {
        int start = i * chunk_size;
        int end = (i == threads_to_use - 1) ? data.size() : start + chunk_size;

        threads.emplace_back([&, start, end, i]() {
            for (int j = start; j < end; ++j) {
                if (predicate(data[j])) {
                    buckets[i].push_back(data[j]); 
                }
            }
        });
    }

    for (auto& t : threads) t.join();

    std::vector<Row> final_result;
    for (const auto& bucket : buckets) {
        final_result.insert(final_result.end(), bucket.begin(), bucket.end());
    }

    return final_result;
}

size_t count_rows(const std::vector<Row>& data) {
    int count = 0;
    for(const auto& v : data) {
        count++;
    }
    return count;
}

std::unordered_map<std::string, std::vector<Row>>
groupby(
    const std::vector<Row>& data,
    std::function<std::string(const Row&)> key_func
) {

    std::unordered_map<std::string, std::vector<Row>> groups;
    if(data.empty()) return groups;

    for(const auto& row : data) {
        std::string group = key_func(row);
        groups[group].push_back(row);
    }

    return groups;
}

std::vector<Row> sort_rows(
    const std::vector<Row>& data,
    std::function<bool(const Row&, const Row&)> comp
) {

    auto sorted = data;
    std::stable_sort(sorted.begin(), sorted.end(), comp);
    return sorted;
}

std::vector<Row> distinct_rows(
    const std::vector<Row>& data
) {
    std::vector<Row> result;
    std::unordered_set<Row, RowHash> seen;

    result.reserve(data.size());

    for(const auto& row : data) {
        if(seen.find(row) == seen.end()) {
            seen.insert(row);
            result.push_back(row);
        }
    }
    return result;
}