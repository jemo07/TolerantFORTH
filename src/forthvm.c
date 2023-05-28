#include <stdio.h>
#include <stdlib.h>
#include "forthvm.h"

// Stack and memory variables
int32_t rack[256] = { 0 };
int32_t stack[256] = { 0 };
int32_t top = 0;
int8_t RP = 0; // Return Stack Pointer
int8_t SP = 0; // Data Stack Pointer
int32_t UP = 0; // User Area Pointer
int32_t IP = 0; // Interpreter Pointer
int32_t WP = 0; // Word or Work Pointer
int32_t thread = 0;
int32_t data[16000] = {};
uint8_t* cData = (uint8_t*)data;

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

void forthvm_init() {
    // Initialize stack and memory
    top = 0;
    SP = -1;
    IP = 0;
    WP = 0;

    // Initialize dictionary
    for (int i = 0; i < MAX_DICTIONARY_SIZE; i++) {
        dictionary[i].name = NULL;
        dictionary[i].func = NULL;
    }

    // Define built-in words
    forthvm_define_word("BYE", bye);
    forthvm_define_word("?RX", qrx);
    forthvm_define_word("TX!", txsto);
    forthvm_define_word("NEXT", next);
    forthvm_define_word("DOVAR", dovar);
    forthvm_define_word("DOCON", docon);
    forthvm_define_word("DOLIT", dolit);
    forthvm_define_word("DOLIST", dolist);
    forthvm_define_word("EXIT", exitt);
    forthvm_define_word("EXECUTE", execu);
    forthvm_define_word("DOSTR", donext);
    forthvm_define_word("?BRANCH", qbran);
    forthvm_define_word("BRANCH", bran);
    forthvm_define_word("!", store);
    forthvm_define_word("@", at);
    forthvm_define_word("C!", cstor);
    forthvm_define_word("C@", cat);
    forthvm_define_word("R>", rfrom);
    forthvm_define_word("R@", rat);
    forthvm_define_word(">R", tor);
    forthvm_define_word("DROP", drop);
    forthvm_define_word("DUP", dup);
    forthvm_define_word("SWAP", swap);
    forthvm_define_word("OVER", over);
    forthvm_define_word("0<", zless);
    forthvm_define_word("AND", andd);
    forthvm_define_word("OR", orr);
    forthvm_define_word("XOR", xorr);
    forthvm_define_word("U+", uplus);
    forthvm_define_word("NOP", nop);
    forthvm_define_word("?DUP", qdup);
    forthvm_define_word("ROT", rot);
    forthvm_define_word("2DROP", ddrop);
    forthvm_define_word("2DUP", ddup);
    forthvm_define_word("+", plus);
    forthvm_define_word("INVERT", inver);
    forthvm_define_word("NEGATE", negat);
    forthvm_define_word("DNEGATE", dnega);
    forthvm_define_word("-", subb);
    forthvm_define_word("ABS", abss);
    forthvm_define_word(">", great);
    forthvm_define_word("<", less);
    forthvm_define_word("=", equal);
    forthvm_define_word("0<", zless);
    forthvm_define_word("UM/MOD", ummod);
    forthvm_define_word("MS/MOD", msmod);
    forthvm_define_word("SL/MOD", slmod);
    forthvm_define_word("MOD", mod);
    forthvm_define_word("/", slash);
    forthvm_define_word("UM*", umsta);
    forthvm_define_word("*", star);
    forthvm_define_word("M*", mstar);
    forthvm_define_word("S/MOD", ssmod);
    forthvm_define_word("STASL", stasl);
    forthvm_define_word("PICK", pick);
    forthvm_define_word("+!", pstor);
    forthvm_define_word("2!", dstor);
    forthvm_define_word("2@", dat);
    forthvm_define_word("COUNT", count);
    forthvm_define_word("MAX", max);
    forthvm_define_word("MIN", min);
}

void forthvm_run() {
    forthvm_reset();
    for (;;) {
        forthvm_execute(cData[IP++]);
    }
}

void forthvm_push(int value) {
    stack[++SP] = value;
}

int forthvm_pop() {
    return stack[SP--];
}

void forthvm_execute(int token) {
    primitives[token]();
}

void forthvm_define_word(const char *name, void (*func)()) {
    dictionary[DP].name = name;
    dictionary[DP].func = func;
    DP++;
}

void forthvm_reset() {
    IP = 0;
    SP = -1;
    top = 0;
}

// Primitive function implementations
void bye(void) {
    exit(0);
}

void qrx(void) {
    forthvm_push(getchar());
    if (top != 0)
        forthvm_push(TRUE);
}

void txsto(void) {
    putchar(top);
    forthvm_pop();
}

void next(void) {
    IP = data[IP >> 2];
    WP = UP + 4;
}

void dovar(void) {
    forthvm_push(WP);
}

void docon(void) {
    forthvm_push(data[WP >> 2]);
}

void dolit(void) {
    forthvm_push(data[IP >> 2]);
    IP += 4;
    next();
}

void dolist(void) {
    forthvm_pushR(IP);
    IP = WP;
    next();
}

void exitt(void) {
    IP = forthvm_popR();
    next();
}

void execu(void) {
    UP = top;
    WP = UP + 4;
    forthvm_pop();
}

void donext(void) {
    if (forthvm_popR()) {
        forthvm_pushR(forthvm_popR() - 1);
        IP = data[IP >> 2];
    } else {
        IP += 4;
        RP--;
    }
    next();
}

