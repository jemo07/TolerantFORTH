#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "forthvm.h"

void dictionary_init();
void dictionary_define_word(const char#ifndef FORTHVM_H
#define FORTHVM_H

#include <stdint.h>

// Constants
#define MAX_STACK_SIZE 1024
#define MAX_DICTIONARY_SIZE 4096

// Forward declarations
struct Dictionary;
typedef struct Dictionary Dictionary;

// Definition of forthvm_word struct
typedef struct {
    const char* name;
    void (*func)();
} forthvm_word;

// Function prototypes
void forthvm_init();
void forthvm_run();
void forthvm_push(int32_t value);
int32_t forthvm_pop();
void forthvm_execute(int32_t token);
void forthvm_define_word(const char* name, void (*func)());
void forthvm_reset();

// Primitive function prototypes
void bye();
void qrx();
void txsto();
void next();
void dovar();
void docon();
void dolit();
void dolist();
void exitt();
void execu();
void donext();
void qbran();
void bran();
void store();
void at();
void cstor();
void cat();
void rfrom();
void rat();
void tor();
void drop();
void dup();
void swap();
void over();
void zless();
void andd();
void orr();
void xorr();
void uplus();
void nop();
void qdup();
void rot();
void ddrop();
void ddup();
void plus();
void inver();
void negat();
void dnega();
void subb();
void abss();
void great();
void less();
void equal();
void uless();
void ummod();
void msmod();
void slmod();
void mod();
void slash();
void umsta();
void star();
void mstar();
void ssmod();
void stasl();
void pick();
void pstor();
void dstor();
void dat();
void count();
void max();
void min();
void dup2();
void swap2();
void drop2();
void over2();
void pick2();
void rot2();
void minus();
void negate();
void max2();
void min2();

#endif /* FORTHVM_H */
 *name, forthvm_word_func func);
forthvm_word* dictionary_find_word(const char *name);

#endif // DICTIONARY_H

