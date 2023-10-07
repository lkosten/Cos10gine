project(catch)

set(HEADER_FILES
        catch/catch_amalgamated.hpp
        )

set(SOURCE_FILES
        catch/catch_amalgamated.cpp
        Test.cpp
        PerftTests.cpp
        )


add_library(catch_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

add_executable(AllTests ${SOURCE_FILES})

target_link_libraries(AllTests ChessEngine_lib)
