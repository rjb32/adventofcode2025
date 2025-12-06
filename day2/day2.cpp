#include <stdint.h>
#include <locale>
#include <vector>

#include <assert.h>
#include <iostream>

// Custom locale structure to change the definition
// of what is considered whitespace for formatted input streams
// Inspired from https://stackoverflow.com/questions/7302996/changing-the-delimiter-for-cin-c
struct CustomDelim : std::ctype<char> {
    CustomDelim()
        : std::ctype<char>(getTable())
    {
    }

    static mask const* getTable() {
        static mask rc[table_size];
        rc[','] = std::ctype_base::space;
        rc['-'] = std::ctype_base::space;
        rc['\n'] = std::ctype_base::space;
        return &rc[0];
    }
};

bool isInvalid(const std::vector<uint64_t>& digits) {
    const size_t n = digits.size();
    const size_t firstEnd = n/2;

    if (n%2 != 0) {
        return false;
    }

    for (size_t i = 0; i < firstEnd; i++) {
        if (digits[i] != digits[firstEnd+i]) {
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv) {
    // Set custom delimiter to be able to use formatted input
    CustomDelim customDelim;
    std::cin.imbue(std::locale(std::cin.getloc(), &customDelim));

    // Digit buffer to be reused for all IDs
    std::vector<uint64_t> digits;

    uint64_t num = 0;
    bool readingLHS = true;
    uint64_t lhs = 0;
    uint64_t rhs = 0;
    uint64_t totalInvalid = 0;
    uint64_t invalidSum = 0;
    while (std::cin >> num) {
        if (readingLHS) {
            lhs = num;
            readingLHS = false;
            continue;
        } else {
            rhs = num;
            readingLHS = true;
        }

        std::cout << lhs << ' ' << rhs << '\n';
        assert(lhs <= rhs);

        // Search for invalid IDs
        uint64_t invalidCount = 0;
        for (uint64_t ID = lhs; ID <= rhs; ID++) {
            digits.clear();

            // Use horner rule to extract digits of ID
            uint64_t current = ID;
            uint64_t prevDigit = 0;
            bool allSame = true;
            bool first = true;
            while (current != 0) {
                const uint64_t digit = current%10;
                current = current/10;
                digits.push_back(digit);

                if (first) {
                    first = false;
                } else {
                    if (digit != prevDigit) {
                        allSame = false;
                    }
                }

                prevDigit = digit;
            }

            const bool invalid = (digits.size()%2 == 0) && (allSame || isInvalid(digits));
            if (invalid) {
                std::cout << "Invalid " << ID << '\n';
                invalidCount++;
                invalidSum += ID;
            }
        }

        totalInvalid += invalidCount;
    }

    std::cout << "Total invalid IDs count: " << totalInvalid << '\n';
    std::cout << "Sum: " << invalidSum << "\n";

    return 0;
}
