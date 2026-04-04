#ifndef ROW_H
#define ROW_H
#include <vector>
#include <string>

typedef struct {
    int id;
    std::vector<std::string> values;
} Row;

#endif