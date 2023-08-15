#include "gtest/gtest.h"
#include "Util.h"

using namespace std;

/*
    x . . x . . . .
    . . . . . . . .
    . . . . . . . .
    x . . x . . . .
    . . . . . . . .
    . . . . . . . .
    . . . . . . . .
    . . . . . . . .
*/
TEST(UtilTest, copy_block) {
    vector<vector<unsigned char>> a(16, vector<unsigned char>(16, '0'));
    vector<vector<unsigned char>> b(4, vector<unsigned char>(4, '1'));
    vector<vector<unsigned char>> c(4, vector<unsigned char>(4, '0'));

    copy_block(a, b, 0, 0);

    for (int x = 0; x < b.size(); x++) {
        for (int y = 0; y < b.size(); y++) {
            EXPECT_EQ(b[x][y], c[x][y]); // loop on the elements   
        }
    }   
}

/*
    . . . . . . . .
    . . . . . . . .
    . . . . . . . .
    . . . . . . . .
    . . . . x . . x
    . . . . . . . .
    . . . . . . . .
    . . . . x . . x
*/
TEST(UtilTest, copy_block_bottom_right) {
    vector<vector<unsigned char>> a(8, vector<unsigned char>(8)); 
    vector<vector<unsigned char>> b(4, vector<unsigned char>(4));
    vector<vector<unsigned char>> c(4, vector<unsigned char>(4));

    for (int x= 0; x < a.size(); x++) {
        for (int y = 0; y < a.size(); y++) {          
            int target = x * a.size() + y;          
            switch (target) {
                case 36: 
                    a[x][y] = 'x';
                    break;
                case 39:
                    a[x][y] = 'x';
                    break;
                case 60:
                    a[x][y] = 'x';
                    break;
                case 63:
                    a[x][y] = 'x';
                    break;
                default:
                    a[x][y] = 'a';
            }
        }
    }   
 
    for (int x= 0; x < c.size(); x++) {
        for (int y = 0; y < c.size(); y++) {
            int target = x * c.size() + y;          
            switch (target) {
                case 0: 
                    c[x][y] = 'x';
                    break;
                case 3:
                    c[x][y] = 'x';
                    break;
                case 12:
                    c[x][y] = 'x';
                    break;
                case 15:
                    c[x][y] = 'x';
                    break;
                default:
                    c[x][y] = 'a';
            }
        }
    }
    copy_block(a, b, 4, 4);

    for (int x = 0; x < b.size(); x++) {
        for (int y = 0; y < b.size(); y++) {
            EXPECT_EQ(b[x][y], c[x][y]); // loop on the elements   
        }
    }   
}

/*
    . . . . . . . .
    . . . . . . . .
    . . x . . x . .
    . . . . . . . .   
    . . . . . . . . 
    . . x . . x . .   
    . . . . . . . .
    . . . . . . . .
*/
TEST(UtilTest, copy_block_center) {
    vector<vector<unsigned char>> a(8, vector<unsigned char>(8)); 
    vector<vector<unsigned char>> b(4, vector<unsigned char>(4));
    vector<vector<unsigned char>> c(4, vector<unsigned char>(4));

    for (int x= 0; x < a.size(); x++) {
        for (int y = 0; y < a.size(); y++) {          
            int target = x * a.size() + y;          
            switch (target) {
                case 18: 
                    a[x][y] = 'x';
                    break;
                case 21:
                    a[x][y] = 'x';
                    break;
                case 42:
                    a[x][y] = 'x';
                    break;
                case 45:
                    a[x][y] = 'x';
                    break;
                default:
                    a[x][y] = 'a';
            }
        }
    }   
 
    for (int x= 0; x < c.size(); x++) {
        for (int y = 0; y < c.size(); y++) {
            int target = x * c.size() + y;          
            switch (target) {
                case 0: 
                    c[x][y] = 'x';
                    break;
                case 3:
                    c[x][y] = 'x';
                    break;
                case 12:
                    c[x][y] = 'x';
                    break;
                case 15:
                    c[x][y] = 'x';
                    break;
                default:
                    c[x][y] = 'a';
            }
        }
    }
    copy_block(a, b, 2, 2);

    for (int x = 0; x < b.size(); x++) {
        for (int y = 0; y < b.size(); y++) {
            EXPECT_EQ(b[x][y], c[x][y]); // loop on the elements   
        }
    }   
}

TEST(dctTest, firstTrail) {
    // initializer list
    vector<vector<double>> d(BLOCK_X, vector<double>(BLOCK_Y));
    vector<vector<double>> f(BLOCK_X, vector<double>(BLOCK_Y));
    // vector<vector<int>> temp(BLOCK_X,vector<int>(BLOCK_Y));
    for (int i = 0; i < BLOCK_X; i++) {
        for (int j = 0; j < BLOCK_Y; j++) {
            d[i][j] = 255;
        }
    }
    dct(d,f);
    EXPECT_DOUBLE_EQ(f[0][0],2040);
}


// TODO
// TEST
//     vector<vector<int>> index_lookup = {{0, 1, 5, 6, 14, 15, 27, 28}, {2, 4, 7, 13, 16, 26, 29, 42}, {3, 8, 12, 17, 25, 30, 41, 43},
//                                         {9, 11, 18, 24, 31, 40, 44, 53}, {10, 19, 23, 32, 39, 45, 52, 54}, {20, 22, 33, 38, 46, 51, 55, 60},
//                                         {21, 34, 37, 47, 50, 56, 59, 61}, {35, 36, 48, 49, 57, 58, 62, 63}};

//     vector<vector<int>> v = {{0,1,2,3,4,5,6,7},{8,9,10,11,12,13,14,15},{16,17,18,19,20,21,22,23},{24,25,26,27,28,29,30,31},
//                                    {32,33,34,35,36,37,38,39},{40,41,42,43,44,45,46,47},{48,49,50,51,52,53,54,55},{56,57,58,59,60,61,62,63}};
    

    
//     int temp;
//     for (int x = 0; x < index_lookup.size(); x++) {
//         for (int y = 0; y < index_lookup[0].size(); y++) {
//             int posn = x * 8 + y;
//             int i, j; // TODO: Can this not be n^2?
//             for (i = 0; i < index_lookup.size() - 1; i++) {
//                 for (j = 0; j < index_lookup[0].size() - 1; j++) {
//                     if (index_lookup[i][j] == posn) {
//                         break;
//                     }
//                 }
//                 if (index_lookup[i][j] == posn) {
//                     break;
//                 }
//             }
//             temp = v[x][y];
//             v[x][y] = v[i][j];
//             v[j][i] = temp;
//         }
//     }