#include "gtest/gtest.h"
#include <fstream>
#ifndef UTIL
    #include "Util.h"
#endif
#ifndef UNCOMPRESSED_IMAGE_DATA
    #include "UncompressedImageData.h"
#endif

std::vector<unsigned char> get_raw_data() {
    std::ifstream input_file("../tst/test0.pgm");    // a particular argument (that needs to be vetted)
    char character; 
    std::vector <unsigned char> data;
   
    while (input_file.good()) {
        input_file.get(character);
        data.push_back((unsigned char) character);
    }
    return data;
}

TEST(UncompressedImageDataTest, P5HeaderName) {
    std::vector<unsigned char> raw_data = get_raw_data();
    UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    EXPECT_EQ("P5", uncompressedImageData->header.P5);
}

TEST(UncompressedImageDataTest, P5IncorrectHeaderName) {
    std::vector<unsigned char> raw_data = get_raw_data();
    raw_data[0] = 'X';
    EXPECT_THROW(UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data), std::runtime_error);
    try {
        UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    } catch (const std::runtime_error& ia) {
        EXPECT_STREQ("Invalid header P5", ia.what());
    }
}

TEST(UncompressedImageDataTest, P5ShortHeaderName) {
    std::vector<unsigned char> raw_data = get_raw_data();
    raw_data.erase(raw_data.begin(), raw_data.begin() + 1);
    EXPECT_THROW(UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data), std::runtime_error);
    try {
        UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    } catch (const std::runtime_error& ia) {
        EXPECT_STREQ("Invalid header P5", ia.what());
    }
}

TEST(UncompressedImageDataTest, P5LongHeaderName) {
    std::vector<unsigned char> raw_data = get_raw_data();
    raw_data.insert(raw_data.begin(), 'X');
    EXPECT_THROW(UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data), std::runtime_error);
    try {
        UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    } catch (const std::runtime_error& ia) {
        EXPECT_STREQ("Invalid header P5", ia.what());
    }
}

TEST(UncompressedImageDataTest, valid_x_size) {
    std::vector<unsigned char> raw_data = get_raw_data();
    UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    EXPECT_EQ(16, uncompressedImageData->header.x_size);
}

TEST(UncompressedImageDataTest, invalid_x_size) {
    std::vector<unsigned char> raw_data = get_raw_data();
    raw_data[3] = 'Z';
    EXPECT_THROW(UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data), std::runtime_error);
    try {
        UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    } catch (const std::runtime_error& ia) {
        EXPECT_STREQ("Invalid x size", ia.what());
    }
}

// // could do a lot more parsing here to get the error to happen in extract_file_fields
// // instaed the error happens on return from that function. stoi is still called, but we check the values
// // in the constructor.
TEST(UncompressedImageDataTest, missing_x_size) {
    std::vector<unsigned char> raw_data = get_raw_data();
    raw_data.erase(raw_data.begin() + 3, raw_data.begin() + 5);
    EXPECT_THROW(UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data), std::runtime_error);
    try {
        UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    } catch (const std::runtime_error& ia) {
        EXPECT_STREQ("Invalid x size", ia.what());
    }
}
    // std::cout << raw_data[0] << raw_data[1] << raw_data[2] << raw_data[3] << raw_data[4] << std::endl;
TEST(UncompressedImageDataTest, valid_y_size) {
    std::vector<unsigned char> raw_data = get_raw_data();
    UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    EXPECT_EQ(16, uncompressedImageData->header.y_size);
}

TEST(UncompressedImageDataTest, invalid_y_size) {
    std::vector<unsigned char> raw_data = get_raw_data();
    raw_data[6] = 'X';
    EXPECT_THROW(UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data), std::runtime_error);
    try {
        UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    } catch (const std::runtime_error& ia) {
        EXPECT_STREQ("Invalid y size", ia.what());
    }
}

// // the comment above is connected to this test: we simply don't initialize y_size and the error happens in the constructor, not
// // the helper function
TEST(UncompressedImageDataTest, missing_y_size) {
    std::vector<unsigned char> raw_data = get_raw_data();
    raw_data.erase(raw_data.begin() + 6, raw_data.begin() + 8);
    EXPECT_THROW(UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data), std::runtime_error);
    try {
        UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    } catch (const std::runtime_error& re) {
        EXPECT_STREQ("Invalid y size", re.what());
    }
}

TEST(UncompressedImageDataTest, valid_maxval) {
    std::vector<unsigned char> raw_data = get_raw_data();
    UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    EXPECT_EQ(255, uncompressedImageData->header.maxval);
}

TEST(UncompressedImageDataTest, invalid_maxval) {
    std::vector<unsigned char> raw_data = get_raw_data();
    raw_data[8] = 'X';
    EXPECT_THROW(UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data), std::runtime_error);
    try {
        UncompressedImageData *uncompressedImageData = new UncompressedImageData(raw_data);
    } catch (const std::runtime_error& ia) {
        EXPECT_STREQ("Invalid maxval", ia.what());
    }
}

// // TODO tests on data. These are so be we need a helper function to bring in a real file.
// TEST(UncompressedImageDataTest, valid_data) {
//     try {
//         UncompressedImageData *unc = read_image_file("../tst/test0.pgm");
//     } catch (...) {
//         // There should not have been an error here
//     }
// }