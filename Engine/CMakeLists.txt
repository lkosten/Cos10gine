project(Engine)

set(HEADER_FILES
        BoardRepresentation/BitBoard.h
        BoardRepresentation/PieceDefines.h
        MoveGenerator.h
        BoardRayIterator.h
        ChessGame.h
        ChessNotations/FEN.h)

set(SOURCE_FILES
        BoardRepresentation/BitBoard.cpp
        MoveGenerator.cpp
        BoardRayIterator.cpp
        ChessGame.cpp
        ChessNotations/FEN.cpp)

add_library(Engine STATIC ${SOURCE_FILES} ${HEADER_FILES})