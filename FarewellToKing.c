//
//FarewellToKing.c
//FarewellToKing 0.0.2 - Chess Library
//Edward Sandor
//November 2014 - 2015
//
//Contains implementations of all methods for general game manipulation. 
//

#include "FarewellToKing.h"

void beginStandardGame(Game * game){

    standardBoard(game->board);

    game->ep       = XX;
    game->halfmove = 0;
    game->fullmove = 1;

    game->turn = WHITE;
        
    updateMasks(game);
}
void updateMasks(Game * game){
    
    superMask(game->board, &game->boardM, &game->whiteM, &game->blackM, &game->pawnM, &game->knightM, &game->bishopM, &game->rookM, &game->queenM, &game->kingM, &game->spareM);    

    POS_T i;
    for(i = 0; i < STDBOARD; i++){
        TURN_T colorp = (game->board[i] & COLORMASK);
        if((game->board[i] & TYPEMASK) == KING){

            if((game->board[i] & COLORMASK) == WHITE){
                game->wKing = i;
            }
            else{
                game->bKing = i;
            }
        }
        game->moveM[i] = moveMask(&game->board[i], &i, (colorp == game->turn || ((game->board[i] & TYPEMASK) == PAWN))?(&game->boardM):((colorp != WHITE)?(&game->blackM):(&game->whiteM)), (colorp == WHITE)?(&game->blackM):(&game->whiteM), &game->ep);
    }

    stripCheck(game->board, game->moveM, (game->turn == WHITE)?(&game->whiteM):(&game->blackM), (game->turn == WHITE)?(&game->wKing):(&game->bKing));
    addCastle(game->board, game->moveM, &game->whiteM, &game->blackM, &game->turn);
}
void movePiece(Game * game, POS_T * target, POS_T * source){
    if((game->moveM[*source] & (1ULL << *target)) != 0 && (game->board[*source] & COLORMASK)==game->turn){

        if((game->turn & COLORMASK) == BLACK)
            game->fullmove++;

        game->halfmove++;

        if(((game->board[*target] & TYPEMASK) != EMPTY) || ((game->board[*source] & TYPEMASK) == PAWN))
            game->halfmove = 0;

        PIECE_T empty = EMPTY;

        if((game->board[*source] & TYPEMASK) == PAWN){

            if(*target == game->ep){
                if( (game->turn) == WHITE ){
                    game->board[game->ep - 8] = empty;
                }
                else{
                    game->board[game->ep + 8] = empty;
                }
            }

            if((*target - *source) / 8 == 2)
                game->ep = *source + 8;
            else if ((*target - *source) / 8 == -2)
                game->ep = *source - 8;
            else
                game->ep = XX;               
        }
        else{
            game->ep = XX;
        }
        
        if((game->board[*source] & TYPEMASK) == KING){
            if((*target - *source) == 2){
                game->board[*target - 1] = (game->board[*source + 3] & (TYPEMASK | COLORMASK)) | HASMOVED;
                game->board[*source + 3] = EMPTY;
            }
            if((*target - *source) == -2){
                game->board[*target + 1] = (game->board[*source - 4] & (TYPEMASK | COLORMASK)) | HASMOVED;
                game->board[*source - 4] = EMPTY;
            }
        }

        game->board[*source] = (game->board[*source] & (TYPEMASK | COLORMASK)) | HASMOVED;
        placePiece(game->board, &game->board[*source], target); 
        placePiece(game->board, &empty, source);
        game->turn = game->turn ^ BLACK;
        updateMasks(game);
    }
}
