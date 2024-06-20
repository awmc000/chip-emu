#include "chip8.hpp"
#include <catch2/catch_test_macros.hpp>


TEST_CASE( "Can create a Chip8 object", "[create]") {
    Chip8 chip{};
    REQUIRE(chip.draw == 0);
}