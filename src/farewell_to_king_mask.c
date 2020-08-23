/*
 farewell_to_king_mask.c
 Farewell To King - Chess Library
 Edward Sandor
 January 2015 - 2020
 
 Conatains implementation of all methods used to generate and manipulate board masks.
*/

#include "farewell_to_king_mask.h"
#include "farewell_to_king_types.h"

ftk_board_mask_t ftk_build_type_mask(const ftk_board_s *board, ftk_type_e type) 
{
  ftk_board_mask_t mask = 0;

  int i;
  ftk_board_mask_t biterator = 1ULL;

  for (i = 0; i < FTK_STD_BOARD_SIZE; i++) {
    if (board->square[i].type == type) {
      mask = mask | biterator;
    }
    biterator = (biterator << 1);
  }

  return mask;
}

ftk_board_mask_t ftk_build_color_mask(const ftk_board_s *board, ftk_color_e color) 
{
  ftk_board_mask_t mask = 0;

  int i;
  ftk_board_mask_t biterator = 1ULL;

  for (i = 0; i < FTK_STD_BOARD_SIZE; i++) {
    if (board->square[i].color == color) {
      mask = mask | biterator;
    }
    biterator = (biterator << 1);
  }

  return mask;
}

ftk_board_mask_t ftk_build_board_mask(const ftk_board_s *board) {
  ftk_board_mask_t mask = 0;

  int i;
  ftk_board_mask_t biterator = 1ULL;
  for (i = 0; i < FTK_STD_BOARD_SIZE; i++) {
    if (board->square[i].type != FTK_TYPE_EMPTY) {
      mask = mask | biterator;
    }
    biterator = (biterator << 1);
  }

  return mask;
}

void ftk_build_all_masks(ftk_board_s *board)
{
  board->board_mask = 0;
  board->white_mask = 0;
  board->black_mask = 0;
  board->pawn_mask = 0;
  board->knight_mask = 0;
  board->bishop_mask = 0;
  board->rook_mask = 0;
  board->queen_mask = 0;
  board->king_mask = 0;

  int i;
  ftk_board_mask_t biterator = 1ULL;
  for(i=0;i<FTK_STD_BOARD_SIZE;i++)
  {
    if(board->square[i].type != FTK_TYPE_EMPTY)
    {
      board->board_mask |= biterator;
      if(board->square[i].color == FTK_COLOR_WHITE)
      {
        board->white_mask |= biterator;
      }
      else
      {
        board->black_mask |= biterator;
      }
      switch(board->square[i].type){
        case FTK_TYPE_PAWN:
          board->pawn_mask   |= biterator;
          break;
        case FTK_TYPE_KNIGHT:
          board->knight_mask |= biterator;
          break;
        case FTK_TYPE_BISHOP:
          board->bishop_mask |= biterator;
          break;
        case FTK_TYPE_ROOK:
          board->rook_mask   |= biterator;
          break;
        case FTK_TYPE_QUEEN:
          board->queen_mask  |= biterator;
          break;
        case FTK_TYPE_KING:
          board->king_mask   |= biterator;
          break;
        default:
            break;
      }
    }
    biterator = biterator << 1;
  }
}

