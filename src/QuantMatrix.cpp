#ifndef QUANT_MATRIX
    #include "QuantMatrix.h"
#endif
#ifndef UTIL
    #include "Util.h"
#endif
#include <iostream>

using namespace std;

QuantMatrix::QuantMatrix(vector <unsigned char> raw_data) {
    string::size_type sz;
    
    vector<int> int_data;
    string num;

    for (int x = 0; x < QUANT_MATRIX_SIZE; x++) {
        vector<double> vec;
        for (int y = 0; y < QUANT_MATRIX_SIZE; y++) {
            vec.push_back(0);
        }
        this->quant_matrix.push_back(vec);
    }

    for (int i = 0; i < raw_data.size(); i++) {
        while((i < raw_data.size()) && (raw_data[i] != ' ') && (raw_data[i] != '\n')) {
            num.push_back(raw_data[i]);
            i++;
        }   
        if (!num.empty()) {
            try {
                int_data.push_back(stoi(num, &sz));
                num.clear();
            } catch (...) {
                throw(invalid_argument("Invalid quantfile"));
            }   
        }   
        // skip the space TODO Huge assumption that there is a space.
    }   
    if (int_data.size() != QUANT_MATRIX_SIZE * QUANT_MATRIX_SIZE) {
        throw runtime_error("Invalid quant matrix");
    } 
    int i;
    for (int x = 0, i = 0; x < QUANT_MATRIX_SIZE; x++) {
        for (int y = 0; y < QUANT_MATRIX_SIZE; y++, i++) {
            this->quant_matrix[x][y] = int_data[i];
        }   
    }
}