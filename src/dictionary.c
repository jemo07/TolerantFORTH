#include "dictionary.h"
#include <stdlib.h>
#include <string.h>

// Structure representing a dictionary word entry
typedef struct {
    char name[MAX_WORD_LENGTH];
    forthvm_word_func func;
} forthvm_word_entry;

static forthvm_word_entry* dictionary = NULL;
static int num_entries = 0;

void dictionary_init() {
    // Initialize the dictionary
    dictionary = (forthvm_word_entry*)malloc(MAX_ENTRIES * sizeof(forthvm_word_entry));
    num_entries = 0;
}

void dictionary_define_word(const char *name, forthvm_word_func func) {
    // Add the word to the dictionary
    if (num_entries < MAX_ENTRIES) {
        strncpy(dictionary[num_entries].name, name, MAX_WORD_LENGTH - 1);
        dictionary[num_entries].func = func;
        num_entries++;
    } else {
        // Handle error: dictionary is full
        printf("Error: Dictionary is full. Cannot add word '%s'\n", name);
    }
}

forthvm_word* dictionary_find_word(const char *name) {
    // Search for the word in the dictionary
    for (int i = 0; i < num_entries; i++) {
        if (strcmp(dictionary[i].name, name) == 0) {
            forthvm_word* word = (forthvm_word*)malloc(sizeof(forthvm_word));
            strncpy(word->name, dictionary[i].name, MAX_WORD_LENGTH - 1);
            word->func = dictionary[i].func;
            return word;
        }
    }
    return NULL;  // Word not found in dictionary
}