ftk_board_mask_t ftk_build_move_mask_raw(ftk_square_s square, ftk_board_mask_t board_mask, ftk_board_mask_t opponent_mask, ftk_position_t position, ftk_position_t *ep)
{
  ftk_board_mask_t mask = 0;

  if (square.type == FTK_TYPE_PAWN) 
  {
    int direction = (square.color == FTK_COLOR_WHITE)
                        ? 1
                        : -1;             // change direction based on color
    int test = position + direction * 8; // forward/back 1

    if (test < FTK_STD_BOARD_SIZE && test >= 0 &&
        ((board_mask & 1ULL << test) == 0)) {
      mask |= (1ULL << test);

      test = position + direction * 16; // forward/back 2

      if ((square.moved == FTK_MOVED_NOT_MOVED) && test < FTK_STD_BOARD_SIZE &&
          test >= 0 && ((board_mask & 1ULL << test) == 0)) {
        mask |= (1ULL << test);
      }
    }
    test = position + direction * 9; // capture up-right/down-left diagnol
    if (test < FTK_STD_BOARD_SIZE && test >= 0 &&
        ((opponent_mask & 1ULL << test) != 0 || test == *ep)) {
      mask |= (1ULL << test);
    }
    test = position + direction * 7; // capture up-left/down-right diagnol
    if (test < FTK_STD_BOARD_SIZE && test >= 0 &&
        ((opponent_mask & 1ULL << test) != 0 || test == *ep)) {
      mask |= (1ULL << test);
    }

  } 
  else if (square.type == FTK_TYPE_KNIGHT) 
  {
    // generate mask if square is a knight
    int test = position + 17;  // up two and right one
    if (test < FTK_STD_BOARD_SIZE && test >= 0 && (test % 8) > (position % 8) &&
        ((board_mask & 1ULL << test) == 0 ||
         (opponent_mask & 1ULL << test) != 0)) {
      mask |= (1ULL << test);
    }
    test = position - 17; // down two and left one
    if (test < FTK_STD_BOARD_SIZE && test >= 0 && (test % 8) < (position % 8) &&
        ((board_mask & 1ULL << test) == 0 ||
         (opponent_mask & 1ULL << test) != 0)) {
      mask |= (1ULL << test);
    }
    test = position + 15; // up two and left one
    if (test < FTK_STD_BOARD_SIZE && test >= 0 && (test % 8) < (position % 8) &&
        ((board_mask & 1ULL << test) == 0 ||
         (opponent_mask & 1ULL << test) != 0)) {
      mask |= (1ULL << test);
    }
    test = position - 15; // down two and right one
    if (test < FTK_STD_BOARD_SIZE && test >= 0 && (test % 8) > (position % 8) &&
        ((board_mask & 1ULL << test) == 0 ||
         (opponent_mask & 1ULL << test) != 0)) {
      mask |= (1ULL << test);
    }
    test = position + 10; // right two and up one
    if (test < FTK_STD_BOARD_SIZE && test >= 0 && (test % 8) > (position % 8) &&
        ((board_mask & 1ULL << test) == 0 ||
         (opponent_mask & 1ULL << test) != 0)) {
      mask |= (1ULL << test);
    }
    test = position - 10; // left two and down one
    if (test < FTK_STD_BOARD_SIZE && test >= 0 && (test % 8) < (position % 8) &&
        ((board_mask & 1ULL << test) == 0 ||
         (opponent_mask & 1ULL << test) != 0)) {
      mask |= (1ULL << test);
    }
    test = position + 6; // left two and up one
    if (test < FTK_STD_BOARD_SIZE && test >= 0 && (test % 8) < (position % 8) &&
        ((board_mask & 1ULL << test) == 0 ||
         (opponent_mask & 1ULL << test) != 0)) {
      mask |= (1ULL << test);
    }
    test = position - 6; // right two and down one
    if (test < FTK_STD_BOARD_SIZE && test >= 0 && (test % 8) > (position % 8) &&
        ((board_mask & 1ULL << test) == 0 ||
         (opponent_mask & 1ULL << test) != 0)) {
      mask |= (1ULL << test);
    }
  } 
  else if ((square.type == FTK_TYPE_BISHOP) ||
             (square.type == FTK_TYPE_ROOK) ||
             (square.type == FTK_TYPE_QUEEN) ||
             (square.type == FTK_TYPE_KING)) 
  {
    if (square.type != FTK_TYPE_ROOK) 
    {
      int test = position + 9; // up-right diagnol
      while (test < FTK_STD_BOARD_SIZE && test >= 0 &&
             (test % 8) > (position % 8) &&
             ((board_mask & 1ULL << test) == 0 ||
              (opponent_mask & 1ULL << test) != 0)) {
        mask |= (1ULL << test);
        if (square.type == FTK_TYPE_KING ||
            (board_mask & 1ULL << test) != 0)
          break;
        test += 9;
      }
      test = position - 9; // down-left diagnol
      while (test < FTK_STD_BOARD_SIZE && test >= 0 &&
             (test % 8) < (position % 8) &&
             ((board_mask & 1ULL << test) == 0 ||
              (opponent_mask & 1ULL << test) != 0)) {
        mask |= (1ULL << test);
        if (square.type == FTK_TYPE_KING ||
            (board_mask & 1ULL << test) != 0)
          break;
        test -= 9;
      }
      test = position + 7; // up-left diagnol
      while (test < FTK_STD_BOARD_SIZE && test >= 0 &&
             (test % 8) < (position % 8) &&
             ((board_mask & 1ULL << test) == 0 ||
              (opponent_mask & 1ULL << test) != 0)) {
        mask |= (1ULL << test);
        if (square.type == FTK_TYPE_KING ||
            (board_mask & 1ULL << test) != 0)
          break;
        test += 7;
      }
      test = position - 7; // down-right diagnol
      while (test < FTK_STD_BOARD_SIZE && test >= 0 &&
             (test % 8) > (position % 8) &&
             ((board_mask & 1ULL << test) == 0 ||
              (opponent_mask & 1ULL << test) != 0)) {
        mask |= (1ULL << test);
        if (square.type == FTK_TYPE_KING ||
            (board_mask & 1ULL << test) != 0)
          break;
        test -= 7;
      }
    }
    if (square.type != FTK_TYPE_BISHOP) {
      int test = position + 1; // right horizontal
      while (test < FTK_STD_BOARD_SIZE && test >= 0 &&
             (test % 8) > (position % 8) &&
             ((board_mask & 1ULL << test) == 0 ||
              (opponent_mask & 1ULL << test) != 0)) {
        mask |= (1ULL << test);
        if (square.type == FTK_TYPE_KING ||
            (board_mask & 1ULL << test) != 0)
          break;
        test += 1;
      }
      test = position - 1; // left horizontal
      while (test < FTK_STD_BOARD_SIZE && test >= 0 &&
             (test % 8) < (position % 8) &&
             ((board_mask & 1ULL << test) == 0 ||
              (opponent_mask & 1ULL << test) != 0)) {
        mask |= (1ULL << test);
        if (square.type == FTK_TYPE_KING ||
            (board_mask & 1ULL << test) != 0)
          break;
        test -= 1;
      }
      test = position + 8; // up vertical
      while (test < FTK_STD_BOARD_SIZE && test >= 0 &&
             ((board_mask & 1ULL << test) == 0 ||
              (opponent_mask & 1ULL << test) != 0)) {
        mask |= (1ULL << test);
        if (square.type == FTK_TYPE_KING ||
            (board_mask & 1ULL << test) != 0)
          break;
        test += 8;
      }
      test = position - 8; // down vertical
      while (test < FTK_STD_BOARD_SIZE && test >= 0 &&
             ((board_mask & 1ULL << test) == 0 ||
              (opponent_mask & 1ULL << test) != 0)) {
        mask |= (1ULL << test);
        if (square.type == FTK_TYPE_KING ||
            (board_mask & 1ULL << test) != 0)
          break;
        test -= 8;
      }
    }
  }

  return mask;
}
ftk_board_mask_t ftk_build_move_mask(const ftk_board_s *board, ftk_position_t position, ftk_position_t *ep)
{
  ftk_square_s     square        = board->square[position];
  ftk_board_mask_t opponent_mask = (FTK_COLOR_WHITE == square.color) ? board->black_mask : board->white_mask;

  return ftk_build_move_mask_raw(square, board->board_mask, opponent_mask, position, ep);
}

