/*
 farewell_to_king_strings.h
 Farewell To King - Chess Library
 Edward Sandor
 January 2015 - 2020
 
 Contains implementations of all methods that generate formatted strings for human readable output.
*/

#include <assert.h>
#include <string.h>
#include "farewell_to_king.h"
#include "farewell_to_king_strings.h"

#define FTK_MASK_STRING_CHAR 'X'

static bool ftk_increment_string_index(const char * str, ftk_string_index_t *index, ftk_string_index_t increment)
{
  ftk_string_index_t i = 0;
  bool ret_val = true;

  for(i = 0; i < increment; i++)
  {
    if(0 == str[*index])
    {
      ret_val = false;
      break;
    }
    else
    {
      (*index)++;
    }
  }

  return ret_val;
}

ftk_position_t ftk_string_to_position(const char *input) {
  if(input[0] >= 'A' && input[0] <= 'H' && input[1] >= '1' && input[1] <= '8')
  {
    return (input[1] - '1')*8 + (input[0] - 'A');
  }
  else if(input[0] >= 'a' && input[0] <= 'h' && input[1] >= '1' && input[1] <= '8')
  {
    return (input[1] - '1')*8 + (input[0] - 'a');
  }
  else
  {
    return FTK_XX;
  }
}
void ftk_position_to_string(ftk_position_t square, char coordinate[]) {
  if(square < FTK_XX){
    coordinate[0] = (square % 8) + 'a';
    coordinate[1] = (square / 8) + '1';
    coordinate[2] = 0;
  }
  else{
    coordinate[0] = 'X';
    coordinate[1] = 'X';
    coordinate[2] =  0 ;
  }
}

ftk_type_e ftk_char_to_piece_type(char input){
  switch(input){
    case 'P':
    case 'p':
      return FTK_TYPE_PAWN;
      break;
    case 'N':
    case 'n':
      return FTK_TYPE_KNIGHT;
      break;
    case 'B':
    case 'b':
      return FTK_TYPE_BISHOP;
      break;
    case 'R':
    case 'r':
      return FTK_TYPE_ROOK;
      break;
    case 'Q':
    case 'q':
      return FTK_TYPE_QUEEN;
      break;
    case 'K':
    case 'k':
      return FTK_TYPE_KING;
      break;
    default:
        return FTK_TYPE_EMPTY;
        break;
  } 
}

ftk_square_s ftk_char_to_square(char input) {
  ftk_square_s ret_val;
  FTK_SQUARE_CLEAR(ret_val);

  ret_val.type = ftk_char_to_piece_type(input);
  if(ret_val.type != FTK_TYPE_EMPTY)
  {
    ret_val.color = (input >= 'A' && input <= 'Z')?FTK_COLOR_WHITE:FTK_COLOR_BLACK;
  }

  return ret_val;
}

/**
 * @brief Convert a square into a character representation
 * 
 * @param square Square to convert
 * @return char Character representing the given square, 'X' if empty
 */
char ftk_square_to_char(ftk_square_s square) {
  switch(square.type)
  {
    case FTK_TYPE_PAWN:
      return (FTK_COLOR_WHITE == square.color)?'P':'p';
      break;
    case FTK_TYPE_KNIGHT:
      return (FTK_COLOR_WHITE == square.color)?'N':'n';
      break;
    case FTK_TYPE_BISHOP:
      return (FTK_COLOR_WHITE == square.color)?'B':'b';
      break;
    case FTK_TYPE_ROOK:
      return (FTK_COLOR_WHITE == square.color)?'R':'r';
      break;
    case FTK_TYPE_QUEEN:
      return (FTK_COLOR_WHITE == square.color)?'Q':'q';
      break;
    case FTK_TYPE_KING:
      return (FTK_COLOR_WHITE == square.color)?'K':'k';
      break;
    default:
      return 'X';
  }
}

/**
 * @brief Convert a type into a character representation
 * 
 * @param type Type to convert
 * @return char Character representing the given type, 'X' if invalid 
 */
