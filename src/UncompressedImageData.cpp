#ifndef UTIL
    #include "Util.h"
#endif
#ifndef UNCOMPRESSED_IMAGE_DATA2
    #include "UncompressedImageData.h"
#endif
#include <stdlib.h>
#include <iostream>

using namespace std;

UncompressedImageData::UncompressedImageData(vector<unsigned char> raw_data) {
    this->header.P5 = "";
    this->header.x_size = 0;
    this->header.y_size = 0;
    this->header.maxval = 0;

    extract_file_fields(raw_data);

    if (this->header.P5 != "P5") {
        throw runtime_error("Invalid header P5");
    }
    if (this->header.x_size == 0) {
        throw runtime_error("Invalid x size");
    }
    if (this->header.y_size == 0) {
        throw runtime_error("Invalid y size");
    }
    if (this->header.maxval != MAXVAL) {
        throw runtime_error("Invalid maxval");
    }
}

void UncompressedImageData::extract_file_fields(vector<unsigned char> &raw_data) {
    string file_type;
    string _x_size;
    string _y_size;
    string _maxval;
    string data;
    string::size_type sz;
    int line_count = 0;
    int i;
    for (i = 0; i < raw_data.size(); i++) {
        if (raw_data[i] == '\n') 
            line_count++;
        if (line_count == 0 && raw_data[i] != ' ') {
            file_type.push_back(raw_data[i]);
        }
        if (line_count == 1) {
            while ((i < raw_data.size()) && (raw_data[i] != ' ')) {
                _x_size.push_back(raw_data[i]);
                i++;
            }
            if (!_x_size.empty()) {
                try {
                    this->header.x_size = stoi(_x_size, &sz);
                } catch (...) {
                    throw(std::runtime_error("Invalid x size"));
                }
            }
            i++; // skip the space
            while ((i < raw_data.size()) && (raw_data[i] != '\n')) {
                _y_size.push_back(raw_data[i]);
                i++;
            }
            if (!_y_size.empty()) {
                try {
                    this->header.y_size = stoi(_y_size, &sz);
                } catch (...) {
                    throw(std::runtime_error("Invalid y size"));
                }
            }
            line_count++;
            continue;
        }
        if (line_count == 2) {
            while ((i < raw_data.size()) && (raw_data[i] != '\n')) {
                _maxval.push_back(raw_data[i]);
                i++;
            }
            line_count++;
            continue;
        }
        if (line_count == 3) {
            for (int x = 0; x < this->header.x_size; x++) {
                this->image_data.push_back(vector<unsigned char>());
                for (int y = 0; y < this->header.y_size; y++, i++) {
                    this->image_data[x].push_back(raw_data[i]);
                }
            }
        }
    }
    if (!file_type.empty()) {
        this->header.P5 = file_type;
    } 
    if (!_maxval.empty()) {
        try {
            this->header.maxval = stoi(_maxval, &sz);
        } catch (...) {
            throw(std::runtime_error("Invalid maxval"));
        }
    }
    if ((this->image_data.size() > this->header.x_size) || 
        (this->image_data.empty() && this->image_data[0].size() < this->header.y_size)) {
        for (int i = 0; i < this->header.x_size; i++) {
            this->image_data[i].resize(this->header.y_size);
        }
        this->image_data.resize(this->header.x_size);
    }
}

void UncompressedImageData::print_image_data() {
    for (int x = 0; x < this->header.x_size; x++) {
        for (int y = 0; y < this->header.y_size; y++) {
            cout << +this->image_data[x][y] << ' '; // the `+` promotes to int which helps read things as ASCII
        }
        cout << endl;
    }
}
