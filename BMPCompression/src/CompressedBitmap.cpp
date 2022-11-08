#include "CompressedBitmap.h"

#include <bitset>
#include <array>
#include <cmath>

namespace
{
    constexpr int CODED_BMP_BITS_NUM = 32;
    constexpr int CODED_BMP_BYTES_SIZE = CODED_BMP_BITS_NUM / 8;
    constexpr std::array<unsigned char, 4> WHITE_PIXELS{0xff, 0xff, 0xff, 0xff};
    constexpr std::array<unsigned char, 4> BLACK_PIXELS{0x00, 0x00, 0x00, 0x00};

    using compressed_bmp_bitset = std::bitset<CODED_BMP_BITS_NUM>;

    std::vector<unsigned char> Decode(const char *input_data, const int data_size, const int width)
    {
        const std::vector<unsigned char> empty_line(width, 0xff);

        auto data_end = std::next(input_data, data_size);

        std::vector<unsigned char> res;
        for (auto data_begin = input_data; data_begin != data_end;)
        {
            auto colors = std::next(data_begin, 4);
            std::bitset<32> compressed_bmp = *(reinterpret_cast<const unsigned int *>(data_begin));

            for (int i = 31; i > 0; i -= 2)
            {
                if (!compressed_bmp[i])
                {
                    res.insert(res.end(), WHITE_PIXELS.begin(), WHITE_PIXELS.end());
                }
                else if (compressed_bmp[i] && !compressed_bmp[i - 1])
                {
                    res.insert(res.end(), BLACK_PIXELS.begin(), BLACK_PIXELS.end());
                }
                else if (compressed_bmp[i] && compressed_bmp[i - 1])
                {
                    res.insert(res.end(), colors, std::next(colors, 4));
                    std::advance(colors, 4);
                }
            }

            data_begin = colors;
        }

        return res;
    }
}

std::vector<unsigned char> BitEncoding::Encode(const std::vector<unsigned char>& data, int width, int height)
{
    std::vector<BitEncoding::Elem> res;

    // buffers
    auto end_line = data;

    res.resize(height * width);
    int curr_bit_pos{31};
    int cur_elem{0};

    for (auto current_element = data.begin(); current_element != data.end();)
    {
        auto uint = *reinterpret_cast<const uint32_t *>(current_element.base());

        if (curr_bit_pos < 0)
        {
            curr_bit_pos = 31;
            ++cur_elem;
        }

        if (uint == 0xffffffff)
        {
            curr_bit_pos -= 2;
        }
        else if (uint == 0x00000000)
        {
            auto temp = static_cast<int>(1 << curr_bit_pos);
            res[cur_elem].code |= temp;
            curr_bit_pos -= 2;
        }
        else
        {
            auto temp = static_cast<int>(3 << (curr_bit_pos - 1));
            res[cur_elem].code |= temp;
            curr_bit_pos -= 2;

            res[cur_elem].colors.insert(res[cur_elem].colors.end(),
                                        {static_cast<unsigned char>(uint),
                                         static_cast<unsigned char>(uint >> 8),
                                         static_cast<unsigned char>(uint >> 16),
                                         static_cast<unsigned char>(uint >> 24)});
        }

        current_element += 4;
    }

    std::vector<unsigned char> ret;
    std::for_each(res.begin(), std::next(res.begin(), cur_elem + 1), [&ret](const BitEncoding::Elem &elem)
                  {
        auto u_code = elem.code.to_ulong();

        ret.insert(ret.end(), {static_cast<unsigned char>(u_code),
                                             static_cast<unsigned char>(u_code >> 8),
                                             static_cast<unsigned char>(u_code >> 16),
                                             static_cast<unsigned char>(u_code >> 24)});

        ret.insert(ret.end(),elem.colors.begin(), elem.colors.end()); });

    return ret;
}

std::vector<unsigned char> BitEncoding::Decode(const std::vector<unsigned char>& data, const int width)
{
    const std::vector<unsigned char> empty_line(width, 0xff);

    //auto data_end = std::next(data, width * height);

    std::vector<unsigned char> res;
    for (auto data_begin = data.begin(); data_begin != data.end();)
    {
        auto colors = std::next(data_begin, 4);
        std::bitset<32> compressed_bmp = *(reinterpret_cast<const unsigned int *>(data_begin.base()));

        for (int i = 31; i > 0; i -= 2)
        {
            if (!compressed_bmp[i])
            {
                res.insert(res.end(), WHITE_PIXELS.begin(), WHITE_PIXELS.end());
            }
            else if (compressed_bmp[i] && !compressed_bmp[i - 1])
            {
                res.insert(res.end(), BLACK_PIXELS.begin(), BLACK_PIXELS.end());
            }
            else if (compressed_bmp[i] && compressed_bmp[i - 1])
            {
                res.insert(res.end(), colors, std::next(colors, 4));
                std::advance(colors, 4);
            }
        }

        data_begin = colors;
    }

    return res;
}

