#include "CompressedBitmap.h"

#include <array>
#include <cmath>
#include <memory>

namespace
{
    constexpr std::array<unsigned char, 4> WHITE_PIXELS{0xff, 0xff, 0xff, 0xff};
    constexpr std::array<unsigned char, 4> BLACK_PIXELS{0x00, 0x00, 0x00, 0x00};
}

std::vector<unsigned char> BitEncoding::Encode(const std::vector<unsigned char> &data, int width, int height)
{
    std::vector<BitEncoding::Elem> res;

    // buffers
    res.resize(height * width);
    int curr_bit_pos{31};
    int cur_elem{0};

    for (auto current_element = data.begin(); current_element < data.end();)
    {
        auto uint = *reinterpret_cast<const uint32_t *>(std::addressof(*current_element));

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

std::vector<unsigned char> BitEncoding::Decode(const std::vector<unsigned char> &data, const int width)
{
    const std::vector<unsigned char> empty_line(width, 0xff);

    std::vector<unsigned char> res;
    for (auto data_begin = data.begin(); data_begin < data.end();)
    {
        auto colors = std::next(data_begin, 4);
        std::bitset<32> compressed_bmp = *(reinterpret_cast<const unsigned int *>(std::addressof(*data_begin)));
        auto num = compressed_bmp.to_ulong();
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