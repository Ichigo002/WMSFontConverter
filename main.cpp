#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// struct Glyph
// {
//     uint16_t xAdvance; // cursor move after drawing (width+spacing)
//     uint8_t w, h; // width and height of glyph
//     int xOffset, yOffset; // offset from cursor/baseline to top-left of bitmap
// };

/*

Generator font input:

A
01110
10001
10001
11111
10001
10001
10001__________________ baseline;y=0


generator font output:

    RAW BITMAP:
    01110100|01100011|11111000|11000110|00100000

    const uint8_t bitmap[4] = {0x74, 0x63, 0xf8, 0xc6, 0x20};
    const Glyph glyphs[1] = {{0, 5, 5, 7, 0, -7}};

    {5, 5, 7, 0, -7} =>
                byteAddr = 0;
                xAdvance = 5;
                w = 5;
                h = 7;
                xOffset = 0;
                yOffset = -7;

    const int bitmap_length = 5; // length of bitmap table
    const int glyphs_length = 1; // length of glyphs table



*/

void writeBitmapValue(std::ofstream &o, string &binary);

int main()
{
    // cout << "Filename with font to convert: ";
    string fn = "input.txt";
    // cin >> fn;

    std::ifstream in(fn);

    if (!in.good())
    {
        cerr << "Error: Opening file\n";
        return -1;
    }

    std::ofstream bitmap("bitmap-out.cpp");
    std::ofstream glyphs("glyphs-out.cpp");

    if (!glyphs.good() || !bitmap.good())
    {
        cerr << "Error: Can't create new files\n";
        return -2;
    }

    bitmap << "const uint_8t bitmap[] = { ";
    glyphs << "const Glyph glyphs[] = { ";

    int w = 0, h = 0;
    int number_of_glyphs = 0;
    int number_of_bytes = 0;
    bool newc = true;

    string line;
    string buff;
    while (getline(in, line))
    {
        
    }

    bitmap << " };\n";
    bitmap << "size_t bitmap_size = " << number_of_bytes << ";";
    bitmap.close();

    glyphs << "};\n";
    glyphs << "size_t glyph_size = " << number_of_glyphs << ";";
    glyphs.close();

    return 0;
}

void writeBitmapValue(std::ofstream &o, string &binary)
{
    unsigned long long n = std::stoull(binary, nullptr, 2);
    o << "0x" << std::uppercase << std::hex << n;
}