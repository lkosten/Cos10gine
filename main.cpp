#include "Interfaces/UCI.h"

#pragma execution_character_set( "utf-8" )

int main() {
    // SetConsoleOutputCP( 65001 );

    UCI uci;
    uci.MainUpdateLoop(std::cin);

    return 0;
}
