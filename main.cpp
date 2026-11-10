#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

// struct Glyph
// {
//     uint8_t w, h; // width and height of glyph
//     int yOffset; // offset from cursor/baseline to top-left of bitmap
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
                w = 5;
                h = 7;
                yOffset = -7;

    const int bitmap_length = 5; // length of bitmap table
    const int glyphs_length = 1; // length of glyphs table



*/

// returns: number of bytes which binary taken
int writeBytesToBitmap(std::ofstream &o, string &binary);

void writeBitmapValue(std::ofstream &o, string &binary);

void writeGlyph(std::ofstream &o, int byteAddr, int w, int h, int xOffset)
{
    o << "{ " << byteAddr << ", " << w << ", " << h << ", " << xOffset << "}";
}

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

    bitmap << "const uint8_t bitmap[] = {\n    ";
    glyphs << "const Glyph glyphs[] = {\n    ";

    int w = 0, h = 0;
    int maxh = 0;
    int number_of_glyphs = 0;
    int number_of_bytes = 0;

    int xOffset = 0;
    bool first_loop = true;

    string line;
    string buff = "";
    char which;
    while (getline(in, line))
    {
        if (line.length() == 1 || line == "END") // Start of reading character.
        {

            if (first_loop)
            {
                first_loop = false;
                continue;
            }

            if (number_of_bytes != 0)
                bitmap << ", \n    ";

            int bytes = writeBytesToBitmap(bitmap, buff);

            number_of_bytes += bytes;

            if (xOffset == 0)
            {
                cerr << "Invalid format: xOffset = 0. Not found baseline of character. \nAppend '=' at the end of line which is baseline";
                return -4;
            }

            if (buff.length() != w * h)
            {
                cerr << "Invlid format: w0 != w1. Rows has different lengths.\n character: '" << which << "'";
                return -3;
            }

            if (number_of_glyphs != 0)
                glyphs << ", \n    ";

            writeGlyph(glyphs, number_of_bytes - bytes, w, h, xOffset);

            if (xOffset < maxh)
                maxh = xOffset;

            which = line[0];
            h = w = xOffset = 0;
            buff = "";
            number_of_glyphs++;
            continue;
        }

        if (line[line.length() - 1] == '=')
        {
            line.pop_back();
            xOffset = -h - 1;
        }

        if (w == 0)
            w = line.length();

        buff += line;
        h++;
    }

    bitmap << std::dec;
    bitmap << " };\n";
    bitmap << "const int bitmap_size = " << number_of_bytes << ";";
    bitmap.close();

    glyphs << "};\n";
    glyphs << "const int glyph_size = " << number_of_glyphs << ";\n";
    glyphs << "const int max_Yoffset = " << maxh << ";\n";
    glyphs.close();

    return 0;
}

int writeBytesToBitmap(std::ofstream &o, string &binary)
{
    int s = ceil(binary.length() / 8.0f);

    string buff = "";
    for (size_t i = 0; i < s; i++)
    {
        buff = binary.substr(i * 8, 8);

        if (buff.length() < 8)
        {
            int t = 8 - buff.length();
            for (size_t j = 0; j < t; j++)
            {
                buff += '0';
            }
        }

        if (i != 0)
            o << ", ";

        writeBitmapValue(o, buff);
    }

    return s;
}

void writeBitmapValue(std::ofstream &o, string &binary)
{
    unsigned long long n = std::stoull(binary, nullptr, 2);
    o << "0x" << std::uppercase << std::hex << n;
}