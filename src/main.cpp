#include <iostream>
#include <fstream>
#include <string>

#include "Util.h"
#include "UncompressedImageData.h"
#include "QuantMatrix.h"

using namespace std;

void encode(UncompressedImageData*, QuantMatrix*, double, string);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cout << "usage: myDCT  <input image> <quantfile> <qscale> <output file>" << endl;
        return 1;
    }

    string::size_type sz;
    string input_image = argv[1];
    string quant_file = argv[2];
    double qscale;
    try {
        qscale = stod(argv[3], &sz);
    } catch(...) {
        throw runtime_error("could not parse quantization value");
    }
    string output_file = argv[4];

    UncompressedImageData *unc = read_image_file(input_image);

    QuantMatrix *qnt = read_quant_file(quant_file);

    encode(unc, qnt, qscale, output_file);

    return 0;
}

/*
 * TODO: Make function description
 */
void encode(UncompressedImageData* unc, QuantMatrix *qnt, double qscale, string filename) {
    std::ofstream ofs (filename, std::ios_base::app);
    write_header(unc, qscale, ofs);
    for (int x = 0; x < unc->header.x_size; x += 16) {
        for (int y = 0; y < unc->header.y_size; y += 16) {
            vector<vector<double>> f_data(MACROBLOCK_X, vector<double>(MACROBLOCK_Y));
            for (int i = 0; i < MACROBLOCK_X; i += 8) {
                for (int j = 0; j < MACROBLOCK_Y; j += 8) {
                    vector<vector<double>> d_block(BLOCK_X, vector<double>(BLOCK_Y));
                    vector<vector<double>> f_block(BLOCK_X, vector<double>(BLOCK_Y));
                    vector<vector<unsigned char>> out_block(BLOCK_X, vector<unsigned char>(BLOCK_Y)); // block after quantization has been applied
                    // cout << "processing block at x = " << i + x << " y = " << j + y << endl;
                    copy_block(unc->image_data, d_block, i + x, j + y);  // spatial block
                    // cout << "spatial:" << endl;
                    // print_block(d_block);
                    dct(d_block, f_block);  // dct block
                    // cout << "dct:" << endl;
                    // print_block(f_block);
                    quantize_and_clip(f_block, qnt->quant_matrix, out_block, qscale);
                    // cout << "quantized and clipped:" << endl;
                    // print_block(out_block);
                    zig_zag_reorder(out_block);// zig zag reorder
                    // cout << "zig zag reordered:" << endl;
                    // print_block(out_block);
                    // cout << "----------------------------------------------" << endl;
                    ofs << x << ' ' << y << endl << ' ';
                    write_block(out_block, ofs);
                }
            }
        }
    }
}