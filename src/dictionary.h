#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "forthvm.h"

void dictionary_init();
void dictionary_define_word(const char *name, forthvm_word_func func);
forthvm_word* dictionary_find_word(const char *name);

#endif // DICTIONARY_H

