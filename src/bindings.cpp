#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h> 
#include "pipeline.h"
#include "csv_reader.h"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<Row>);

PYBIND11_MODULE(cpp_data_engine, m) {
    py::class_<Row>(m, "Row")
        .def("get", &Row::get);

    py::bind_vector<std::vector<Row>>(m, "VectorRow");

    py::class_<Pipeline>(m, "Pipeline")
        .def("run", &Pipeline::run)
        .def("feature_to_double", &Pipeline::feature_to_double)
        .def("count", &Pipeline::count);

    m.def("load_data", [](std::string path) {
        auto data = std::make_shared<std::vector<Row>>(read_csv(path));
        return new Pipeline(data); 
    });
}