#include <assert.h>
#include <iostream>

constexpr uint64_t DIAL_SIZE = 100;

struct SimuResult {
    uint64_t _pos {0};
    uint64_t _zeroes {0};
};

// Simulate the effect of a move of the dial
// by simulating the rotation of the dial
SimuResult simMove(uint64_t pos, int sign, uint64_t dist) {
    uint64_t zeroes = 0;
    if (sign > 0) {
        while (dist > 0) {
            if (pos == 99) {
                pos = 0; 
            } else {
                pos++;
            }

            zeroes += (pos == 0);
            dist--;
        }
    } else if (sign < 0) {
        while (dist > 0) {
            if (pos == 0) {
                pos = 99;
            } else {
                pos--;
            }

            zeroes += (pos == 0);
            dist--;
        }
    }

    return {pos, zeroes};
}

// Compute effect of a move without a loop
// new position = (pos +/- dist) % DIAL_SIZE
SimuResult computeMove(uint64_t pos, int sign, uint64_t dist) {
    uint64_t zeroes = 0;
    if (sign < 0) {
        if (dist > pos) {
            const uint64_t diff = dist-pos;
            const uint64_t mod = diff%DIAL_SIZE;
            zeroes = (dist-pos)/DIAL_SIZE + (pos != 0);
            pos = (mod == 0) ? 0 : DIAL_SIZE-mod;
        } else if (dist < pos) {
            zeroes = 0;
            pos = pos-dist;
        } else {
            // pos == dist
            zeroes = 1;
            pos = 0;
        }
    } else if (sign > 0) {
        zeroes = (pos+dist)/DIAL_SIZE;
        pos = (pos+dist)%DIAL_SIZE;
    }

    return {pos, zeroes};
}

constexpr bool USE_SIM = false;

SimuResult updateMove(uint64_t pos, int sign, uint64_t dist) {
    if constexpr (USE_SIM) {
        return simMove(pos, sign, dist);
    } else {
        return computeMove(pos, sign, dist);
    }
}

int main(int argc, char** argv) {

    std::string line;

    uint64_t pos = 50;
    uint64_t countAtRestZeroes = 0;
    uint64_t countEncounteredZeroes = 0;
    while (std::cin >> line) {
        if (line.empty()) {
            break;
        }

        const size_t lineSize = line.size();
        assert(lineSize > 1);

        const char firstChar = line[0];

        // Horner rule to parse the distance
        uint64_t dist = 0;
        for (size_t i = 1; i < lineSize; i++) {
            const char distChar = line[i];
            dist = dist*10 + (distChar-'0');
        }

        // Update position based on move
        if (firstChar == 'L') {
            // Minus
            const auto [npos, zeroes] = updateMove(pos, -1, dist);
            pos = npos;
            countEncounteredZeroes += zeroes;
            std::cout << firstChar << dist << " " << zeroes << '\n';
        } else if (firstChar == 'R') {
            // Plus
            const auto [npos, zeroes] = updateMove(pos, 1, dist);
            pos = npos;
            countEncounteredZeroes += zeroes;
            std::cout << firstChar << dist << " " << zeroes << '\n';
        } else {
            assert(false);
        }

        countAtRestZeroes += (pos == 0);
    }

    std::cout << "Puzzle1 zeroes at rest: " << countAtRestZeroes << '\n';
    std::cout << "Puzzle1 zeroes encountered in movement: " << countEncounteredZeroes << '\n';

    return 0;
}