ftk_board_mask_t ftk_build_path_mask(ftk_square_s square, ftk_position_t target, ftk_position_t source, ftk_board_mask_t moves) 
{
  ftk_board_mask_t mask = 0;

  if ((moves & (1ULL << target)) != 0) 
  {
    switch (square.type) 
    {
      case FTK_TYPE_PAWN:
      {
        if ((target - source) == 16) 
        {
          mask = ((1ULL << target) | (1ULL << (target - 8)));
        } 
        else if ((source - target) == 16) 
        {
          mask = ((1ULL << target) | (1ULL << (target + 8)));
        } 
        else 
        {
          mask = (1ULL << target);
        }
        break;
      }
      case FTK_TYPE_KNIGHT:
      case FTK_TYPE_KING:
      {
        mask = (1ULL << target);
        break;
      }
      case FTK_TYPE_BISHOP:
      case FTK_TYPE_ROOK:
      case FTK_TYPE_QUEEN:
      {
        if ((target - source) % 9 == 0) 
        {
          char i;
          for (i = target; i > source; i = i - 9)
            mask |= (1ULL << i);

          for (i = target; i < source; i = i + 9)
            mask |= (1ULL << i);
        } 
        else if ((target - source) % 7 == 0) 
        {
          char i;
          for (i = target; i > source; i = i - 7)
            mask |= (1ULL << i);

          for (i = target; i < source; i = i + 7)
            mask |= (1ULL << i);
        } 
        else if ((target - source) % 8 == 0) 
        {
          char i;
          for (i = target; i > source; i = i - 8)
            mask |= (1ULL << i);
          for (i = target; i < source; i = i + 8)
            mask |= (1ULL << i);
        } 
        else 
        {
          char i;
          for (i = target; i > source; i = i - 1)
            mask |= (1ULL << i);

          for (i = target; i < source; i = i + 1)
            mask |= (1ULL << i);
        }
        break;
      }
      default:
      {
        break;
      }
    }
  }

  return mask;
}

