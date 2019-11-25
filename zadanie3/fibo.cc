#include "fibo.h"

#include <cassert>
#include <utility>
#include <vector>

using Positive = uint64_t;
using Bitset = boost::dynamic_bitset<>;

/*
 * ****PUBLIC METHODS AND MEMBERS ****
 */

Fibo::Fibo() : number_data(1) {}

Fibo::Fibo(const std::string &number_string) : number_data(number_string) {
    assert(number_string.size() == 1 || *number_string.begin() == '1');
    normalize();
}

Fibo::Fibo(const char *number) : Fibo(std::string(number)) {}

Fibo::Fibo(const Fibo &other_fibo) : number_data(other_fibo.number_data) {}

bool operator<(const Fibo &left, const Fibo &right) {
    if (left.number_data.size() < right.number_data.size()) {
        return true;
    }
    if (left.number_data.size() > right.number_data.size()) {
        return false;
    }
    return left.number_data < right.number_data;
}

bool operator==(const Fibo &left, const Fibo &right) {
    return left.number_data == right.number_data;
}

Fibo &Fibo::operator=(const Fibo &right) {
    number_data = right.number_data;
    return *this;
}

/**
 * Implementation of addition for two fibo number based on paper https://arxiv.org/abs/1207.4497
 * @param right number to be added to this
 * @return summed number fibo
 */
Fibo &Fibo::operator+=(const Fibo &right) {
    std::vector<uint8_t> temp_data(1 + std::max(length(), right.length()));
    for (size_t i = 0; i < std::min(length(), right.length()); ++i) {
        temp_data[i] = ((uint8_t) number_data.test(i)) + ((uint8_t) right.number_data.test(i));
    }

    const Bitset *longer_data = length() > right.length() ? &number_data : &right.number_data;
    for (size_t i = std::min(length(), right.length()); i < std::max(length(), right.length()); ++i) {
        temp_data[i] = (uint8_t) longer_data->test(i);
    }

    exchange_in_frames<4, 12>(temp_data, true, ADDITION_FRAME_PAIRS);
    try_exchange_single_frame<3>(temp_data, 2, {0, 2, 1}, {1, 1, 0});
    try_exchange_single_frame<3>(temp_data, 2, {0, 1, 2}, {1, 0, 1});
    try_exchange_single_frame<3>(temp_data, 2, {0, 3, 0}, {1, 1, 1});
    try_exchange_single_frame<3>(temp_data, 2, {0, 2, 0}, {1, 0, 1});
    try_exchange_single_frame<4>(temp_data, 3, {0, 1, 2, 0}, {1, 0, 1, 0});
    try_exchange_single_frame<2>(temp_data, 1, {0, 3}, {1, 1});
    try_exchange_single_frame<2>(temp_data, 1, {0, 2}, {1, 0});
    try_exchange_single_frame<3>(temp_data, 2, {0, 1, 2}, {1, 0, 1});
    exchange_in_frames<3, 1>(temp_data, false, {frame_pair<3>{{0, 1, 1}, {1, 0, 0}}});
    exchange_in_frames<3, 1>(temp_data, true, {frame_pair<3>{{0, 1, 1}, {1, 0, 0}}});

    size_t size = temp_data.back() == 0 ? temp_data.size() - 1 : temp_data.size();
    number_data.resize(size);
    for (size_t i = 0; i < size; ++i) {
        assert(temp_data[i] < 2);
        number_data[i] = temp_data[i];
    }
    normalize();

    return *this;
}

Fibo &Fibo::operator&=(const Fibo &right) {
    bitwise_operator(right, std::bit_and<>());
    return *this;
}

Fibo &Fibo::operator|=(const Fibo &right) {
    bitwise_operator(right, std::bit_or<>());
    return *this;
}

Fibo &Fibo::operator^=(const Fibo &right) {
    bitwise_operator(right, std::bit_xor<>());
    return *this;
}

Fibo &Fibo::operator<<=(const size_t& n) {
    number_data.resize(number_data.size() + n);
    number_data <<= n;
    return *this;
}

std::ostream &operator<<(std::ostream &stream, const Fibo &data) {
    stream << data.number_data;
    return stream;
}

size_t Fibo::length() const {
    return number_data.size();
}

/*
 * ****PRIVATE METHODS AND MEMBERS ****
 */

void Fibo::normalize() {
    size_t idx = highest_set_bit_idx();
    if (idx == number_data.size() || idx == 0) {
        number_data.resize(1);
        return;
    }
    number_data.resize(idx + 1);

    if (number_data.test(idx) && number_data.test(idx - 1)) {
        number_data[idx] = false;
        number_data[idx - 1] = false;
        number_data.push_back(true);
    }
    for (size_t i = idx - 1; i > 0; i--) {
        if (number_data.test(i) && number_data.test(i - 1)) {
            number_data[i - 1] = false;
            number_data[i] = false;
            number_data[i + 1] = true;
            normalize_backwards(i + 1);
        }
    }
}

