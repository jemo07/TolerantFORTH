#ifndef FORTHVM_H
#define FORTHVM_H
#include <stdint.h>

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


// Primitive function declarations
void bye(void);
void qrx(void);
void txsto(void);
void next(void);
void dovar(void);
void docon(void);
void dolit(void);
void dolist(void);
void exitt(void);
void execu(void);
void donext(void);
void qbran(void);
void bran(void);
void store(void);
void at(void);
void cstor(void);
void cat(void);
void rfrom(void);
void rat(void);
void tor(void);
void drop(void);
void dup(void);
void swap(void);
void over(void);
void zless(void);
void andd(void);
void orr(void);
void xorr(void);
void uplus(void);
void nop(void);
void qdup(void);
void rot(void);
void ddrop(void);
void ddup(void);
void plus(void);
void inver(void);
void negat(void);
void dnega(void);
void subb(void);
void abss(void);
void great(void);
void less(void);
void equal(void);
void uless(void);
void ummod(void);
void msmod(void);
void slmod(void);
void mod(void);
void slash(void);
void umsta(void);
void star(void);
void mstar(void);
void ssmod(void);
void stasl(void);
void pick(void);
void pstor(void);
void dstor(void);
void dat(void);
void count(void);
void max(void);
void min(void);

// Array of primitive functions
void (*primitives[])() = {
    bye,   // 0
    qrx,   // 1
    txsto, // 2
    next,  // 3
    dovar, // 4
    docon, // 5
    dolit, // 6
    dolist,// 7
    exitt, // 8
    execu, // 9
    donext,// 10
    qbran, // 11
    bran,  // 12
    store, // 13
    at,    // 14
    cstor, // 15
    cat,   // 16
    rfrom, // 17
    rat,   // 18
    tor,   // 19
    drop,  // 20
    dup,   // 21
    swap,  // 22
    over,  // 23
    zless, // 24
    andd,  // 25
    orr,   // 26
    xorr,  // 27
    uplus, // 28
    nop,   // 29
    qdup,  // 30
    rot,   // 31
    ddrop, // 32
    ddup,  // 33
    plus,  // 34
    inver, // 35
    negat, // 36
    dnega, // 37
    subb,  // 38
    abss,  // 39
    great, // 40
    less,  // 41
    equal, // 42
    uless, // 43
    ummod, // 44
    msmod, // 45
    slmod, // 46
    mod,   // 47
    slash, // 48
    umsta, // 49
    star,  // 50
    mstar, // 51
    ssmod, // 52
    stasl, // 53
    pick,  // 54
    pstor, // 55
    dstor, // 56
    dat,   // 57
    count, // 58
    max,   // 59
    min    // 60
};

#endif /* FORTHVM_H */