ftk_check_e ftk_strip_check(ftk_board_s *board, ftk_color_e turn)
{
  ftk_check_e check = FTK_CHECK_NO_CHECK;
  ftk_position_t   i,j;
  ftk_position_t   ep;
  ftk_board_mask_t temp_mask;
  ftk_board_mask_t path;
  ftk_board_mask_t cross;
  ftk_board_mask_t turn_mask = (FTK_COLOR_WHITE == turn) ? board->white_mask : board->black_mask;
  ftk_board_mask_t opponent_mask = (FTK_COLOR_WHITE == turn) ? board->black_mask : board->white_mask;
  ftk_board_mask_t move_mask_no_opponent;
  ftk_position_t   protecting;
  ftk_position_t   king_position;
  FTK_MASK_TO_POSITION(board->king_mask & turn_mask, king_position);

  for (i = 0; i < FTK_STD_BOARD_SIZE; i++) 
  {
    /* Walk through moves of the entire board */
    if(board->square[i].type != FTK_TYPE_EMPTY && board->square[i].color != turn)
    {
      if(FTK_TYPE_PAWN == board->square[i].type)
      {
        /* Ignore forward moves for Pawns as they can only capture diagnally */
        temp_mask = board->move_mask[i];

        if(FTK_COLOR_WHITE == board->square[i].color)
        {
          if(i+FTK_STD_BOARD_ROWS < FTK_STD_BOARD_SIZE)
          {
            temp_mask &= ~FTK_POSITION_TO_MASK(i+FTK_STD_BOARD_ROWS);
          }
          if(i+(2*FTK_STD_BOARD_ROWS) < FTK_STD_BOARD_SIZE)
          {
            temp_mask &= ~FTK_POSITION_TO_MASK(i+(2*FTK_STD_BOARD_ROWS));
          }
        }
        else 
        {
          if(i >= FTK_STD_BOARD_ROWS)
          {
            temp_mask &= ~FTK_POSITION_TO_MASK(i-FTK_STD_BOARD_ROWS);
          }
          if(i >= (2*FTK_STD_BOARD_ROWS))
          {
            temp_mask &= ~FTK_POSITION_TO_MASK(i-(2*FTK_STD_BOARD_ROWS));
          }
        }

        /* Do not allow King to move into a valid oppenet's piece valid move */
        board->move_mask[king_position] &= ~temp_mask;
      }
      else
      {
        /* Do not allow King to move into a valid oppenet's piece valid move*/
        board->move_mask[king_position] &= ~board->move_mask[i];
      }

      /* Square under test is opponents piece */
      if (board->move_mask[i] & FTK_POSITION_TO_MASK(king_position)) 
      {
        /* If opponents piece can move to the King's square, the King is in check */
        check = FTK_CHECK_IN_CHECK;

        /* Consider path to king */
        path = ftk_build_path_mask(board->square[i], king_position, i, board->move_mask[i]);
        for (j = 0; j < FTK_STD_BOARD_SIZE; j++) 
        {
          if(board->square[j].type != FTK_TYPE_EMPTY && board->square[j].type != FTK_TYPE_KING && board->square[j].color == turn)
          {
            /* Walk entire board and clear all moves that do not clear check */
            /* If piece is not the king, remove all moves that do not block check or capture attacker */
            board->move_mask[j] &= (path | FTK_POSITION_TO_MASK(i));
          }
        }
      }
      /*Build opponents move mask as if current-turn player's pieces do not exist */
      move_mask_no_opponent = ftk_build_move_mask_raw(board->square[i], opponent_mask, turn_mask, i, &ep);
      /* Get path to players king */
      path = ftk_build_path_mask(board->square[i], king_position, i, move_mask_no_opponent);

      if (path) 
      {
        /* Overlap between piece's path to King and current player's pieces */
        cross = path & turn_mask;

        /* Assume no protecting piece */
        protecting = FTK_XX;

        for (j = 0; j < FTK_STD_BOARD_SIZE; j++) 
        {
          if ((cross & FTK_POSITION_TO_MASK(j)) && (board->square[j].type != FTK_TYPE_KING) )
          {
            if (protecting != FTK_XX) 
            {
              /* If a piece is already blocking, this is the second blocking piece.
                This is not a potential uncovered check, clear blocking square and break loop */
              protecting = FTK_XX;
              break;
            }
            /* Piece is protecting King */
            protecting = j;
          }
        }
        if (protecting != FTK_XX) 
        {
          /* If piece is blocking check, only allow moves on the pieces path */
          board->move_mask[protecting] = board->move_mask[protecting] & (path | FTK_POSITION_TO_MASK(i));
        }
      }
    }
  }

  return check;
}

