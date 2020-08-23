cmake_minimum_required (VERSION 3.16.3)

add_library (farewell_to_king SHARED farewell_to_king.c farewell_to_king_mask.c farewell_to_king_board.c)

if(INCLUDE_STR)
    add_library (farewell_to_king_strings SHARED farewell_to_king_strings.c)
    install (TARGETS farewell_to_king_strings DESTINATION lib)
    install (FILES ${HEADERS}/farewell_to_king_strings.h DESTINATION include)
endif (INCLUDE_STR)

install (TARGETS farewell_to_king DESTINATION lib)