char ftk_type_to_char(ftk_type_e type) {
  switch(type)
  {
    case FTK_TYPE_PAWN:
      return 'p';
      break;
    case FTK_TYPE_KNIGHT:
      return 'n';
      break;
    case FTK_TYPE_BISHOP:
      return 'b';
      break;
    case FTK_TYPE_ROOK:
      return 'r';
      break;
    case FTK_TYPE_QUEEN:
      return 'q';
      break;
    case FTK_TYPE_KING:
      return 'k';
      break;
    default:
      return 'X';
  }
}

ftk_result_e ftk_long_algebraic_move(const char *input, ftk_position_t *target,
                             ftk_position_t *source, ftk_type_e *pawn_promotion,
                             ftk_castle_e *castle) 
{

  ftk_result_e result = FTK_SUCCESS;
  uint8_t inputSize = 0;

  *pawn_promotion = FTK_TYPE_EMPTY;
  *castle = FTK_CASTLE_NONE;

  for(inputSize = 0; input[inputSize] != 0; inputSize++);

  if(input[0] == 'O'){
    if(inputSize == 3)
    {
      *castle = FTK_CASTLE_KING_SIDE_WHITE;
    }
    else
    {
      *castle = FTK_CASTLE_QUEEN_SIDE_WHITE;
    }
  }
  else if(inputSize >= 4){
      char coord[5];
      coord[0] = input[0];
      coord[1] = input[1];
      coord[2] = 0;
      *source = ftk_string_to_position(coord);

      coord[0] = input[2];
      coord[1] = input[3];
      coord[2] = 0;
      *target = ftk_string_to_position(coord);
      if(inputSize == 5)
        *pawn_promotion = ftk_char_to_piece_type(input[4]);
  }
  else {
    result = FTK_FAILURE;
  }

  return result;
}

ftk_result_e ftk_xboard_move(const char *input, ftk_position_t *target,
                             ftk_position_t *source, ftk_type_e *pawn_promotion,
                             ftk_castle_e *castle) 
{
  return ftk_long_algebraic_move(input, target, source, pawn_promotion, castle);
}

void ftk_mask_to_string(ftk_board_mask_t mask, char *output) {
  char ret[FTK_BOARD_STRING_SIZE]; //(2*8 columns + '\r\n')*8rows

  int i;
  for (i = 0; i < FTK_BOARD_STRING_SIZE; i++)
    ret[i] = ' ';
  for (i = 0; i < 8; i++) {
    ret[(i * (17) + 15)] = '\r';
    ret[(i * (17) + 16)] = '\n';
  }

  for (i = 0; i < FTK_STD_BOARD_SIZE; i++) {
    char name = ' ';
    if ((mask & (1ULL << i)) != 0)
      name = FTK_MASK_STRING_CHAR;
    else
      name = (((i + (i / 8)) % 2) ? '.' : '#');

    int j = FTK_STD_BOARD_SIZE - i - 1;
    int k = (-(j % 8) + 7);
    j = j + (k - (j % 8));
    ret[((j * 2) + (j / 8))] = name;
  }
  ret[143] = '\0';
  memcpy(output, ret, FTK_BOARD_STRING_SIZE);
}
/**
 * @brief Creates a monospace representation of a chess board ('\r\n' for newlines)
 * 
 * @param board Board to create a string for
 * @param output Output string, needs to store at least FTK_BOARD_STRING_SIZE characters
 */
void ftk_board_to_string(const ftk_board_s *board, char *output) {
  char ret[FTK_BOARD_STRING_SIZE]; //(2*8 columns + '\r\n')*8rows

  int i;
  for (i = 0; i < FTK_BOARD_STRING_SIZE; i++)
    ret[i] = ' ';
  for (i = 0; i < 8; i++) {
    ret[(i * (17) + 15)] = '\r';
    ret[(i * (17) + 16)] = '\n';
  }

  for (i = 0; i < FTK_STD_BOARD_SIZE; i++) {
    char name = ftk_square_to_char(board->square[i]);
    if ('X' == name) {
      name = (((i + (i / 8)) % 2) ? '.' : '#');
    }
    int j = FTK_STD_BOARD_SIZE - i - 1;
    int k = (-(j % 8) + 7);
    j = j + (k - (j % 8));
    ret[((j * 2) + (j / 8))] = name;
  }
  ret[143] = '\0';
  memcpy(output, ret, FTK_BOARD_STRING_SIZE);
}
/**
 * @brief Creates a monospace representation of a chess board bitmask with coordinate ('\r\n' for newlines)
 * 
 * @param board Board to create a string for
 * @param output Output string, needs to store at least FTK_BOARD_STRING_WITH_COORDINATES_SIZE characters
 */
