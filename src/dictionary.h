#ifndef DICTIONARY_H
#define DICTIONARY_H

// Define the maximum length of a word
#define MAX_WORD_LENGTH 32

// Function to add a new word to the dictionary
void dictionary_add(const char* word, void (*code)());

// Function to find the code associated with a word in the dictionary
void (*dictionary_find(const char* word))();

#endif /* DICTIONARY_H */

