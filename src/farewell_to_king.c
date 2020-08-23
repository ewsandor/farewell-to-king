/*
 farewell_to_king.c
 FarewellToKing - Chess Library
 Edward Sandor
 November 2014 - 2020
 
 Contains implementations of all methods for general game manipulation. 
*/

#include "farewell_to_king.h"
#include "farewell_to_king_types.h"

void ftk_begin_standard_game(ftk_game_s *game) {

  ftk_set_standard_board(&game->board);

  game->ep = FTK_XX;
  game->halfmove = 0;
  game->fullmove = 1;

  game->turn = FTK_COLOR_WHITE;

  ftk_update_board_masks(game);
}
void ftk_update_board_masks(ftk_game_s *game) 
{
  ftk_build_all_masks(&game->board);

  ftk_position_t i;
  for(i = 0; i < FTK_STD_BOARD_SIZE; i++)
  {
    ftk_color_e colorp = (game->board.square[i].color);

    game->board.move_mask[i] = ftk_build_move_mask(&game->board, i, &game->ep);
  }

  ftk_strip_check( &game->board, game->turn);

  ftk_add_castle(&game->board, game->turn);
}
ftk_move_s ftk_move_piece(ftk_game_s *game, ftk_position_t target, ftk_position_t source, ftk_type_e pawn_promotion) 
{
  ftk_move_s move;
  ftk_square_s empty;
  FTK_SQUARE_CLEAR(empty);

  if((game->board.move_mask[source] & (1ULL << target)) != 0 && game->board.square[source].color == game->turn)
  {
    move.source         = source;
    move.target         = target;

    move.moved          = game->board.square[source];
    move.capture        = game->board.square[target];

    move.ep             = game->ep;
    move.pawn_promotion = FTK_TYPE_DONT_CARE;

    move.turn           = game->turn;
    move.fullmove       = game->fullmove;
    move.halfmove       = game->halfmove;

    if(game->turn == FTK_COLOR_BLACK)
    {
      game->fullmove++;
    }

    game->halfmove++;

    if((game->board.square[target].type != FTK_TYPE_EMPTY) || (game->board.square[source].type == FTK_TYPE_PAWN))
    {
      game->halfmove = 0;
    }

    if(game->board.square[source].type == FTK_TYPE_PAWN)
    {
      if(target == game->ep)
      {
        if( FTK_COLOR_WHITE == game->turn )
        {
          move.capture = game->board.square[game->ep - 8];
          FTK_SQUARE_CLEAR(game->board.square[game->ep - 8]);
        }
        else
        {
          move.capture = game->board.square[game->ep + 8];
          FTK_SQUARE_CLEAR(game->board.square[game->ep + 8]);
        }
      }

      if((target - source) / 8 == 2)
      {
        game->ep = source + 8;
      }
      else if ((target - source) / 8 == -2)
      {
        game->ep = source - 8;
      }
      else
      {
        game->ep = FTK_XX;               
      }

      if(0 == target / 8 ||
         7 == target / 8 )
      {
        if(FTK_TYPE_KNIGHT == pawn_promotion ||
           FTK_TYPE_BISHOP == pawn_promotion ||
           FTK_TYPE_ROOK   == pawn_promotion ||
           FTK_TYPE_QUEEN  == pawn_promotion )
        {
          game->board.square[source].type = pawn_promotion;
        }
        else 
        {
          game->board.square[source].type = FTK_TYPE_QUEEN;
        }
        move.pawn_promotion = game->board.square[source].type;
      }
    }
    else{
      game->ep = FTK_XX;
    }
    
    if(game->board.square[source].type == FTK_TYPE_KING)
    {
      if((target - source) == 2){
        /* Save old Rook */
        move.capture = game->board.square[source + 3];
        /* Move Rook */
        game->board.square[target - 1] = game->board.square[source + 3];
        game->board.square[target - 1].moved = FTK_MOVED_HAS_MOVED;
        /* Clear old Rook */
        FTK_SQUARE_CLEAR(game->board.square[source + 3]);
      }
      if((target - source) == -2){
        /* Save old Rook */
        move.capture = game->board.square[source - 4];
        /* Move Rook */
        game->board.square[target + 1] = game->board.square[source - 4];
        game->board.square[target + 1].moved = FTK_MOVED_HAS_MOVED;
        /* Clear old Rook */
        FTK_SQUARE_CLEAR(game->board.square[source - 4]);
      }
    }

    game->board.square[target] = game->board.square[source];
    game->board.square[target].moved = FTK_MOVED_HAS_MOVED;
    FTK_SQUARE_CLEAR(game->board.square[source]);
    game->turn = (FTK_COLOR_WHITE == game->turn)? FTK_COLOR_BLACK:FTK_COLOR_WHITE;
    ftk_update_board_masks(game);
  }
  else
  {
    move.source   = FTK_XX;
    move.target   = FTK_XX;

    move.moved    = empty;
    move.capture  = empty;

    move.ep       = FTK_XX;

    move.turn     = 0;
    move.fullmove = 0;
    move.halfmove = 0;
  }

  return move;
}

