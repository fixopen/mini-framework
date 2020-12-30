#pragma once

#include <array>
#include <iterator>
#include <limits>
#include <cstdint>

namespace utils {
    class md5 {
    private:
        std::uint32_t a0_ = 0x67452301;
        std::uint32_t b0_ = 0xefcdab89;
        std::uint32_t c0_ = 0x98badcfe;
        std::uint32_t d0_ = 0x10325476;

        std::array<std::uint32_t, 16> m_array_;
        std::array<std::uint32_t, 16>::iterator m_array_first_;

        static const std::array<std::uint32_t, 64> k_array_;
        static const std::array<std::uint32_t, 64> s_array_;

    private:
        static constexpr std::uint32_t left_rotate(std::uint32_t x, std::uint32_t c) {
            return (x << c) | (x >> (32 - c));
        }

        template <class OutputIterator>
        static void uint32_to_byte(std::uint32_t n, OutputIterator& out) {
            *out++ = n & 0xff;
            *out++ = (n >> 8) & 0xff;
            *out++ = (n >> 16) & 0xff;
            *out++ = (n >> 24) & 0xff;
        }

        template <class OutputIterator>
        static void uint32_to_hex(std::uint32_t n, OutputIterator& out) {
            static auto const hex_chars = "0123456789abcdef";

            // print nibbles, low byte first (but high nibble before low nibble)
            // so shift is 4, 0, 12, 8, 20, 16, ...
            for (auto i = 0u; i < 32; i += 4) {
                *out++ = hex_chars[(n >> (i ^ 4)) & 0xf];
            }
        }

    private:
        template<class InputIterator>
        static std::uint8_t input_u8(const InputIterator& it) {
            return *it;
        }

        void reset_m_array() {
            m_array_first_ = m_array_.begin();
        }

        template <class InputIterator>
        void bytes_to_m_array(InputIterator & first, std::array<std::uint32_t, 16>::iterator m_array_last) {
            for (; m_array_first_ != m_array_last; ++m_array_first_) {
                //*m_array_first_ = std::uint8_t(*first++);
                //*m_array_first_ |= std::uint8_t(*first++) << 8;
                //*m_array_first_ |= std::uint8_t(*first++) << 16;
                //*m_array_first_ |= std::uint8_t(*first++) << 24;
                *m_array_first_ = input_u8(first++);
                *m_array_first_ |= input_u8(first++) << 8;
                *m_array_first_ |= input_u8(first++) << 16;
                *m_array_first_ |= input_u8(first++) << 24;
            }
        }

        template <class InputIterator>
        void true_bit_to_m_array(InputIterator& first, std::ptrdiff_t chunk_length) {
            switch (chunk_length % 4) {
            case 0:
                *m_array_first_ = 0x00000080;
                break;
            case 1:
                *m_array_first_ = input_u8(first++);
                *m_array_first_ |= 0x00008000;
                break;
            case 2:
                *m_array_first_ = input_u8(first++);
                *m_array_first_ |= input_u8(first++) << 8;
                *m_array_first_ |= 0x00800000;
                break;
            case 3:
                *m_array_first_ = input_u8(first++);
                *m_array_first_ |= input_u8(first++) << 8;
                *m_array_first_ |= input_u8(first++) << 16;
                *m_array_first_ |= 0x80000000;
                break;
            }
            ++m_array_first_;
        }

        void zeros_to_m_array(std::array<std::uint32_t, 16>::iterator m_array_last) {
            for (; m_array_first_ != m_array_last; ++m_array_first_) {
                *m_array_first_ = 0;
            }
        }

        void original_length_bits_to_m_array(std::uint64_t original_length_bits) {
            *m_array_first_++ = static_cast<std::uint32_t>(original_length_bits);
            *m_array_first_++ = original_length_bits >> 32;
        }

        void hash_chunk() {
            std::uint32_t A = a0_;
            std::uint32_t B = b0_;
            std::uint32_t C = c0_;
            std::uint32_t D = d0_;

            std::uint32_t F;
            unsigned int g;

            for (unsigned int i = 0; i < 64; ++i) {
                if (i < 16) {
                    F = (B & C) | ((~B) & D);
                    g = i;
                } else if (i < 32) {
                    F = (D & B) | ((~D) & C);
                    g = (5 * i + 1) & 0xf;
                } else if (i < 48) {
                    F = B ^ C ^ D;
                    g = (3 * i + 5) & 0xf;
                } else {
                    F = C ^ (B | (~D));
                    g = (7 * i) & 0xf;
                }

                std::uint32_t D_temp = D;
                D = C;
                C = B;
                B += left_rotate(A + F + k_array_[i] + m_array_[g], s_array_[i]);
                A = D_temp;
            }

            a0_ += A;
            b0_ += B;
            c0_ += C;
            d0_ += D;
        }

    public:
        template<class InputIterator>
        typename std::enable_if<std::numeric_limits<typename InputIterator::value_type>::digits <= 8>::type
            update(InputIterator first, InputIterator last) {
            std::uint64_t original_length_bits = std::distance(first, last) * 8;

			std::ptrdiff_t chunk_length;
			for (chunk_length = std::distance(first, last); chunk_length >= 64; chunk_length = std::distance(first, last)) {
                reset_m_array();
                bytes_to_m_array(first, m_array_.end());
                hash_chunk();
            }

            reset_m_array();
            bytes_to_m_array(first, m_array_.begin() + chunk_length / 4);
            true_bit_to_m_array(first, chunk_length);

            if (chunk_length >= 56) {
                zeros_to_m_array(m_array_.end());
                hash_chunk();

                reset_m_array();
                zeros_to_m_array(m_array_.end() - 2);
                original_length_bits_to_m_array(original_length_bits);
                hash_chunk();
            } else {
                zeros_to_m_array(m_array_.end() - 2);
                original_length_bits_to_m_array(original_length_bits);
                hash_chunk();
            }
        }

    public:
        md5() {}

        template<class InputIterator>
        md5(InputIterator first, InputIterator last) {
            update(first, last);
        }

        template<class Container>
        Container digest() const {
            Container c;
            digest(c);
            return c;
        }

        template<class Container>
        Container hex_digest() const {
            Container c;
            hex_digest(c);
            return c;
        }

        template <class Container>
        void digest(Container& container) const {
            container.resize(16);
            auto it = container.begin();

            uint32_to_byte(a0_, it);
            uint32_to_byte(b0_, it);
            uint32_to_byte(c0_, it);
            uint32_to_byte(d0_, it);
        }

        template <class Container>
        void hex_digest(Container & container) const {
            container.resize(32);
            auto it = container.begin();

            uint32_to_hex(a0_, it);
            uint32_to_hex(b0_, it);
            uint32_to_hex(c0_, it);
            uint32_to_hex(d0_, it);
        }
    };
}

/*
#include <iostream>

int main() {
    std::string data = "Hello World";
    std::string data_hex_digest;
    md5 hash;
    hash.update(data.begin(), data.end());
    hash.hex_digest(data_hex_digest);
    std::cout << data_hex_digest << std::endl;

    std::string data = "Hello World";
    const md5 hash(data.begin(), data.end());
    const auto data_hex_digest = hash.hex_digest<std::string>();
    std::cout << data_hex_digest << std::endl;
}
*/