void ftk_mask_to_string_with_coordinates(ftk_board_mask_t mask,
                                         char *output) {
  char ret[FTK_BOARD_STRING_WITH_COORDINATES_SIZE]; //('a  ' + 2*8 columns + '\r\n')*10rows (8 + gap + coordinate)

  int i;
  for (i = 0; i < FTK_BOARD_STRING_WITH_COORDINATES_SIZE; i++)
    ret[i] = ' ';
  for (i = 0; i < 10; i++) {
    ret[(i * (20) + 18)] = '\r';
    ret[(i * (20) + 19)] = '\n';
    if (i < 8)
      ret[(i * 20)] = '8' - i;
  }

  for (i = 0; i < FTK_STD_BOARD_SIZE; i++) {
    char name = ' ';
    if ((mask & (1ULL << i)) != 0)
      name = FTK_MASK_STRING_CHAR;
    else
      name = (((i + (i / 8)) % 2) ? '.' : '#');

    int j = FTK_STD_BOARD_SIZE - i - 1;
    int k = (-(j % 8) + 7);
    j = j + (k - (j % 8));
    ret[((j * 2) + (j / 8) + (j / 8 + 1) * 3)] = name;
  }
  for (i = FTK_STD_BOARD_SIZE + 8; i < FTK_STD_BOARD_SIZE + 16; i++) {
    ret[((i * 2) + (i / 8) + (i / 8 + 1) * 3)] =
        'a' + i - 8 - FTK_STD_BOARD_SIZE;
  }
  ret[209] = '\0';
  memcpy(output, ret, FTK_BOARD_STRING_WITH_COORDINATES_SIZE);
}
/**
 * @brief Creates a monospace representation of a chess board with coordinate ('\r\n' for newlines)
 * 
 * @param board Board to create a string for
 * @param output Output string, needs to store at least FTK_BOARD_STRING_WITH_COORDINATES_SIZE characters
 */
