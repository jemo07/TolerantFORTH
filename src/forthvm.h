#ifndef FORTHVM_H
#define FORTHVM_H

// Constants
#define MAX_STACK_SIZE 1024
#define MAX_DICTIONARY_SIZE 4096

// Forward declarations
struct Dictionary;
typedef struct Dictionary Dictionary;

// Function prototypes
void forthvm_init();
void forthvm_run();
void forthvm_push(int value);
int forthvm_pop();
void forthvm_execute(int token);
void forthvm_define_word(const char *name, void (*func)());
void forthvm_reset();

#endif /* FORTHVM_H */
