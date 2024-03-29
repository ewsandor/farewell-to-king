cmake_minimum_required (VERSION 3.18)
project (farewelltoking)

option (INCLUDE_STR "Build farewell_to_king_strings, includes operations to generate formatted strings." ON)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFTK_DEBUG_BUILD")

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(farewell_to_king_source src/farewell_to_king.c
                            src/farewell_to_king_bitops.c
                            src/farewell_to_king_board.c
                            src/farewell_to_king_mask.c)
if(INCLUDE_STR)
  list(APPEND farewell_to_king_source src/farewell_to_king_strings.c)
endif()

add_library(farewelltoking STATIC ${farewell_to_king_source})
target_include_directories(farewelltoking PUBLIC include)

add_library(farewelltoking-shared SHARED ${farewell_to_king_source})
target_include_directories(farewelltoking-shared PUBLIC include)

add_executable(farewelltoking-test test/farewell_to_king_test.c)
target_link_libraries(farewelltoking-test farewelltoking)
enable_testing()
add_test("Fischer-Spassky_1972_Game-6" bash -c "diff -u ../test/fischer-spassky_1972_game6.ftk_key <(cat ../test/fischer-spassky_1972_game6.ftk_test | ./farewelltoking-test)")