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
            column_data.push_back(safe_stod(row.get(column_idx)));
        } catch(...) {
            column_data.push_back(0.0); // Handling non-numeric data gracefully (for now) 
        }
    }
    return column_data;
}

void Pipeline::normalize_column(int column_idx) {
    auto current_data = this->run();
    if(current_data.empty()) return;

    double min_val = std::numeric_limits<double>::max();
    double max_val = std::numeric_limits<double>::min();

    for(const auto& row : current_data) {
        double val = safe_stod(row.get(column_idx));
        if(val < min_val) min_val = val;
        if(val > max_val) max_val = val;
    }

    double range = max_val - min_val;
    if(range == 0) return;

    this->add_task([column_idx, min_val, range](const std::vector<Row>& data) {
        return map_rows_parallel(data, [column_idx, min_val, range](const Row& row) {
            try {
                double old_val = safe_stod(row.get(column_idx));
                double normalized = (old_val - min_val) / range;
                return row.with_value(column_idx, std::to_string(normalized));
            } catch(...) {
                return row; 
            }
        }, 4);
    });
}


std::vector<Row> Pipeline::run() {
    std::vector<Row> data = *input;
    for(auto& op : ops) {
        data = op(data);
    }
    return data;
}

void Pipeline::add_task(std::function<std::vector<Row>(const std::vector<Row>&)> task) {
    ops.push_back(task);
}
