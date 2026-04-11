#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <map>
#include "row.h"
#include "operations.h"

class Pipeline {
    private:
        std::map<std::string, int> header_map;
        std::vector<std::function<std::vector<Row>(const std::vector<Row>&)>> ops;
        std::shared_ptr<const std::vector<Row>> input;
    
    public:
        Pipeline(std::shared_ptr<const std::vector<Row>> input, const std::vector<std::string>& headers);
        Pipeline& map(std::function<Row(const Row&)> func);
        Pipeline& filter(std::function<bool(const Row&)> predicate);
        Pipeline& sort(std::function<bool(const Row&, const Row&)> comp);
        Pipeline& distinct();
        size_t count(); 

        std::vector<double> feature_to_double(const std::string& col_name);
        void normalize_column(const std::string& col_name);

        std::vector<double> feature_to_double(int column_idx);
        void normalize_column(int column_idx);
        
        template<typename T>
        T reduce(std::function<T(T, const Row&)> reducer, T initial) {
            return reduce_rows<T>(this->run(), reducer, initial);
        }

        template<typename T>
        T reduce_parallel(std::function<T(T, const Row&)> reducer, 
                        std::function<T(T, T)> combine, 
                        T initial, int num_threads) {
            return reduce_rows_parallel<T>(this->run(), reducer, combine, initial, num_threads);
        }

        std::vector<Row> run();
        void add_task(std::function<std::vector<Row>(const std::vector<Row>&)> task);

        std::vector<std::vector<double>> to_matrix(const std::vector<std::string>& selected_columns);

};