template<typename Comp>
CompressedBitMapT<Comp>::CompressedBitMapT(unsigned char *data, int width, int height):
m_current_option(Option::ENCODE),
m_data(data, data + (width * height)),
m_width(width),
m_height(height)
{
}

template<typename Comp>
CompressedBitMapT<Comp>::CompressedBitMapT(std::istream &in)
{
    //BitMap ret;
    in.read(reinterpret_cast<char *>(&m_width), sizeof(int));
    in.read(reinterpret_cast<char *>(&m_height), sizeof(int));
    //m_data.reserve(ret.width * ret.height);
    std::array<char, 300'000> buf{};
    in.read(buf.data(), buf.size());

    if (auto readed_elems = in.gcount(); readed_elems > 10)
    {
        auto decoded = Decode(buf.data(), readed_elems, ret.width);
        m_data.data.insert(m_data.data.end(), buf.begin(), std::next(buf.begin(), readed_elems));
    }
}

template<typename Comp>
bool CompressedBitMapT<Comp>::Encode()
{
    if(m_data.empty() || m_current_option != Option::ENCODE)
    {
        return false;
    }

    m_data = Comp::Encode(m_data,m_width,m_height);

    return true;
}

template<typename Comp>
void CompressedBitMapT<Comp>::Save(std::ostream &out)
{
    out.write(reinterpret_cast<const char *>(&m_width), sizeof(int));
    out.write(reinterpret_cast<const char *>(&m_height), sizeof(int));
    out.write(m_data.data(), sizeof(unsigned char) * line.colors.size());

    out.flush();
}

template<typename Comp>
BitMap CompressedBitMapT<Comp>::Decode()
{
    if(m_data.empty() || m_current_option != Option::DECODE)
    {
        return {};
    }

    return {m_width, m_height, Comp::Decode(m_data,m_width)};
}

CompressedBitMap::CompressedBitMap(unsigned char *data, const int width, const int height) : m_width(width), m_height(height)
{
    if (!data)
    {
        return;
    }

    // buffers
    auto end_line = data;

    m_lines.resize(height * width);
    int curr_bit_pos{31};
    int cur_elem{0};

    for (auto current_element = data; current_element < data + (height * width);)
    {
        auto uint = *reinterpret_cast<uint32_t *>(current_element);

        if (curr_bit_pos < 0)
        {
            curr_bit_pos = 31;
            ++cur_elem;
        }

        if (uint == 0xffffffff)
        {
            curr_bit_pos -= 2;
        }
        else if (uint == 0x00000000)
        {
            auto temp = static_cast<int>(1 << curr_bit_pos);
            m_lines[cur_elem].code |= temp;
            curr_bit_pos -= 2;
        }
        else
        {
            auto temp = static_cast<int>(3 << (curr_bit_pos - 1));
            m_lines[cur_elem].code |= temp;
            curr_bit_pos -= 2;

            m_lines[cur_elem].colors.insert(m_lines[cur_elem].colors.end(),
                                            {static_cast<unsigned char>(uint),
                                             static_cast<unsigned char>(uint >> 8),
                                             static_cast<unsigned char>(uint >> 16),
                                             static_cast<unsigned char>(uint >> 24)});
        }

        current_element += 4;
    }

    m_lines.erase(std::next(m_lines.begin(), cur_elem + 1), m_lines.end());
}

void CompressedBitMap::Save(std::ostream &out)
{
    out.write(reinterpret_cast<const char *>(&m_width), sizeof(int));
    out.write(reinterpret_cast<const char *>(&m_height), sizeof(int));
    // out << '\n';
    const auto empty_line = 0;
    for (auto &line : m_lines)
    {
        const auto val = line.code.to_ulong();
        out.write(reinterpret_cast<const char *>(&val), sizeof(unsigned int));
        out.write(reinterpret_cast<const char *>(line.colors.data()), sizeof(unsigned char) * line.colors.size());
    }

    out.flush();
}

BitMap CompressedBitMap::ReadCompressedFile(std::istream &in)
{
    BitMap ret;
    in.read(reinterpret_cast<char *>(&ret.width), sizeof(int));
    in.read(reinterpret_cast<char *>(&ret.height), sizeof(int));
    ret.data.reserve(ret.width * ret.height);
    std::array<char, 300'000> buf{};
    in.read(buf.data(), buf.size());

    if (auto readed_elems = in.gcount(); readed_elems > 10)
    {
        auto decoded = Decode(buf.data(), readed_elems, ret.width);
        ret.data.insert(ret.data.end(), decoded.begin(), decoded.end());
    }

    ret.data.erase(ret.data.begin() + (ret.width * ret.height), ret.data.end());

    return ret;
}