void qbran(void) {
    if (top == 0)
        IP = data[IP >> 2];
    else
        IP += 4;
    forthvm_pop();
    next();
}

void bran(void) {
    IP = data[IP >> 2];
    next();
}

void store(void) {
    data[top >> 2] = stack[SP--];
    forthvm_pop();
}

void at(void) {
    top = data[top >> 2];
}

void cstor(void) {
    cData[top] = stack[SP--];
    forthvm_pop();
}

void cat(void) {
    top = cData[top];
}

void rfrom(void) {
    forthvm_pushR(forthvm_popR());
}

void rat(void) {
    forthvm_pushR(forthvm_popR());
}

void tor(void) {
    forthvm_pushR(top);
    forthvm_pop();
}

void drop(void) {
    forthvm_pop();
}

void dup(void) {
    stack[++SP] = top;
}

void swap(void) {
    int32_t temp = top;
    top = stack[SP];
    stack[SP] = temp;
}

void over(void) {
    forthvm_push(stack[SP - 1]);
}

void zless(void) {
    top = (top < 0) ? TRUE : FALSE;
}

void andd(void) {
    top &= stack[SP--];
}

void orr(void) {
    top |= stack[SP--];
}

void xorr(void) {
    top ^= stack[SP--];
}

void uplus(void) {
    stack[SP] += top;
    top = (stack[SP] < top) ? TRUE : FALSE;
}

void nop(void) {
    next();
}

void qdup(void) {
    if (top != 0)
        stack[++SP] = top;
}

void rot(void) {
    int32_t temp = stack[SP - 1];
    stack[SP - 1] = stack[SP];
    stack[SP] = top;
    top = temp;
}

void ddrop(void) {
    forthvm_drop();
    forthvm_drop();
}

void ddup(void) {
    forthvm_over();
    forthvm_over();
}

void plus(void) {
    top += stack[SP--];
}

void inver(void) {
    top = ~top;
}

void negat(void) {
    top = -top;
}

void dnega(void) {
    forthvm_inver();
    forthvm_tor();
    forthvm_inver();
    forthvm_push(TRUE);
    forthvm_uplus();
    forthvm_rfrom();
    forthvm_plus();
}

void subb(void) {
    top = stack[SP--] - top;
}

void abss(void) {
    if (top < 0) {
        top = -top;
    }
}

void great(void) {
    top = (stack[SP--] > top) ? TRUE : FALSE;
}

void less(void) {
    top = (stack[SP--] < top) ? TRUE : FALSE;
}

void equal(void) {
    top = (stack[SP--] == top) ? TRUE : FALSE;
}

void uless(void) {
    top = ((uint32_t)stack[SP] < (uint32_t)top) ? TRUE : FALSE;
    SP--;
}

void ummod(void) {
    int64_t d = (uint32_t)top;
    int64_t m = (uint32_t)stack[SP];
    int64_t n = (uint32_t)stack[SP - 1];
    n += m << 32;
    forthvm_pop();
    top = (uint32_t)(n / d);
    stack[SP] = (uint32_t)(n % d);
}

void msmod(void) {
    int64_t d = top;
    int64_t m = stack[SP];
    int64_t n = stack[SP - 1];
    n += m << 32;
    forthvm_pop();
    top = (n / d);
    stack[SP] = (n % d);
}

void slmod(void) {
    if (top != 0) {
        WP = stack[SP] / top;
        stack[SP] %= top;
        top = WP;
    }
}

void mod(void) {
    if (top != 0)
        top = stack[SP--] % top;
    else {
        SP--;
        top = 0;
    }
}

void slash(void) {
    if (top != 0)
        top = stack[SP--] / top;
    else {
        SP--;
        top = 0;
    }
}

void umsta(void) {
    int64_t d = (uint32_t)top;
    int64_t m = (uint32_t)stack[SP];
    m *= d;
    top = (uint32_t)(m >> 32);
    stack[SP] = (uint32_t)m;
}

void star(void) {
    top *= stack[SP--];
}

void mstar(void) {
    int64_t d = top;
    int64_t m = stack[SP];
    m *= d;
    top = (m >> 32);
    stack[SP] = m;
}

void ssmod(void) {
    int64_t d = top;
    int64_t m = stack[SP];
    int64_t n = stack[SP - 1];
    n *= m;
    forthvm_pop();
    top = (n / d);
    stack[SP] = (n % d);
}

void stasl(void) {
    int64_t d = top;
    int64_t m = stack[SP];
    int64_t n = stack[SP - 1];
    n *= m;
    forthvm_pop();
    forthvm_pop();
    top = (n / d);
}

void pick(void) {
    top = stack[SP - top];
}

void pstor(void) {
    data[top >> 2] += stack[SP--];
    forthvm_pop();
}

void dstor(void) {
    data[(top >> 2) + 1] = stack[SP--];
    data[top >> 2] = stack[SP--];
    forthvm_pop();
}

void dat(void) {
    forthvm_push(data[top >> 2]);
    top = data[(top >> 2) + 1];
}

void count(void) {
    stack[++SP] = top + 1;
    top = cData[top];
}

void max(void) {
    if (top < stack[SP])
        forthvm_pop();
    else
        SP--;
}

void min(void) {
    if (top < stack[SP])
        SP--;
    else
        forthvm_pop();
}

// main
// TODO: Add functions for REPL and Interpreter here.

int main() {
    forthvm_init();
    forthvm_run();
    printf("\nTolerantForth v0.01\n");
    for (;;) {
        primitives[cData[UP++]]();
    }
}
