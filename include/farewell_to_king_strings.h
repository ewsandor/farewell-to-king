/*
 farewell_to_king_strings.h
 FarewellToKing - Chess Library
 Edward Sandor
 January 2015 - 2020
 
 Contains declarations of all methods that generate formatted strings for human readable output.
*/

#ifndef FAREWELLTOKINGSTR_H
#define FAREWELLTOKINGSTR_H
#include "farewell_to_king_types.h"

/**
 * @brief type to represent string indiceis
 *
 */
typedef uint32_t ftk_string_index_t;

/**
 * @brief Returns piece type based on character
 * 
 * @param input 
 * @return ftk_type_e, FTK_TYPE_EMPTY if unkown piece
 */
ftk_type_e ftk_char_to_piece_type(char input);

/**
 * @brief Converts chess coordinate to square number
 * 
 * @param input Chess coordinate string. size 2
 * @return ftk_position_t 
 */
ftk_position_t ftk_string_to_position(const char *input);

/**
 * @brief Converts square number to chess coordinate
 * 
 * @param position position to be converted
 * @param coordinate string representing position. size 3
 */
void ftk_position_to_string(ftk_position_t position, char *coordinate);

/**
 * @brief Converts character to a square
 * 
 * @param input A character representing a piece
 * @return ftk_square_s 
 */
ftk_square_s ftk_char_to_square(char input);

/**
 * @brief Converts square a character.  Uppercase white, lowercase black.  'X' if empty
 * 
 * @param piece 
 * @return char 
 */
char ftk_square_to_char(ftk_square_s piece);

#define FTK_MOVE_STRING_SIZE 6

/**
 * @brief Parses a long algebraic move string 
 * 
 * @param input input string, assumes exactly one long algebraic command in 0-terminated string with no leading or trailing white space. Size FTK_MOVE_STRING_SIZE
 * @param target output target position
 * @param source output source position
 * @param pawn_promotion output Pawn promotion type
 * @param castle output castle type request (will always return white castle as color is unknown)
 * @return ftk_result_e
 */
ftk_result_e ftk_long_algebraic_move(char *input, ftk_position_t *target,
                             ftk_position_t *source, ftk_type_e *pawn_promotion,
                             ftk_castle_e *castle);

/**
 * @brief Parses a xboard move string 
 * 
 * @param input input string, assumes exactly one xboard command in 0-terminated string with no leading or trailing white space. Size FTK_MOVE_STRING_SIZE
 * @param target output target position
 * @param source output source position
 * @param pawn_promotion output Pawn promotion type
 * @param castle output castle type request (will always return white castle as color is unknown)
 * @return ftk_result_e
 */
ftk_result_e ftk_xboard_move(char *input, ftk_position_t *target,
                             ftk_position_t *source, ftk_type_e *pawn_promotion,
                             ftk_castle_e *castle);

/**
 * @brief Create string for mask without board coordinates
 * 
 * @param mask Mask to convert
 * @param output size 144.  Lines ended with "\r\n".
 */
void ftk_mask_to_string(ftk_board_mask_t mask, char *output);

/**
 * @brief Create string for board without board coordinates
 * 
 * @param board Board to convert
 * @param output size 144.  Lines ended with "\r\n".
 */
void ftk_board_to_string(const ftk_board_s *board, char *output);

/**
 * @brief Create string for mask with board coordinates
 * 
 * @param mask Mask to convert
 * @param output size 210.  Lines ended with "\r\n".
 */
void ftk_mask_to_string_with_coordinates(ftk_board_mask_t mask, char *output);

/**
 * @brief Create string for board with board coordinates
 * 
 * @param board Board to convert
 * @param output size 210.  Lines ended with "\r\n".
 */
void ftk_board_to_string_with_coordinates(const ftk_board_s *board, char *output);

/**
 * @brief Create Forsyth-Edwards Notation representation of 'game'.
 * 
 * @param game 
 * @param output 
 */
void ftk_game_to_fen_string(const ftk_game_s *game, char *output);

/**
 * @brief Creates a game from Forsyth-Edwards Notation 
 * 
 * @param game Game to store game data
 * @param fen String containing FEN data.
 */
ftk_result_e ftk_create_game_from_fen_string(ftk_game_s *game, const char *fen);

/**
 * @brief Converts move to xboard string
 * 
 * @param move 
 * @param output buffer for output string (expect size >= FTK_MOVE_STRING_SIZE)
 * @return ftk_result_e 
 */
ftk_result_e ftk_move_to_xboard_string(ftk_move_s *move, char * output);

#endif
