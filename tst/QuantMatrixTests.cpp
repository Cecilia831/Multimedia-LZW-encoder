#include "gtest/gtest.h"

#ifndef UNCOMPRESSED_IMAGE_DATA
    #include "Util.h"
#endif
#ifndef QUANT_MATRIX
    #include "QuantMatrix.h"
#endif
#include <iostream>

TEST(QuantMatrixTest, valid_data) {
    try {
        QuantMatrix *qnt = read_quant_file("../tst/quantfile");
    } catch (...) {
        // There should not have been an error here
    }
}

TEST(QuantMatrixTest, validate_data) {
     
    std::vector<int> valid_data = {16,11,10,16,24,40,51,61,12,12,14,19,26,58,60,55,14,13,16,24,40,57,69,56,
                               14,17,22,29,51,87,80,62,18,22,37,56,68,109,103,77,24,35,55,64,81,104,113,
                               92,49,64,78,87,103,121,120,101,72,92,95,98,112,100,103,99};
    try { 
        QuantMatrix *qnt = read_quant_file("../tst/quantfile");
        int i;
        for (int x = 0, i = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++, i++) {
                EXPECT_EQ(qnt->quant_matrix[x][y], valid_data[i]);
            }
        }
    } catch (...) {
        // There should not have been an error
    }
}