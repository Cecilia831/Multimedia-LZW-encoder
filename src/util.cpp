#ifndef UTIL
    #include "Util.h"
#endif
#ifndef QUANT_MATRIX
#define QUANT_MATRIX
    #include "QuantMatrix.h"
#endif
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

/*
 * TODO: Make function description
 */
UncompressedImageData* read_image_file(string filename) {
    ifstream input_file(filename); // TODO: Confirm file exists?
    char character; 
    vector <unsigned char> data;
   
    while (input_file.good()) {
        input_file.get(character);
        data.push_back((unsigned char) character);
    }
    
    UncompressedImageData *unc = new UncompressedImageData(data);

    return unc;
}

/*
 * TODO: Make function description
 */
QuantMatrix* read_quant_file(string filename) {
    ifstream input_file(filename);
    char character;
    vector <unsigned char> data;

    while (input_file.good()) {
        input_file.get(character);
        data.push_back((unsigned char) character);
    }
    
    QuantMatrix *qm = new QuantMatrix(data);

    return qm;
}

/* Given two 2D vectors, copy the contents of a region in the first matrix to the second. Validation of the offsets
 * and the vector sizes should be handled outside function- it fails silently if the copying isn't possible.
 *
 * Args: src 2D vector ref, dst 2D vector ref, x offset into matrix, y offset into matrix, block x size, block y size
 *
 * Returns: vector of vectors of unsigned chars 
 */
void copy_block(vector<vector<unsigned char>> &d_data, vector<vector<double>> &block, int off_x, int off_y) {
    if(!d_data.empty() && !d_data[0].empty() &&
       !block.empty() && !block[0].empty() &&
       block.size() + off_x <= d_data.size() &&
       block[0].size() + off_y <= d_data[0].size()) {
        for (int i = 0; i < block.size(); i++) {
            for (int j = 0; j < block.size(); j++) {   
                block[i][j] = (double)d_data[i + off_x][j + off_y];
            }
        }
    }  
}

// Only used in testing
void copy_block(vector<vector<unsigned char>> &d_data, vector<vector<unsigned char>> &block, int off_x, int off_y) {
    if(!d_data.empty() && !d_data[0].empty() &&
       !block.empty() && !block[0].empty() &&
       block.size() + off_x <= d_data.size() &&
       block[0].size() + off_y <= d_data[0].size()) {
        
        double val;
        string::size_type sz;
        for (int i = 0; i < block.size(); i++) {

            for (int j = 0; j < block.size(); j++) {             
                block[i][j] =  d_data[i + off_x][j + off_y];                   
            }
        }
    }  
}

/*
 * TODO: Make function description
 */
void dct (vector<vector<double>> &d_data, vector<vector<double>> &f_data) {
    for (int x = 0; x < BLOCK_X; x++) {
       for (int y = 0; y < BLOCK_Y; y++) {
           f_data[x][y] = d_data[x][y];
       }   
    }   
    double sum = 0, Cu = 0, Cv = 0;
    for (int u = 0; u < BLOCK_X; u++) { 
        for (int v = 0; v < BLOCK_Y; v++) {
            if (u == 0) {
                Cu = 1/sqrt(2);
            } else {
                Cu = 1;
            }   
            if (v == 0){ 
                Cv = 1/sqrt(2);
            } else {
                Cv = 1;
            }   
            sum = 0;
            for (int x = 0; x < BLOCK_X; x++) {
                for (int y = 0; y < BLOCK_Y; y++) {
                    sum += d_data[x][y] * cos((2*x+1)*u*M_PI/16) * cos((2*y+1)*v*M_PI/16);;
                }   
            }
            f_data[u][v] = Cu*Cv*sum/4.0;
        }   
    }
}


/*
 * TODO: Make function description
 */
