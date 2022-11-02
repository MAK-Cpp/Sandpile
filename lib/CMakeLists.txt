add_library(bmppicture STATIC bmppicture.h bmppicture.cpp)
add_library(sandpile STATIC sandpile.h)
set_target_properties(sandpile PROPERTIES LINKER_LANGUAGE CXX)