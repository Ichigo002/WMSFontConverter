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

        if (line.empty())
        {
            // new character
            newc = true;

            number_of_glyphs++;
            /*saving!!*/

           // bitmap << buff;
            writeBitmapValue(bitmap, buff);

            buff = "";
            h = w = 0;

            continue;
        }
        if (newc)
        {
            newc = false;
            glyphs << line[0] << ";\n";

            continue;
        }

        int cut_l, cut_r;
        for (size_t i = line.length() - 1; i >= 0 ; i--)
        {
            if(line[i] == '1')
            {
                cut_r = i+1;
                break;
            }
        }
        for (size_t i = 0; i < line.length(); i++)
        {
            if(line[i] == '1')
            {
                cut_l = i;
                break;
            }
        }
        buff += line.substr(cut_l, line.length() - (line.length() - cut_r) - cut_l);

        h++;
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