ftk_result_e ftk_move_forward(ftk_game_s *game, ftk_move_s *move) {

  if(move->target == FTK_XX || move->source == FTK_XX)
  {
    return FTK_FAILURE;
  }

  ftk_move_s newmove = ftk_move_piece(game, move->target, move->source, move->pawn_promotion);

  if(newmove.target == FTK_XX || newmove.source == FTK_XX)
  {
    return FTK_FAILURE;
  }

  return FTK_SUCCESS;
}

ftk_result_e ftk_move_backward(ftk_game_s *game, ftk_move_s *move) {

  if(move->target == FTK_XX && move->source == FTK_XX)
  {
    return FTK_FAILURE;
  }

  game->board.square[move->source] = move->moved;
  game->ep                         = move->ep;
  game->turn                       = move->turn;
  game->halfmove                   = move->halfmove;
  game->fullmove                   = move->fullmove;

  if(move->target == move->ep && FTK_TYPE_PAWN == move->moved.type)
  {
    if(move->turn == FTK_COLOR_WHITE)
    {
      /* Repalce captured Pawn */
      game->board.square[move->ep - 8] = move->capture;
    }
    else
    {
      /* Repalce captured Pawn */
      game->board.square[move->ep + 8] = move->capture;
    }
    /* Clear en pasant square */
    FTK_SQUARE_CLEAR(game->board.square[move->ep]);
  }
  else if(move->moved.type == FTK_TYPE_KING && (move->target - move->source) ==  2)
  {
    /* Reset caslte King side */
    /* Clear moved King and Rook */
    FTK_SQUARE_CLEAR(game->board.square[move->target]);
    FTK_SQUARE_CLEAR(game->board.square[move->target - 1]);
    /* Repalce Rook */
    game->board.square[move->source + 3] = move->capture;
  }    
  else if(move->moved.type == FTK_TYPE_KING && (move->target - move->source) == -2)
  {
    /* Reset caslte Queen side */
    /* Clear moved King and Rook */
    FTK_SQUARE_CLEAR(game->board.square[move->target]);
    FTK_SQUARE_CLEAR(game->board.square[move->target + 1]);
    /* Repalce Rook */
    game->board.square[move->source - 4] = move->capture;
  } 
  else
  {
    /* Restore target square for normal move*/
    game->board.square[move->target] = move->capture;
  }

  ftk_update_board_masks(game);

  return FTK_SUCCESS;
}

ftk_check_e ftk_check_for_check(const ftk_game_s *game)
{
  ftk_position_t i;
  ftk_check_e check_status = FTK_CHECK_NO_CHECK;
  ftk_board_mask_t player_king_mask = game->board.king_mask & ((FTK_COLOR_WHITE == game->turn)?game->board.white_mask:game->board.black_mask);

  for(i = 0; i < FTK_STD_BOARD_SIZE; i++)
  {
    if(game->board.move_mask[i] & player_king_mask)
    {
      check_status = FTK_CHECK_IN_CHECK;
      break;
    }
  }

  return check_status;
}

bool ftk_check_legal_moves(const ftk_game_s *game)
{
  bool ret_val = false;
  ftk_position_t i;

  for(i = 0; i < FTK_STD_BOARD_SIZE; i++)
  {
    if(game->board.square[i].color == game->turn && game->board.move_mask[i])
    {
      ret_val = true;
      break;
    }
  }

  return ret_val;
}

ftk_game_end_e ftk_check_for_game_end(const ftk_game_s *game)
{
  ftk_game_end_e game_end = FTK_END_NOT_OVER;

  if( false == ftk_check_legal_moves(game) )
  {
    game_end = (FTK_CHECK_IN_CHECK == ftk_check_for_check(game))?FTK_END_CHECKMATE:FTK_END_DRAW_STALEMATE;
  }
  else if (game->halfmove > FTK_DRAW_HALF_MOVES) {
    game_end = FTK_END_DRAW_FIFTY_MOVE_RULE;
  }

  return game_end;
}