#pragma once
#include <vector>
#include <iostream>
#include <array>

struct BitMap
{
    int width{0};
    int height{0};
    std::vector<unsigned char> data;
};

class CompressedBitMap
{
public:
    CompressedBitMap(unsigned char *data, int width, int height);

    void Save(std::ostream& out);
    static BitMap ReadCompressedFile(std::istream& in);

private:

    struct Elem
    {
        std::bitset<32> code;
        std::vector<unsigned char> colors;
    };

    std::vector<Elem> m_lines;
    int m_width;
    int m_height;
};