void Fibo::normalize_backwards(size_t idx) {
    assert(number_data.test(idx));
    while (idx < number_data.size() - 1) {
        if (number_data.test(idx + 1)) {
            number_data[idx] = false;
            number_data[idx + 1] = false;
            if (idx == number_data.size() - 2) {
                number_data.push_back(true);
            } else {
                number_data[idx + 2] = true;
            }
            idx += 2;
        } else {
            return;
        }
    }
}

size_t Fibo::highest_set_bit_idx() const {
    for (size_t i = number_data.size(); i > 0; i--) {
        if (number_data.test(i - 1)) {
            return i - 1;
        }
    }
    return number_data.size();
}

void Fibo::bitwise_operator(const Fibo &right, const std::function<bool(bool, bool)> &op) {
    if (number_data.size() < right.number_data.size()) {
        number_data.resize(right.number_data.size());
    }
    for (size_t i = 0; i < right.number_data.size(); ++i) {
        number_data[i] = op(number_data[i], right.number_data[i]);
    }
    for (size_t i = right.number_data.size(); i < number_data.size(); ++i) {
        number_data[i] = op(number_data[i], false);
    }
    normalize();
}

/*
 * ****STATIC METHODS AND MEMBERS ****
 */

const std::vector<Positive> Fibo::computed_fib_numbers = Fibo::calculcate_fib_numbers(ULLONG_MAX);

const std::array<Fibo::frame_pair<4>, 12> Fibo::ADDITION_FRAME_PAIRS = {
    frame_pair<4>{{0, 2, 0, 0}, {1, 0, 0, 1}}, frame_pair<4>{{0, 2, 0, 1}, {1, 0, 0, 2}},
    frame_pair<4>{{0, 2, 0, 2}, {1, 0, 0, 3}}, frame_pair<4>{{0, 3, 0, 0}, {1, 1, 0, 1}},
    frame_pair<4>{{0, 3, 0, 1}, {1, 1, 0, 2}}, frame_pair<4>{{0, 3, 0, 2}, {1, 1, 0, 3}},
    frame_pair<4>{{0, 2, 1, 0}, {1, 1, 0, 0}}, frame_pair<4>{{0, 2, 1, 1}, {1, 1, 0, 1}},
    frame_pair<4>{{0, 2, 1, 2}, {1, 1, 0, 2}}, frame_pair<4>{{0, 1, 2, 0}, {1, 0, 1, 0}},
    frame_pair<4>{{0, 1, 2, 1}, {1, 0, 1, 1}}, frame_pair<4>{{0, 1, 2, 2}, {1, 0, 1, 2}}
};

std::vector<Positive> Fibo::calculcate_fib_numbers(Positive limit) {
    std::vector<Positive> result;
    Positive a = 0, b = 1;
    // Prevent integer overflow
    while (limit - b >= a) {
        b += a;
        a = b - a;
        result.push_back(b);
    }
    return result;
}

Positive Fibo::fib_number(Positive nth) {
    assert(nth < computed_fib_numbers.size());
    return computed_fib_numbers[nth];
}

Positive Fibo::index_of_fib_less_or_equal(Positive number) {
    auto it = std::lower_bound(Fibo::computed_fib_numbers.begin(), Fibo::computed_fib_numbers.end(), number);
    if (it == Fibo::computed_fib_numbers.end()) {
        return Fibo::computed_fib_numbers.size() - 1;
    }
    Positive index = it - Fibo::computed_fib_numbers.begin();
    return fib_number(index) == number ? index : index - 1;
}

Bitset Fibo::get_fib_representation(Positive number) {
    if (number == 0) {
        Bitset result(1);
        return result;
    }

    Bitset result(index_of_fib_less_or_equal(number) + 1);
    Positive index;
    while (number > 0) {
        index = index_of_fib_less_or_equal(number);
        result[index] = true;
        number -= fib_number(index);
    }
    // number may need normalization now if it is really big uint64_t
    return result;
}

template <std::size_t frame_size>
void Fibo::try_exchange_single_frame(std::vector<uint8_t> &data, size_t index,
                                     const std::array<uint8_t, frame_size> &from,
                                     const std::array<uint8_t, frame_size> &to) {
    if (data.size() >= frame_size && index < data.size()) {
        for (size_t j = 0; j < frame_size; ++j) {
            if (data[index - j] != from[j]) {
                return;
            }
        }
        for (size_t j = 0; j < frame_size; ++j) {
            data[index - j] = to[j];
        }
    }
}

template <std::size_t frame_size, std::size_t frames>
void Fibo::exchange_in_frames(std::vector<uint8_t> &data, bool from_left,
    const std::array<std::pair<std::array<uint8_t, frame_size>, std::array<uint8_t, frame_size>>, frames> &frame_pairs) {
    size_t i = from_left ? data.size() - 1 : frame_size - 1;

    while ((from_left && i > frame_size - 2) || (!from_left && i < data.size())) {
        for (auto const &frame_pair : frame_pairs) {
            try_exchange_single_frame<frame_size>(data, i, frame_pair.first, frame_pair.second);
        }
        i += (from_left ? -1 : 1);
    }
}

/*
 * **** NON-MEMBER FUNCTIONS ****
 */


const Fibo &Zero() {
    static Fibo zero(0);
    return zero;
}

const Fibo &One() {
    static Fibo one(1);
    return one;
}