void quantize_and_clip(vector<vector<double>> &f_block, vector<vector<double>> &quant_matrix, vector<vector<unsigned char>> &out_block, double qscale) {
    // quantcoeff[x][y] = round( dctmatrix[x][y] / (qt[x][y]*qscale))
    double out_byte;
    for (int x = 0; x < f_block.size(); x++) {
        for (int y = 0; y < f_block[0].size(); y++) {
            // crop the values of the coefficients to the range [-127,128]
            out_byte = round(f_block[x][y] / quant_matrix[x][y] * qscale);           // <- error, should be quant matrix
            // anything between -200 and -127 will be mapped to -127.
            if (out_byte < -127.0) {
                out_byte = -127.0;
            } // anything between 128 and 200 will be remapped to 128. 
            else if (out_byte > 128.0) {
                out_byte = 128.0;
            }
            // add 127 to each of the coefficients to map the range of [-127,128] to [0,255]
            out_byte += 127.0;

            out_block[x][y] = (unsigned char)out_byte; // TODO explicit cast here, is this the best practice?
        }
    }
}

/*
 * TODO: Make function description
 */
void zig_zag_reorder(vector<vector<unsigned char>> &block) {
    vector<vector<int>> index_lookup = {{0, 1, 5, 6, 14, 15, 27, 28}, {2, 4, 7, 13, 16, 26, 29, 42}, {3, 8, 12, 17, 25, 30, 41, 43},
                                        {9, 11, 18, 24, 31, 40, 44, 53}, {10, 19, 23, 32, 39, 45, 52, 54}, {20, 22, 33, 38, 46, 51, 55, 60},
                                        {21, 34, 37, 47, 50, 56, 59, 61}, {35, 36, 48, 49, 57, 58, 62, 63}};

    vector<vector<double>> v(block.size(), vector<double>(block[0].size()));
    unsigned char temp;
    for (int x = 0; x < block.size(); x++) {
        for (int y = 0; y < block[0].size(); y++) {
            int posn = x * 8 + y;
            int _block_x, _block_y; // TODO: Can this not be n^2?
            for (int i = 0; i < index_lookup.size(); i++) {
                for (int j = 0; j < index_lookup[0].size(); j++) {
                    if (index_lookup[i][j] == posn) {
                        _block_y = j;
                        break;
                    }
                }
                if (index_lookup[i][_block_y] == posn) {
                    _block_x = i;
                    break;
                }
            }
            v[x][y] = block[_block_x][_block_y];
        }
    }
    for (int x = 0; x < block.size(); x++) {
        for (int y = 0; y < block[0].size(); y++) {
            block[x][y] = v[x][y];
        }
    }
}

void write_header(UncompressedImageData* unc, double quant_value, ofstream &ofs) {
    ofs << "MYDCT" << endl
        << unc->header.x_size << ' ' << unc->header.y_size << endl
        << to_string(quant_value) << endl; // TODO is there a glitch in the provided file? there's another line of `0 0\n` below the quant value and before the data 
}


// TODO: file always appends. Could pass in stream so that we only use one file write for the duration of the program. If another file is found
//       before any writing is done, we would simply overwrite that file and continue appending to our own.
// TODO: Make sure the file exists!!! Otherwise there is a segmentation fault...
void write_block(vector<vector<unsigned char>> &block, ofstream &ofs) {
    for (int x = 0; x < block.size(); x++) {
        for (int y = 0; y < block[0].size(); y++) {
            if ((x == block.size() - 1) && (y == block[0].size() - 1)) {
                ofs << right << setfill(' ') << setw(4) << (double)block[x][y];
            } else {
                ofs << right << setfill(' ') << setw(4) << (double)block[x][y] << ' '; // TODO is this cast as good as it gets? Be sure to document this.
            }
        }
        ofs << '\n';
    }
}



// Only used for debugging
void print_block(vector<vector<double>> &block) {
    for (int x = 0; x < block.size(); x++) {
        for (int y = 0; y < block[0].size(); y++) {
            cout << block[x][y] << ' ';
        }
        cout << endl;
    }
}

// Only used for debugging
void print_block(vector<vector<unsigned char>> &block) {
    for (int x = 0; x < block.size(); x++) {
        for (int y = 0; y < block[0].size(); y++) {
            cout << +block[x][y] << ' ';
        }
        cout << endl;
    }
}