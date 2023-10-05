//
// Created by Kostya on 10/6/2023.
//

#include "catch/catch_amalgamated.hpp"

#include <cstdint>

uint32_t factorial( uint32_t number ) {
    return number <= 1 ? number : factorial(number-1) * number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( factorial( 1) == 1 );
    REQUIRE( factorial( 2) == 2 );
    REQUIRE( factorial( 3) == 3 );
    REQUIRE( factorial(10) == 3'628'800 );
}

TEST_CASE( "Factorials are computed2", "[factorial]" ) {
    REQUIRE( factorial( 1) == 1 );
    REQUIRE( factorial( 2) == 2 );
    REQUIRE( factorial( 3) == 6 );
    REQUIRE( factorial(10) == 3'628'800 );
}

TEST_CASE( "Factorials are computed3", "[factorial]" ) {
    REQUIRE( factorial( 1) == 1 );
    REQUIRE( factorial( 2) == 2 );
    REQUIRE( factorial( 3) == 3 );
    REQUIRE( factorial(10) == 3'628'800 );
}