void ftk_add_castle(ftk_board_s *board, ftk_color_e turn) 
{
  ftk_castle_mask_t castle = FTK_CASTLE_NONE;
  ftk_board_mask_t QS;
  ftk_board_mask_t KS;
  ftk_board_mask_t CASTLE_SQUARES;
  ftk_position_t i = 0;

  if(turn == FTK_COLOR_WHITE)
  {
    if(FTK_SQUARE_IS(board->square[FTK_E1], FTK_TYPE_KING, FTK_COLOR_WHITE, FTK_MOVED_NOT_MOVED)){
        /* White King has not moved, consider for castling */
        castle = FTK_CASTLE_KING_SIDE_WHITE | FTK_CASTLE_QUEEN_SIDE_WHITE;
    }

    QS = FTK_POSITION_TO_MASK(FTK_C1) | FTK_POSITION_TO_MASK(FTK_D1);
    KS = FTK_POSITION_TO_MASK(FTK_F1) | FTK_POSITION_TO_MASK(FTK_G1);
    CASTLE_SQUARES = QS | KS | FTK_POSITION_TO_MASK(FTK_E1);

    if (((FTK_POSITION_TO_MASK(FTK_B1) | QS) & (board->white_mask | board->black_mask)) != 0 ||
        !FTK_SQUARE_IS(board->square[FTK_A1], FTK_TYPE_ROOK, FTK_COLOR_WHITE, FTK_MOVED_NOT_MOVED)) 
    {
      /* Rook has moved or squared between Rook and King are not emtpy (Queen
       * side) */
      castle &= ~FTK_CASTLE_QUEEN_SIDE_WHITE;
    }
    if ((KS & (board->white_mask | board->black_mask)) != 0 ||
        !FTK_SQUARE_IS(board->square[FTK_H1], FTK_TYPE_ROOK, FTK_COLOR_WHITE, FTK_MOVED_NOT_MOVED))
    {
      /* Rook has moved or squared between Rook and King are not emtpy (King
       * side) */
      castle &= ~FTK_CASTLE_KING_SIDE_WHITE;
    }

    for(i = 0; i < FTK_STD_BOARD_SIZE && castle != 0; i++)
    {
      /* Check move mask of all black squares if they conflict with white castling */
      if(board->square[i].color == FTK_COLOR_BLACK)
      {
        if (board->move_mask[i] & FTK_POSITION_TO_MASK(FTK_E1))
        {
          /* King is in check */
          castle &= ~FTK_CASTLE_KING_SIDE_WHITE;
          castle &= ~FTK_CASTLE_QUEEN_SIDE_WHITE;
        }
        if (board->move_mask[i] & QS)
        {
          /* King passes through check (Queen side) */
          castle &= ~FTK_CASTLE_QUEEN_SIDE_WHITE;
        }
        if (board->move_mask[i] & KS)
        {
          /* King passes through check (King side) */
          castle &= ~FTK_CASTLE_KING_SIDE_WHITE;
        }
      }
    }
  }
  else
  {
    if( FTK_SQUARE_IS(board->square[FTK_E8], FTK_TYPE_KING, FTK_COLOR_BLACK, FTK_MOVED_NOT_MOVED) )
    {
      /* Black King has not moved, consider for castling */
      castle = FTK_CASTLE_KING_SIDE_BLACK | FTK_CASTLE_QUEEN_SIDE_BLACK;
    }

    QS = FTK_POSITION_TO_MASK(FTK_C8) | FTK_POSITION_TO_MASK(FTK_D8);
    KS = FTK_POSITION_TO_MASK(FTK_F8) | FTK_POSITION_TO_MASK(FTK_G8);
    CASTLE_SQUARES = QS | KS | FTK_POSITION_TO_MASK(FTK_E8);

    if (((FTK_POSITION_TO_MASK(FTK_B8) | QS) & (board->white_mask | board->black_mask)) != 0 ||
        !FTK_SQUARE_IS(board->square[FTK_A8], FTK_TYPE_ROOK, FTK_COLOR_BLACK, FTK_MOVED_NOT_MOVED)) 
    {
      /* Rook has moved or squared between Rook and King are not emtpy (Queen
       * side) */
      castle &= ~FTK_CASTLE_QUEEN_SIDE_BLACK;
    }
    if ((KS & (board->white_mask | board->black_mask)) != 0 ||
        !FTK_SQUARE_IS(board->square[FTK_H8], FTK_TYPE_ROOK, FTK_COLOR_BLACK, FTK_MOVED_NOT_MOVED))
    {
      /* Rook has moved or squared between Rook and King are not emtpy (King
       * side) */
      castle &= ~FTK_CASTLE_KING_SIDE_BLACK;
    }

    for(i = 0; i < FTK_STD_BOARD_SIZE && castle != 0; i++)
    {
      /* Check move mask of all white squares if they conflict with black castling */
      if(board->square[i].color == FTK_COLOR_WHITE)
      {
        if (board->move_mask[i] & FTK_POSITION_TO_MASK(FTK_E8))
        {
          /* King is in check */
          castle &= ~FTK_CASTLE_KING_SIDE_BLACK;
          castle &= ~FTK_CASTLE_QUEEN_SIDE_BLACK;
        }
        if (board->move_mask[i] & QS)
        {
          /* King passes through check (Queen side) */
          castle &= ~FTK_CASTLE_QUEEN_SIDE_BLACK;
        }
        if (board->move_mask[i] & KS)
        {
          /* King passes through check (King side) */
          castle &= ~FTK_CASTLE_KING_SIDE_BLACK;
        }
      }
    }
  }
  /* Add castling to King's valid move masks if allowed (Rook already has mask set implicitly) */
  if(castle & FTK_CASTLE_KING_SIDE_WHITE)
  {
      board->move_mask[FTK_E1] |= FTK_POSITION_TO_MASK(FTK_G1);
  }
  if(castle & FTK_CASTLE_QUEEN_SIDE_WHITE)
  {
      board->move_mask[FTK_E1] |= FTK_POSITION_TO_MASK(FTK_C1);
  }
  if(castle & FTK_CASTLE_KING_SIDE_BLACK)
  {
      board->move_mask[FTK_E8] |= FTK_POSITION_TO_MASK(FTK_G8);
  }
  if(castle & FTK_CASTLE_QUEEN_SIDE_BLACK)
  {
      board->move_mask[FTK_E8] |= FTK_POSITION_TO_MASK(FTK_C8);
  }
}