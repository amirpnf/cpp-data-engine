#ifndef ROW_H
#define ROW_H
#include <vector>
#include <string>

struct Row {
    std::vector<std::string> values;

    std::string get(size_t index) const {
        if (index < values.size()) {
            return values[index];
        }
        return ""; 
    }

    bool operator==(const Row& other) const {
        return (values == other.values);
    }
};

struct RowHash {
    std::size_t operator()(const Row& r) const {
        std::size_t seed = 0;

        for (const auto& v : r.values) {
            seed ^= std::hash<std::string>{}(v)
                  + 0x9e3779b9
                  + (seed << 6)
                  + (seed >> 2);
        }

        return seed;
    }
};

#endif