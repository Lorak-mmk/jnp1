#ifndef FIBO_H
#define FIBO_H

#include <boost/dynamic_bitset.hpp>
#include <boost/operators.hpp>
#include <ostream>
#include <string>

class Fibo : boost::addable<Fibo>,
             boost::totally_ordered<Fibo>,
             boost::bitwise<Fibo>,
             boost::left_shiftable<Fibo, size_t> {
    template <std::size_t frame_size>
    using frame_pair = std::pair<std::array<uint8_t, frame_size>, std::array<uint8_t, frame_size>>;

   public:
    Fibo();

    explicit Fibo(const std::string& number_string);

    explicit Fibo(const char* number);

    template <typename T,
              typename = std::enable_if_t<std::is_integral<T>::value && !std::is_same<bool, T>::value &&
                                          !std::is_same<signed char, T>::value && !std::is_same<char, T>::value &&
                                          !std::is_same<unsigned char, T>::value && !std::is_same<wchar_t, T>::value &&
                                          !std::is_same<char16_t, T>::value && !std::is_same<char32_t, T>::value>>
    Fibo(T number) : number_data(get_fib_representation(number)) {
        assert(number >= 0);
        normalize();
    }

    Fibo(const Fibo& other_fibo);

    /**
     * Compare fibo numbers by comparing their lengths and bits set
     * @param left to be compared
     * @param right to be compared
     * @return true when left < right, otherwise false
     */
    friend bool operator<(const Fibo& left, const Fibo& right);

    /**
     * Check equality for fibo numbers
     * @param left to be compared
     * @param right to be compared
     * @return true when left == right, otherwise false
     */
    friend bool operator==(const Fibo& left, const Fibo& right);

    /**
     * Assign fibo number to this fibo number
     * @param right number to be assigned
     * @return this fibo number after assignment
     */
    Fibo& operator=(const Fibo& right);

    /**
     * Add second fibo number to current in linear time of number of bits
     * @param right number to be added
     * @return fibo result of addition
     */
    Fibo& operator+=(const Fibo& right);

    /**
     * Do AND bit operation on fibo bits with right, aligning to LSB
     * @param right fibo number to do AND operation
     * @return this & right
     */
    Fibo& operator&=(const Fibo& right);

    /**
     * Do OR bit operation on fibo bits with right, aligning to LSB
     * @param right fibo number to do OR operation
     * @return this | right
     */
    Fibo& operator|=(const Fibo& right);

    /**
     * Do XOR bit operation on fibo bits with right, aligning to LSB
     * @param right fibo number to do XOR operation
     * @return this ^ right
     */
    Fibo& operator^=(const Fibo& right);

    /**
     * Left shift fibo number representation
     * @param n number of places to shift fibo number
     * @return shifted fibo number
     */
    Fibo& operator<<=(const size_t& n);

    /**
     * Print fibo number bits to std::ostream starting from MSB
     * @param stream to be printed data on
     * @param data to be printed to stream
     * @return stream on which data is printed
     */
    friend std::ostream& operator<<(std::ostream& stream, const Fibo& data);

    /**
     * Get the length of fibo bits representation
     * @return number of bits in fibo number (for 0 returns 1)
     */
    size_t length() const;

   private:
    /**
     * Field that keeps the normed representation of number in Fibonacci system
     */
    boost::dynamic_bitset<> number_data;

    /**
     * Normalizes representation stored in number data
     * After calling, highest bit stored in number_data will be 1
     * (in other words, number_data will be shrink to fit number exactly)
     * Also, ther won't be two 1 bits next to each other
     */
    void normalize();

    /**
     * Helper function for normalize. Normalizes backwards from given index.
     * Fixes situations like 101011, which normalizes changes to 101100 at some point.
     * This function will change it to 1000000.
     * @param index to start the normalization from
     */
    void normalize_backwards(size_t index);

    /**
     * @return index of highest bit in number_data that is set. number_data.size() if no bit is set.
     */
    size_t highest_set_bit_idx() const;

    /**
     * Do the bitwise operation with specified by function operation
     * @param right second number for operation
     * @param op operator to do the bitwise operation
     */
    void bitwise_operator(const Fibo& right, const std::function<bool(bool, bool)>& op);

    /*
     * ****STATIC METHODS AND MEMBERS ****
     */

    /**
     * Static vector of Fibonacci numbers that are smaller than the biggest value of uint64_t
     */
    static const std::vector<uint64_t> computed_fib_numbers;
    
    static const std::array<frame_pair<4>, 12> ADDITION_FRAME_PAIRS;

    /**
     * Calculate and return vector of Fibonacci numbers.
     * @param limit generate numbers not bigger than this
     * @return vector with calculated numbers
     */
    static std::vector<uint64_t> calculcate_fib_numbers(uint64_t limit);

    /**
     * Getting the Fibonacci number of specified index. Assert that the index
     * is not greater than the index of biggest Fibonacci number that is smaller than
     * the biggest value of uint64_t
     * @param nth index of Fibonacci number to be returned
     * @return Fibonacci nth number
     */
    static uint64_t fib_number(uint64_t nth);

    /**
     * Get the biggest index of Fibonacci number that is smaller or equal to specified number.
     * If cannot find the number in uint64_t range than the assertion fails.
     * @param number from which the Fibonacci number index is searched
     * @return index of described Fibonacci number
     */
    static uint64_t index_of_fib_less_or_equal(uint64_t number);

    /**
     * Generate the maybe not normed number representation in Fibonacci system
     * @param number of which Fibonacci system representation is calculated
     * @return boost::dynamic_bitset<> representation of number in Fibonacci system
     */
    static boost::dynamic_bitset<> get_fib_representation(uint64_t number);

    /**
     * At specified position of data vector which is the result of addition after modifications so can contain 0, 1, 2,
     * 3 try to check if the values starting from index to index - frame_size + 1 are equal to the values specified by
     * array and if yes then change the values in the data vector to specified ones by another array frame
     * @tparam frame_size size of the frame to be exchanged
     * @param data in which frame is exchanged (data.size() >= frame_size)
     * @param index at which the frame starts from left (index < data.size())
     * @param from array of values to be changed as frame
     * @param to array of values to which the frame is changed on success validation
     */
    template <std::size_t frame_size>
    static void try_exchange_single_frame(std::vector<uint8_t>& data, size_t index,
                                          const std::array<uint8_t, frame_size>& from,
                                          const std::array<uint8_t, frame_size>& to);

    /**
     * Try to exchange frames at all possible positions in specified direction in the way described in
     * try_exchange_single_frame
     * @tparam frame_size size of frame to be exchanged
     * @tparam frames number of frames that can be exchanged
     * @param data in which frames are exchanged
     * @param from_left true whe exchange from MSB, else exchange from LSB
     * @param frame_pairs array of pairs of frames that should be exchanged if found in data
     */
    template <std::size_t frame_size, std::size_t frames>
    static void exchange_in_frames(std::vector<uint8_t>& data, bool from_left,
        const std::array<std::pair<std::array<uint8_t, frame_size>, std::array<uint8_t, frame_size>>, frames>& frame_pairs);
};

/**
 * @brief Get the constant value of number zero in Fibonacci system
 * @return reference to constant Fibonacci number equal to 0
 */
const Fibo& Zero();

/**
 * @brief Get the constant value of number one in Fibonacci system
 * @return reference to constant Fibonacci number equal to 1
 */
const Fibo& One();

#endif  // FIBO_H
