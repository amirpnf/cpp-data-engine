#pragma once
#include <vector>
#include <functional>
#include <memory>
#include "row.h"

class Pipeline {
    private:
        std::vector<std::function<std::vector<Row>(const std::vector<Row>&)>> ops;
        std::shared_ptr<const std::vector<Row>> input;
    
    public:
        Pipeline(std::shared_ptr<const std::vector<Row>> input);
        Pipeline& map(std::function<Row(const Row&)> func);
        Pipeline& filter(std::function<bool(const Row&)> predicate);

        std::vector<Row> run();

};