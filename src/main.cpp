#include "pipeline.h"
#include "csv_reader.h"
#include <memory>
#include <vector>

int main() {
    CSVReader reader("test2.csv");

    auto raw_data = reader.read_all();
    auto headers = reader.get_headers();

    auto data = std::make_shared<std::vector<Row>>(raw_data);
    Pipeline pipe(data, headers);

    pipe.normalize_column("SomeColumnName"); 
    auto result = pipe.run();

    return 0;
}