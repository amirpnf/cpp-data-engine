#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "pipeline.h"
#include "csv_reader.h" 

namespace py = pybind11;

PYBIND11_MODULE(cpp_data_engine, m) {
    py::class_<Pipeline>(m, "Pipeline")
        .def(py::init<std::shared_ptr<const std::vector<Row>>, const std::vector<std::string>&>())
        .def("run", &Pipeline::run)
        .def("count", &Pipeline::count)
        .def("to_matrix", &Pipeline::to_matrix)
        .def("feature_to_double", py::overload_cast<int>(&Pipeline::feature_to_double))
        .def("feature_to_double", py::overload_cast<const std::string&>(&Pipeline::feature_to_double))
        .def("normalize_column", py::overload_cast<int>(&Pipeline::normalize_column))
        .def("normalize_column", py::overload_cast<const std::string&>(&Pipeline::normalize_column))
        .def("filter", &Pipeline::filter)
        .def("map", &Pipeline::map);

    m.def("load_data", [](std::string path) {
        CSVReader reader(path);
        auto data = std::make_shared<std::vector<Row>>(reader.read_all());
        auto headers = reader.get_headers();
        
        return Pipeline(data, headers); 
    }); 
} 