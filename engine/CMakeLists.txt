project(ChessEngine_lib)

set(HEADER_FILES
        BoardRepresentation/BitBoard.h
        BoardRepresentation/PieceDefines.h
        MoveGeneration/MoveGenerator.h
        BoardRayIterator.h
        ChessGame.h
        Interfaces/FEN.h
        MoveGeneration/PrecomputedPiecePatterns.h
        MoveGeneration/MagicBitboards.h
        )

set(SOURCE_FILES
        BoardRepresentation/BitBoard.cpp
        MoveGeneration/MoveGenerator.cpp
        BoardRayIterator.cpp
        ChessGame.cpp
        Interfaces/FEN.cpp
        MoveGeneration/PrecomputedPiecePatterns.cpp
        MoveGeneration/MagicBitboards.cpp
        Search/AlphaBetaSearch.cpp Search/AlphaBetaSearch.h Evaluation/SimpleEvaluation.cpp Evaluation/SimpleEvaluation.h BoardRepresentation/ZobristHash.cpp BoardRepresentation/ZobristHash.h Interfaces/UCI.cpp Interfaces/UCI.h GrandMaster.cpp GrandMaster.h)

add_library(ChessEngine_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})