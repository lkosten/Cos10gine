project(ChessEngine_lib)

set(HEADER_FILES
        BoardRepresentation/BitBoard.h
        BoardRepresentation/PieceDefines.h
        MoveGeneration/MoveGenerator.h
        BoardRayIterator.h
        ChessGame.h
        ChessNotations/FEN.h
        MoveGeneration/PrecomputedPiecePatterns.h
        MoveGeneration/MagicBitboards.h
        )

set(SOURCE_FILES
        BoardRepresentation/BitBoard.cpp
        MoveGeneration/MoveGenerator.cpp
        BoardRayIterator.cpp
        ChessGame.cpp
        ChessNotations/FEN.cpp
        MoveGeneration/PrecomputedPiecePatterns.cpp
        MoveGeneration/MagicBitboards.cpp
        Search/AlphaBetaSearch.cpp Search/AlphaBetaSearch.h Evaluation/SimpleEvaluation.cpp Evaluation/SimpleEvaluation.h BoardRepresentation/ZobristHash.cpp BoardRepresentation/ZobristHash.h)

add_library(ChessEngine_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})