void ftk_board_to_string_with_coordinates(const ftk_board_s *board, char *output) {
  char ret[FTK_BOARD_STRING_WITH_COORDINATES_SIZE]; //('a  ' + 2*8 columns + '\r\n')*10rows

  int i;
  for (i = 0; i < FTK_BOARD_STRING_WITH_COORDINATES_SIZE; i++)
    ret[i] = ' ';
  for (i = 0; i < 10; i++) {
    ret[(i * (20) + 18)] = '\r';
    ret[(i * (20) + 19)] = '\n';
    if (i < 8)
      ret[(i * 20)] = '8' - i;
  }
  for (i = 0; i < FTK_STD_BOARD_SIZE; i++) {
    char name = ftk_square_to_char(board->square[i]);
    if ('X' == name) {
      name = (((i + (i / 8)) % 2) ? '.' : '#');
    }
    int j = FTK_STD_BOARD_SIZE - i - 1;
    int k = (-(j % 8) + 7);
    j = j + (k - (j % 8));
    ret[((j * 2) + (j / 8) + (j / 8 + 1) * 3)] = name;
  }
  for (i = FTK_STD_BOARD_SIZE + 8; i < FTK_STD_BOARD_SIZE + 16; i++) {
    ret[((i * 2) + (i / 8) + (i / 8 + 1) * 3)] =
        'a' + i - 8 - FTK_STD_BOARD_SIZE;
  }
  ret[209] = '\0';
  memcpy(output, ret, FTK_BOARD_STRING_WITH_COORDINATES_SIZE);
}
void ftk_game_to_fen_string(const ftk_game_s *game, char *output) 
{
  memset(output, 0, FTK_FEN_STRING_SIZE);

  int outI = 0;
  for (int i = 7; i >= 0; i--) {
    int count = 0;

    for (unsigned int j = 0; j < 8; j++) {
      if (game->board.square[i * 8 + j].type == FTK_TYPE_EMPTY) {
        count++;
      } else {
        if (count != 0) {
          output[outI] = '0' + count;
          outI++;
        }
        count = 0;
        output[outI] = ftk_square_to_char(game->board.square[i * 8 + j]);
        outI++;
      }
    }
    if (count != 0) {
      output[outI] = '0' + count;
      outI++;
    }
    if (i != 0) {
      output[outI] = '/';
      outI++;
    }
  }
  output[outI] = ' ';
  outI++;
  if (game->turn == FTK_COLOR_WHITE) {
    output[outI] = 'w';
    outI++;
  } else {
    output[outI] = 'b';
    outI++;
  }
  output[outI] = ' ';
  outI++;
  char castle = 1;
  if ((FTK_SQUARE_IS(game->board.square[FTK_E1], FTK_TYPE_KING, FTK_COLOR_WHITE,
                     FTK_MOVED_NOT_MOVED)) &&
      (FTK_SQUARE_IS(game->board.square[FTK_H1], FTK_TYPE_ROOK, FTK_COLOR_WHITE,
                     FTK_MOVED_NOT_MOVED))) {
    output[outI] = 'K';
    outI++;
    castle = 0;
  }
  if ((FTK_SQUARE_IS(game->board.square[FTK_E1], FTK_TYPE_KING, FTK_COLOR_WHITE,
                     FTK_MOVED_NOT_MOVED)) &&
      (FTK_SQUARE_IS(game->board.square[FTK_A1], FTK_TYPE_ROOK, FTK_COLOR_WHITE,
                     FTK_MOVED_NOT_MOVED))) {
    output[outI] = 'Q';
    outI++;
    castle = 0;
  }
  if ((FTK_SQUARE_IS(game->board.square[FTK_E8], FTK_TYPE_KING, FTK_COLOR_BLACK,
                     FTK_MOVED_NOT_MOVED)) &&
      (FTK_SQUARE_IS(game->board.square[FTK_H8], FTK_TYPE_ROOK, FTK_COLOR_BLACK,
                     FTK_MOVED_NOT_MOVED))) {
    output[outI] = 'k';
    outI++;
    castle = 0;
  }
  if ((FTK_SQUARE_IS(game->board.square[FTK_E8], FTK_TYPE_KING, FTK_COLOR_BLACK,
                     FTK_MOVED_NOT_MOVED)) &&
      (FTK_SQUARE_IS(game->board.square[FTK_A8], FTK_TYPE_ROOK, FTK_COLOR_BLACK,
                     FTK_MOVED_NOT_MOVED))) {
    output[outI] = 'q';
    outI++;
    castle = 0;
  }
  if (castle == 1) {
    output[outI] = '-';
    outI++;
  }
  output[outI] = ' ';
  outI++;
  if (game->ep != FTK_XX) {
    char ep[8];
    ftk_position_to_string(game->ep, ep);
    output[outI] = ep[0];
    outI++;
    output[outI] = ep[1];
    outI++;
  } else {
    output[outI] = '-';
    outI++;
  }
  output[outI] = ' ';
  outI++;
  int digits = 0;
  int num = game->half_move;
  if (game->half_move != 0) {
    while (num != 0) {
      digits++;
      num /= 10;
    }
    for (int i = 0; i < digits; i++) {
      num = game->half_move;
      for (int j = i + 1; j < digits; j++) {
        num /= 10;
      }
      output[outI] = num % 10 + '0';
      outI++;
    }
  } else {
    output[outI] = '0';
    outI++;
  }
  output[outI] = ' ';
  outI++;
  digits = 0;
  if (game->full_move != 0) {
    num = game->full_move;
    while (num != 0) {
      digits++;
      num /= 10;
    }
    for (int i = 0; i < digits; i++) {
      num = game->full_move;
      for (int j = i + 1; j < digits; j++) {
        num /= 10;
      }
      output[outI] = num % 10 + '0';
      outI++;
    }
  } else {
    output[outI] = '0';
    outI++;
  }
  output[outI] = ' ';
  outI++;
}

