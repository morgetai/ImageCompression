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

template<typename Comp>
ImageCompression<Comp>::ImageCompression(unsigned char *data, int width, int height):
m_current_option(Option::ENCODE),
m_data(data, data + (width * height)),
m_width(width),
m_height(height)
{
}

template<typename Comp>
ImageCompression<Comp>::ImageCompression(std::istream &in)
{
    m_current_option = Option::DECODE;
    in.read(reinterpret_cast<char *>(&m_width), sizeof(int));
    in.read(reinterpret_cast<char *>(&m_height), sizeof(int));

    std::vector<unsigned char>  buf(300'000);
    in.read(reinterpret_cast<char*>(buf.data()), buf.size());

    if (auto readed_elems = in.gcount(); readed_elems > 10)
    {
        m_data.insert(m_data.end(), buf.begin(), std::next(buf.begin(), readed_elems));
    }
}

template<typename Comp>
bool ImageCompression<Comp>::Encode()
{
    if(m_data.empty() || m_current_option != Option::ENCODE)
    {
        return false;
    }

    m_data = Comp::Encode(m_data,m_width,m_height);

    return true;
}

template<typename Comp>
void ImageCompression<Comp>::Save(std::ostream &out)
{
    if(m_data.empty() || m_current_option != Option::ENCODE)
    {
        return;
    }

    out.write(reinterpret_cast<const char *>(&m_width), sizeof(int));
    out.write(reinterpret_cast<const char *>(&m_height), sizeof(int));
    out.write(reinterpret_cast<const char *>(m_data.data()), sizeof(unsigned char) * m_data.size());

    out.flush();
}

template<typename Comp>
BitMap ImageCompression<Comp>::Decode()
{
    if(m_data.empty() || m_current_option != Option::DECODE)
    {
        return {};
    }

    auto decode = Comp::Decode(m_data,m_width);
    decode.erase(std::next(decode.begin(),m_width * m_height),decode.end());

    return {m_width, m_height, std::move(decode)};
}
