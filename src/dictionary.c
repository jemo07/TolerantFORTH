#include "dictionary.h"

// Define the maximum number of dictionary entries
#define MAX_ENTRIES 1000

// Define the maximum length of a word
#define MAX_WORD_LENGTH 32

// Structure representing a dictionary entry
typedef struct {
    char word[MAX_WORD_LENGTH];
    void (*code)();
} DictionaryEntry;

// Array to store the dictionary entries
static DictionaryEntry dictionary[MAX_ENTRIES];

// Variable to keep track of the number of entries in the dictionary
static int numEntries = 0;

// Function to add a new word to the dictionary
void dictionary_add(const char* word, void (*code)()) {
    if (numEntries < MAX_ENTRIES) {
        // Copy the word and code into the dictionary entry
        strcpy(dictionary[numEntries].word, word);
        dictionary[numEntries].code = code;
        numEntries++;
    } else {
        // Handle error: dictionary is full
    }
}

// Function to find the code associated with a word in the dictionary
void (*dictionary_find(const char* word))() {
    for (int i = 0; i < numEntries; i++) {
        if (strcmp(dictionary[i].word, word) == 0) {
            return dictionary[i].code;
        }
    }
    return NULL;  // Word not found in dictionary
}
