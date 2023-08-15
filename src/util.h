#pragma once

#include <vector>
#include <string>
#ifndef UNCOMPRESSED_IMAGE_DATA1
    #include "UncompressedImageData.h"
#endif
#ifndef QUANT_MATRIX
    #include "QuantMatrix.h"
#endif

using namespace std;

#define PICTURE_X 640
#define PICTURE_Y 480
#define MAXVAL 255
#define MACROBLOCK_X 16
#define MACROBLOCK_Y 16
#define BLOCK_X 8
#define BLOCK_Y 8
#define QUANT_MATRIX_SIZE 8

UncompressedImageData* read_image_file(string);
QuantMatrix* read_quant_file(string);

void copy_block(vector<vector<unsigned char>> &, vector<vector<double>> &, int, int);
void copy_block(vector<vector<unsigned char>> &, vector<vector<unsigned char>> &, int, int);

void dct (vector<vector<double>> &, vector<vector<double>> &);
void quantize_and_clip(vector<vector<double>> &, vector<vector<double>> &, vector<vector<unsigned char>> &, double);
void zig_zag_reorder(vector<vector<unsigned char>> &);
void write_block(vector<vector<unsigned char>> &, ofstream&);
void write_header(UncompressedImageData* unc, double quant_value, ofstream &ofs);

// Debugging
void print_block(vector<vector<double>> &);
void print_block(vector<vector<unsigned char>> &);
