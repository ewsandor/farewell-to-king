/*
 farewell_to_king_types.h
 Farewell To King - Chess Library
 Edward Sandor
 November 2014 - 2020
 
 Contains all type, constant, and structure definitions to be used used by FarewellToKing.
*/

#ifndef _FAREWELL_TO_KING_TYPES_H_
#define _FAREWELL_TO_KING_TYPES_H_
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Defining board size
 * 
 */
#define FTK_STD_BOARD_ROWS    8
#define FTK_STD_BOARD_COLUMNS 8
#define FTK_STD_BOARD_SIZE    (FTK_STD_BOARD_ROWS * FTK_STD_BOARD_COLUMNS)

/**
 * @brief Enum for representing function results
 * 
 */
typedef enum
{
  FTK_SUCCESS,
  FTK_FAILURE,
} ftk_result_e;

/**
 * @brief Type for representing board bitmasks
 * 
 */
typedef uint64_t ftk_board_mask_t;

/**
 * @brief Type for representing board positions
 * 
 */
typedef uint8_t ftk_position_t;

/**
 * @brief Converts a position index to a mask bit
 * 
 */
#define FTK_POSITION_TO_MASK(position) (1ULL << (position))
/**
 * @brief Converts a mask bit to position index (Returns mask MSB if multiple bits are set)
 * 
 */
#define FTK_MASK_TO_POSITION(mask, position) { (position) = 0; ftk_board_mask_t temp_mask = (mask); while(temp_mask){temp_mask >>= 1; if(temp_mask){(position)++;}}}

/**
 * @brief Check state enum
 * 
 */
typedef enum
{
  FTK_CHECK_UNKNOWN,
  FTK_CHECK_NO_CHECK,
  FTK_CHECK_IN_CHECK,
} ftk_check_e;

#define FTK_DRAW_MOVES 50
#define FTK_DRAW_HALF_MOVES (2*FTK_DRAW_MOVES)

/**
 * @brief Game end types enum
 * 
 */
typedef enum
{
  FTK_END_NOT_OVER,
  FTK_END_CHECKMATE,
  FTK_END_RESIGN,
  FTK_END_FORFEIT,
  FTK_END_WIN_ON_TIME,
  FTK_END_DRAW_STALEMATE,
  FTK_END_DRAW_AGREED,
  FTK_END_DRAW_FIFTY_MOVE_RULE,
  FTK_END_DRAW_REPETITION,
  FTK_END_DRAW_DEAD_POSITION,
  FTK_END_DRAW_ON_TIME,
} ftk_game_end_e;

/**
 * @brief Castle mask bit enum
 * 
 */
typedef enum 
{
  FTK_CASTLE_NONE             = 0x0,
  FTK_CASTLE_KING_SIDE_WHITE  = 0x1,
  FTK_CASTLE_KING_SIDE_BLACK  = 0x2,
  FTK_CASTLE_QUEEN_SIDE_WHITE = 0x4,
  FTK_CASTLE_QUEEN_SIDE_BLACK = 0x8,
  FTK_CASTLE_ALL              = 0xF,
} ftk_castle_e;
/**
 * @brief Castle mask type
 * 
 */
typedef uint8_t ftk_castle_mask_t;

/**
 * @brief Colors enum 
 * 
 */
typedef enum{
  FTK_COLOR_NONE      = 0,
  FTK_COLOR_WHITE     = 1,
  FTK_COLOR_BLACK     = 2,
  FTK_COLOR_DONT_CARE = 3,
} ftk_color_e;

/**
 * @brief Piece type enum
 * 
 */
typedef enum 
{
  FTK_TYPE_EMPTY     = 0,
  FTK_TYPE_PAWN      = 1,
  FTK_TYPE_KNIGHT    = 2,
  FTK_TYPE_BISHOP    = 3,
  FTK_TYPE_ROOK      = 4,
  FTK_TYPE_QUEEN     = 5,
  FTK_TYPE_KING      = 6,
  FTK_TYPE_DONT_CARE = 7,
} ftk_type_e;

/**
 * @brief Piece move status enum
 * 
 */
typedef enum
{
  FTK_MOVED_INVALID   = 0,
  FTK_MOVED_NOT_MOVED = 1,
  FTK_MOVED_HAS_MOVED = 2,
  FTK_MOVED_DONT_CARE = 3,
} ftk_moved_status_e;

/**
 * @brief Square state information
 * 
 */
typedef struct {
  ftk_type_e         type:3;
  ftk_color_e        color:2;
  ftk_moved_status_e moved:2;
} ftk_square_s;

/**
 * @brief Clears square to be an empty square
 * 
 */
#define FTK_SQUARE_CLEAR(square)      \
{                                     \
  (square).type  = FTK_TYPE_EMPTY;    \
  (square).color = FTK_COLOR_NONE;    \
  (square).moved = FTK_MOVED_INVALID; \
}

