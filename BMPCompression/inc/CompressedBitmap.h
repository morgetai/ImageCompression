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

class BitEncoding
{
public:
    static std::vector<unsigned char> Encode(const std::vector<unsigned char>& data, int width, int height);
    static std::vector<unsigned char> Decode(const std::vector<unsigned char>& data, int width);

private:
    struct Elem
    {
        std::bitset<32> code;
        std::vector<unsigned char> colors;
    };
};

template <typename Comp>
class ImageCompression
{
public:
    enum class Option
    {
        ENCODE = 0,
        DECODE = 1
    };

    ImageCompression(unsigned char *data, int width, int height);
    ImageCompression(std::istream &in);

    bool Encode();
    BitMap Decode();
    void Save(std::ostream &out);
private:
    Option m_current_option;
    std::vector<unsigned char> m_data;
    int m_width;
    int m_height;
};
