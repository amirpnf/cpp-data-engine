#pragma once
#include <vector>
#include <functional>
#include "row.h"

class Pipeline {
    private:
        std::vector<Row> data;
    
    public:
        Pipeline(const std::vector<Row>& input);
        Pipeline& map(std::function<Row(const Row&)> func);
        Pipeline& filter(std::function<bool(const Row&)> predicate);

        std::vector<Row> run();

};