/**
 * @brief Sets square with specified values 
 */
#define FTK_SQUARE_SET(square, type_v, color_v, moved_v)  \
{                                                         \
  (square).type  = type_v;                                \
  (square).color = color_v;                               \
  (square).moved = moved_v;                               \
}

/**
 * @brief Checks if two squares are identical
 * 
 */
#define FTK_SQUARE_EQUAL(s1, s2)  \
  ( ((s1).type  == (s2).type)  && \
    ((s1).color == (s2).color) && \
    ((s1).moved == (s2).moved) )
/**
 * @brief Checks if square has given type, color, moved properties
 * 
 */
#define FTK_SQUARE_IS(square, type_v, color_v, moved_v)                    \
  ( (((square).type  == type_v ) || (FTK_TYPE_DONT_CARE  == type_v )) && \
    (((square).color == color_v) || (FTK_COLOR_DONT_CARE == color_v)) && \
    (((square).moved == moved_v) || (FTK_MOVED_DONT_CARE == moved_v)) )

/**
 * @brief Type for keeping track of move count
 * 
 */
typedef uint16_t ftk_move_count_t;

/**
 * @brief Game board structure
 * 
 */
typedef struct
{
  /* Square contents */
  ftk_square_s     square[FTK_STD_BOARD_SIZE];

  /* Valid moves for a given square */
  ftk_board_mask_t move_mask[FTK_STD_BOARD_SIZE];
  
  /* Various board bitmasks parsed from square array */
  ftk_board_mask_t board_mask;
  ftk_board_mask_t white_mask;
  ftk_board_mask_t black_mask;
  ftk_board_mask_t pawn_mask;
  ftk_board_mask_t knight_mask;
  ftk_board_mask_t bishop_mask;
  ftk_board_mask_t rook_mask;
  ftk_board_mask_t queen_mask;
  ftk_board_mask_t king_mask;

} ftk_board_s;

/**
 * @brief Game structure
 * 
 */
typedef struct 
{
  /* Active game board */
  ftk_board_s    board;

  /* Current En Pasant target position */
  ftk_position_t ep;

  /* Current turn color */
  ftk_color_e turn;

  /* Number of half moves since last capture or pawn movement */
  ftk_move_count_t halfmove;
  /* Number of full moves in given game */
  ftk_move_count_t fullmove;
} ftk_game_s;

/**
 * @brief Structure storing details of a move
 * 
 */
typedef struct 
{
  /* Move target position */
  ftk_position_t   target;
  /* Move source position */
  ftk_position_t   source;

  /* Moved piece before this move */
  ftk_square_s     moved;
  /* Piece captured in this move, or Rook in castle case */
  ftk_square_s     capture;

  /* En Pasant target position before this move */
  ftk_position_t   ep;
  /* Type Pawn is prmomoted to */
  ftk_type_e       pawn_promotion;

  /* Current turn color */
  ftk_color_e      turn;

  /* Number of moves before this move */
  ftk_move_count_t halfmove;
  ftk_move_count_t fullmove;
} ftk_move_s;

//Square value table
#define FTK_A1  0
#define FTK_B1  1
#define FTK_C1  2
#define FTK_D1  3
#define FTK_E1  4
#define FTK_F1  5
#define FTK_G1  6
#define FTK_H1  7
#define FTK_A2  8
#define FTK_B2  9
#define FTK_C2 10
#define FTK_D2 11
#define FTK_E2 12
#define FTK_F2 13
#define FTK_G2 14
#define FTK_H2 15
#define FTK_A3 16
#define FTK_B3 17
#define FTK_C3 18
#define FTK_D3 19
#define FTK_E3 20
#define FTK_F3 21
#define FTK_G3 22
#define FTK_H3 23
#define FTK_A4 24
#define FTK_B4 25
#define FTK_C4 26
#define FTK_D4 27
#define FTK_E4 28
#define FTK_F4 29
#define FTK_G4 30
#define FTK_H4 31
#define FTK_A5 32
#define FTK_B5 33
#define FTK_C5 34
#define FTK_D5 35
#define FTK_E5 36
#define FTK_F5 37
#define FTK_G5 38
#define FTK_H5 39
#define FTK_A6 40
#define FTK_B6 41
#define FTK_C6 42
#define FTK_D6 43
#define FTK_E6 44
#define FTK_F6 45
#define FTK_G6 46
#define FTK_H6 47
#define FTK_A7 48
#define FTK_B7 49
#define FTK_C7 50
#define FTK_D7 51
#define FTK_E7 52
#define FTK_F7 53
#define FTK_G7 54
#define FTK_H7 55
#define FTK_A8 56
#define FTK_B8 57
#define FTK_C8 58
#define FTK_D8 59
#define FTK_E8 60
#define FTK_F8 61
#define FTK_G8 62
#define FTK_H8 63
#define FTK_XX 64

#endif //_FAREWELL_TO_KING_TYPES_H_