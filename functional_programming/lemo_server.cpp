#include <iostream>
#include <ranges>
#include <vector>
#include "utils.h"

// TODO:
// Call

using namespace std::views;

struct FiniteDifferenceFilter {
    double value{0.};
    bool needs_init_{true};
    double input_last_{0.};
};

int main() {
    // Reproduce chains of operations in the following objects using C++20
    // ranges
    // Get these global constants from config file
    constexpr double SCALE_FIRMWARE = .001;
    constexpr double SCALE_ENCODERS =
        0.00038351860508939673;  // 2 * pi / (2^14 - 1) counts
    constexpr double OFFSET_ENCODERS = 0.0;

    // This is what we receive from Odrive
    std::vector<uint16_t> encoder_readings = {3, 2, 1};
    std::vector<uint8_t> raw_current = {3, 2, 1};

    FiniteDifferenceFilter fd;

    // (Linear) transformations
    auto cast_to_int16 =
        transform([](uint64_t data) { return static_cast<int16_t>(data); });
    auto scale = transform([](int16_t data) { return data * SCALE_FIRMWARE; });
    auto recenter = transform(
        [](double data) { return lemo::utils::recenterAngle(data, 0.); });
    auto encoder_scale =
        transform([](int data) { return data * SCALE_ENCODERS; });
    auto encoder_offset =
        transform([](int data) { return data - OFFSET_ENCODERS; });

    // Transformation cascades
    auto motor_current = raw_current | cast_to_int16 | scale;
    auto motor_position_wrapped =
        encoder_readings | encoder_offset | encoder_scale;
    auto motor_position = motor_position_wrapped | recenter;
    auto motor_velocity = motor_position;

    for (double i : motor_current) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    for (double i : motor_position_wrapped) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    for (double i : motor_position) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}
