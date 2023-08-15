#pragma once

#include <vector>
#include <string>

class UncompressedImageData {
    public:
        struct Header {
            std::string P5;
            int x_size;
            int y_size;
            int maxval;
        } header;
        
        std::vector<std::vector<unsigned char>> image_data;  
        
        UncompressedImageData();
        UncompressedImageData(std::vector<unsigned char>);

        void extract_file_fields(std::vector<unsigned char> &raw_data);
        void print_image_data();
};