ftk_result_e ftk_create_game_from_fen_string(ftk_game_s *game, const char *fen) {

  ftk_result_e ret_val = FTK_SUCCESS;
  ftk_string_index_t inI = 0;

  ftk_position_t squareI;
  
  int i;

  ftk_begin_standard_game(game);
  ftk_clear_board(&game->board);

  for(i = 0; i < 8; i++){
    squareI = 56 - 8*i;
    while(fen[inI] != '/' && fen[inI] != ' '){
      if(fen[inI] > '0' && fen[inI] <= '8'){            
        squareI += (fen[inI] - '0');
      }
      else{
        game->board.square[squareI] = ftk_char_to_square(fen[inI]);
        if(game->board.square[squareI].type == FTK_TYPE_PAWN)
        {
          if((game->board.square[squareI].color == FTK_COLOR_WHITE) && (i == 6))
          {
            game->board.square[squareI].moved = FTK_MOVED_NOT_MOVED;
          }
          else if((game->board.square[squareI].color == FTK_COLOR_BLACK) && (i == 1)){
            game->board.square[squareI].moved = FTK_MOVED_NOT_MOVED;
          }
          else{
            game->board.square[squareI].moved = FTK_MOVED_HAS_MOVED;
          }
        }
        else 
        {
          game->board.square[squareI].moved = FTK_MOVED_HAS_MOVED;
        }
        squareI++;
      }
      if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }
    }
    if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }
  }

  if(fen[inI] == 'w')
    game->turn = FTK_COLOR_WHITE;
  else
    game->turn = FTK_COLOR_BLACK;

  if(false == ftk_increment_string_index(fen, &inI, 2)) { return FTK_FAILURE; }

  if(fen[inI] == '-'){
    if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }
  }
  else{
    while(fen[inI] != ' '){
      switch (fen[inI]){
        case 'K':
          game->board.square[FTK_E1].moved = FTK_MOVED_NOT_MOVED;
          game->board.square[FTK_H1].moved = FTK_MOVED_NOT_MOVED;
          break;
        case 'Q':
          game->board.square[FTK_E1].moved = FTK_MOVED_NOT_MOVED; 
          game->board.square[FTK_A1].moved = FTK_MOVED_NOT_MOVED; 
          break;
        case 'k':
          game->board.square[FTK_E8].moved = FTK_MOVED_NOT_MOVED; 
          game->board.square[FTK_H8].moved = FTK_MOVED_NOT_MOVED; 
          break;
        case 'q':
          game->board.square[FTK_E8].moved = FTK_MOVED_NOT_MOVED; 
          game->board.square[FTK_A8].moved = FTK_MOVED_NOT_MOVED; 
          break;
        default:
          break;
      }
      if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }
    }
  }
  if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }

  if(fen[inI] != '-'){
    char pos[3];
    pos[0] = fen[inI];
    inI++;
    pos[1] = fen[inI];
    inI++;
    pos[2] = 0;
    game->ep = ftk_string_to_position(pos);
  }
  else{
    if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }
  }
  if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }
  int count = 0;
  while(fen[inI] >= '0' && fen[inI] <= '9'){
    count = (count*10) + (fen[inI] - '0');
    if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }
  }
  game->half_move = count;

  if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }

  count = 0;
  while(fen[inI] >= '0' && fen[inI] <= '9'){
    count = (count*10) + (fen[inI] - '0');
    if(false == ftk_increment_string_index(fen, &inI, 1)) { return FTK_FAILURE; }
  }
  game->full_move = count;

  ftk_update_board_masks(game);

  return ret_val;
}

/**
 * @brief Converts move to xboard string
 * 
 * @param move 
 * @param output buffer for output string (expect size >= FTK_MOVE_STRING_SIZE)
 * @return ftk_result_e 
 */
ftk_result_e ftk_move_to_xboard_string(ftk_move_s *move, char * output)
{
  ftk_result_e ret_val = FTK_SUCCESS;

  assert(output != NULL);

  memset(output, 0, FTK_MOVE_STRING_SIZE*sizeof(char));

  ftk_position_to_string(move->source, &output[0]);
  ftk_position_to_string(move->target, &output[2]);

  output[4] = ftk_type_to_char(move->pawn_promotion);

  if('X' == output[4])
  {
    output[4] = '\0';
  }

  return ret_val;
}