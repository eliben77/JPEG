#include <vector>
using namespace std;
typedef unsigned char  uint8;
typedef signed short   int16;
typedef signed int     int32;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned int   uint;
class BitBuffer {
private:
    vector<uint16> m_buffer; // The buffer to hold the bits (8-bit words)
    uint16 sum_bits_in;              // Number of bits currently in the buffer

public:
    BitBuffer() : sum_bits_in(0) {}

    uint16 get_sum_bits_in() const {
        return this->sum_bits_in;
    }
    /// 1 1 1 1 1 1 1 1
    /// 1 1 1 1 1 1 1 1

    // Function to add bits to the buffer
    void addBits(uint16 bits, uint16 num_bits) {
        uint16 current_index = sum_bits_in / 16; // Each index represents an 8-bit word (byte)
        uint16 bit_offset = sum_bits_in % 16;    // Bit offset within the current 8-bit word

        if (bit_offset + num_bits > 16) {
            //111110000000000
            uint16 remaining_bits = num_bits - (16 - bit_offset);

            m_buffer.resize(current_index + 2); // Increase the buffer size if necessary
            m_buffer[current_index] |= (bits >> remaining_bits);
            m_buffer[current_index + 1] = (bits << (16 - remaining_bits));
        } else {
            m_buffer.resize(current_index + 1); // Increase the buffer size if necessary
            m_buffer[current_index] |= (bits << (16 - bit_offset - num_bits));
        }

        sum_bits_in += num_bits;
    }

    // Function to check if the buffer has at least n bits available
    bool hasBits(uint16 num_bits) const {
        return sum_bits_in >= num_bits;
    }

    // Function to retrieve n bits from the buffer
    uint16 getBits(int num_bits) {
        uint16 result = 0;

        uint16 current_index = sum_bits_in / 16;
        uint16 bit_offset = sum_bits_in % 16;
        if (num_bits == 16){
            result = m_buffer[0];
            m_buffer.erase(m_buffer.begin()); // Remove the first element
            sum_bits_in -= num_bits;
            return result;
        }
        if (num_bits == bit_offset && bit_offset == sum_bits_in){
            result = m_buffer[0] >> (16 - bit_offset);
            m_buffer.erase(m_buffer.begin()); // Remove the first element
            sum_bits_in -= num_bits;
            return result;
        }
        if (bit_offset + num_bits > 16) {
            uint16 bits1 = m_buffer[current_index] >> bit_offset;
            uint16 bits2 = m_buffer[current_index + 1] << (16 - bit_offset);
            result = (bits1 | bits2);
        } else {
            result = (m_buffer[current_index] >> (16 - bit_offset - num_bits));
        }

        sum_bits_in -= num_bits;
        return result;
    }
};