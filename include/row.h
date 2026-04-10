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

    Row with_value(int column_idx, const std::string& new_value) const {
        Row copy = *this; 
        if (column_idx >= 0 && column_idx < copy.values.size()) {
            copy.values[column_idx] = new_value; 
        }
        return copy;
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