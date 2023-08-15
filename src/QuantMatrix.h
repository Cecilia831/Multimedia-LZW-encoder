#pragma once

#include <vector>

class QuantMatrix {
    public:
        QuantMatrix(std::vector <unsigned char>);
        std::vector<std::vector<double>> quant_matrix;
};