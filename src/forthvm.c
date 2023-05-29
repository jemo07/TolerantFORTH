#include "forthvm.h"
#include "dictionary.h"

// Stack
static int32_t stack[MAX_STACK_SIZE];
static int8_t S = -1;

// Return stack
static int32_t rack[MAX_STACK_SIZE];
static int8_t R = -1;

// Data memory
static int32_t data[MAX_DICTIONARY_SIZE];

// Code execution
static int32_t IP;
static int32_t WP;
static int32_t P;

// Primitive function implementations
void bye() {
    exit(0);
}

void qrx() {
    forthvm_push(getchar());
    if (forthvm_pop() != 0) {
        forthvm_push(1);
    }
}

void txsto() {
    putchar((char)forthvm_pop());
}

void next() {
    P = data[IP >> 2];
    WP = P + 4;
    IP += 4;
}

void dovar() {
    forthvm_push(WP);
}

void docon() {
    forthvm_push(data[WP >> 2]);
}

void dolit() {
    forthvm_push(data[IP >> 2]);
    IP += 4;
    next();
}

void dolist() {
    rack[++R] = IP;
    IP = WP;
    next();
}

void exitt() {
    IP = rack[R--];
    next();
}

void execu() {
    P = forthvm_pop();
    WP = P + 4;
}

void donext() {
    if (rack[R]) {
        rack[R] -= 1;
        IP = data[IP >> 2];
    } else {
        IP += 4;
        R--;
    }
    next();
}

void qbran() {
    if (forthvm_pop() == 0) {
        IP = data[IP >> 2];
    } else {
        IP += 4;
    }
    next();
}

void bran() {
    IP = data[IP >> 2];
    next();
}

void store() {
    data[forthvm_pop() >> 2] = stack[S--];
}

void at() {
    forthvm_push(data[forthvm_pop() >> 2]);
}

void cstor() {
    data[forthvm_pop()] = (char)stack[S--];
}

void cat() {
    forthvm_push((int32_t)data[forthvm_pop()]);
}

void rfrom() {
    forthvm_push(rack[R--]);
}

void rat() {
    forthvm_push(rack[R]);
}

void tor() {
    rack[++R] = forthvm_pop();
}

void drop() {
    forthvm_pop();
}

void dup() {
    stack[++S] = forthvm_pop();
    stack[S] = stack[S];
}

void swap() {
    WP = forthvm_pop();
    int32_t temp = stack[S];
    stack[S] = WP;
    stack[S] = temp;
}

void over() {
    forthvm_push(stack[S - 1]);
}

void zless() {
    forthvm_push(forthvm_pop() < 0 ? 1 : 0);
}

void andd() {
    stack[S] &= forthvm_pop();
}

void orr() {
    stack[S] |= forthvm_pop();
}

void xorr() {
    stack[S] ^= forthvm_pop();
}

void uplus() {
    stack[S] += forthvm_pop();
    stack[S] = stack[S] < forthvm_pop() ? stack[S] : forthvm_pop();
}

void nop() {
    next();
}

void qdup() {
    if (forthvm_pop() != 0) {
        stack[++S] = stack[S];
    }
}

void rot() {
    WP = stack[S - 1];
    stack[S - 1] = stack[S];
    stack[S] = forthvm_pop();
    stack[S] = WP;
}

void ddrop() {
    drop();
    drop();
}

void ddup() {
    over();
    over();
}

void plus() {
    stack[S] += forthvm_pop();
}

void inver() {
    stack[S] = ~stack[S];
}

void negat() {
    stack[S] = -stack[S];
}

void dnega() {
    inver();
    tor();
    inver();
    forthvm_push(1);
    uplus();
    rfrom();
    plus();
}

void subb() {
    stack[S] = stack[S] - forthvm_pop();
}

void abss() {
    if (stack[S] < 0) {
        stack[S] = -stack[S];
    }
}

void great() {
    stack[S] = (stack[S - 1] > stack[S]) ? 1 : 0;
    S--;
}

void less() {
    stack[S] = (stack[S - 1] < stack[S]) ? 1 : 0;
    S--;
}

void equal() {
    stack[S] = (stack[S - 1] == stack[S]) ? 1 : 0;
    S--;
}

void uless() {
    stack[S] = (stack[S - 1] < stack[S]) ? 1 : 0;
    S--;
}

void ummod() {
    int64_t d = (int64_t)((uint32_t)forthvm_pop());
    int64_t m = (int64_t)((uint32_t)stack[S]);
    int64_t n = (int64_t)((uint32_t)stack[S - 1]);
    n += m << 32;
    S -= 2;
    forthvm_push((int32_t)(n / d));
    forthvm_push((int32_t)(n % d));
}

void msmod() {
    int64_t d = (int64_t)((int32_t)forthvm_pop());
    int64_t m = (int64_t)((int32_t)stack[S]);
    int64_t n = (int64_t)((int32_t)stack[S - 1]);
    n += m << 32;
    S -= 2;
    forthvm_push((int32_t)(n / d));
    forthvm_push((int32_t)(n % d));
}

void slmod() {
    if (forthvm_pop() != 0) {
        WP = stack[S] / forthvm_pop();
        stack[S] %= forthvm_pop();
        stack[S] = WP;
    }
}

void mod() {
    stack[S] = (stack[S] != 0) ? stack[S - 1] % stack[S] : stack[S - 1];
    S--;
}

void slash() {
    stack[S] = (stack[S] != 0) ? stack[S - 1] / stack[S] : (stack[S - 1], 0);
    S--;
}

void umsta() {
    int64_t d = (uint64_t)forthvm_pop();
    int64_t m = (uint64_t)stack[S];
    m *= d;
    forthvm_push((int32_t)(m >> 32));
    stack[S] = (int32_t)m;
}

void star() {
    stack[S] *= forthvm_pop();
}

void mstar() {
    int64_t d = (int64_t)forthvm_pop();
    int64_t m = (int64_t)stack[S];
    m *= d;
    forthvm_push((int32_t)(m >> 32));
    stack[S] = (int32_t)m;
}

void ssmod() {
    int64_t d = (int64_t)forthvm_pop();
    int64_t m = (int64_t)stack[S];
    int64_t n = (int64_t)stack[S - 1];
    n *= m;
    S -= 2;
    forthvm_push((int32_t)(n / d));
    stack[S] = (int32_t)(n % d);
}

void stasl() {
    int64_t d = (int64_t)forthvm_pop();
    int64_t m = (int64_t)stack[S];
    int64_t n = (int64_t)stack[S - 1];
    n *= m;
    S -= 2;
    forthvm_push((int32_t)(n / d));
}

void pick() {
    stack[S] = stack[S - forthvm_pop()];
}

void pstor() {
    data[forthvm_pop() >> 2] += stack[S];
    S--;
}

void dstor() {
    data[forthvm_pop() >> 2] = stack[S];
    data[(forthvm_pop() >> 2) + 1] = stack[S - 1];
    S -= 2;
}

void dat() {
    WP = forthvm_pop() >> 2;
    forthvm_push(data[WP + 1]);
    forthvm_push(data[WP]);
}

void count() {
    stack[++S] = forthvm_pop() + 1;
    forthvm_push((int32_t)cData[forthvm_pop()]);
}

void max() {
    if (forthvm_pop() < stack[S]) {
        S--;
    } else {
        forthvm_pop();
    }
}

void min() {
    if (forthvm_pop() < stack[S]) {
        forthvm_pop();
    } else {
        S--;
    }
}

// Array of primitive functions
void (*primitives[64])(void) = {
    /* case 0 */ nop,
    /* case 1 */ bye,
    /* case 2 */ qrx,
    /* case 3 */ txsto,
    /* case 4 */ docon,
    /* case 5 */ dolit,
    /* case 6 */ dolist,
    /* case 7 */ exitt,
    /* case 8 */ execu,
    /* case 9 */ donext,
    /* case 10 */ qbran,
    /* case 11 */ bran,
    /* case 12 */ store,
    /* case 13 */ at,
    /* case 14 */ cstor,
    /* case 15 */ cat,
    /* case 16 */ nop, // rpat
    /* case 17 */ nop, // rpsto
    /* case 18 */ rfrom,
    /* case 19 */ rat,
    /* case 20 */ tor,
    /* case 21 */ nop, // spat
    /* case 22 */ nop, // spsto
    /* case 23 */ drop,
    /* case 24 */ dup,
    /* case 25 */ swap,
    /* case 26 */ over,
    /* case 27 */ zless,
    /* case 28 */ andd,
    /* case 29 */ orr,
    /* case 30 */ xorr,
    /* case 31 */ uplus,
    /* case 32 */ next,
    /* case 33 */ qdup,
    /* case 34 */ rot,
    /* case 35 */ ddrop,
    /* case 36 */ ddup,
    /* case 37 */ plus,
    /* case 38 */ inver,
    /* case 39 */ negat,
    /* case 40 */ dnega,
    /* case 41 */ subb,
    /* case 42 */ abss,
    /* case 43 */ equal,
    /* case 44 */ uless,
    /* case 45 */ less,
    /* case 46 */ ummod,
    /* case 47 */ msmod,
    /* case 48 */ slmod,
    /* case 49 */ mod,
    /* case 50 */ slash,
    /* case 51 */ umsta,
    /* case 52 */ star,
    /* case 53 */ mstar,
    /* case 54 */ ssmod,
    /* case 55 */ stasl,
    /* case 56 */ pick,
    /* case 57 */ pstor,
    /* case 58 */ dstor,
    /* case 59 */ dat,
    /* case 60 */ count,
    /* case 61 */ dovar,
    /* case 62 */ max,
    /* case 63 */ min,
};

// Initialize the Forth VM
void forthvm_init() {
    dictionary_init();

    // Define Forth words and add them to the dictionary
    dictionary_define_word("+", &add);
    dictionary_define_word("-", &subtract);
    dictionary_define_word(".", &print_top);
    dictionary_define_word("BYE", &bye);
    dictionary_define_word("?RX", &qrx);
    dictionary_define_word("TX!", &txsto);
    dictionary_define_word("NEXT", &next);
    dictionary_define_word("DOVAR", &dovar);
    dictionary_define_word("DOCON", &docon);
    dictionary_define_word("DOLIT", &dolit);
    dictionary_define_word("DOLIST", &dolist);
    dictionary_define_word("EXIT", &exitt);
    dictionary_define_word("EXECUTE", &execu);
    dictionary_define_word("DOSTR", &donext);
    dictionary_define_word("?BRANCH", &qbran);
    dictionary_define_word("BRANCH", &bran);
    dictionary_define_word("!", &store);
    dictionary_define_word("@", &at);
    dictionary_define_word("C!", &cstor);
    dictionary_define_word("C@", &cat);
    dictionary_define_word("R>", &rfrom);
    dictionary_define_word("R@", &rat);
    dictionary_define_word(">R", &tor);
    dictionary_define_word("DROP", &drop);
    dictionary_define_word("DUP", &dup);
    dictionary_define_word("SWAP", &swap);
    dictionary_define_word("OVER", &over);
    dictionary_define_word("0<", &zless);
    dictionary_define_word("=", &equals);
    dictionary_define_word("<>", &notequ);
    dictionary_define_word(">", &greater);
    dictionary_define_word("<", &less);
    dictionary_define_word("+", &plus);
    dictionary_define_word("-", &minus);
    dictionary_define_word("*", &multiply);
    dictionary_define_word("/", &divide);
    dictionary_define_word("MOD", &mod);
    dictionary_define_word("AND", &andd);
    dictionary_define_word("OR", &orr);
    dictionary_define_word("XOR", &xorr);
    dictionary_define_word("NOT", &nott);
    dictionary_define_word("NEGATE", &negat);
    dictionary_define_word("U<", &uless);
    dictionary_define_word("UM+", &umplus);
    dictionary_define_word("CELLS", &cells);
    dictionary_define_word("ALLOT", &allot);
    dictionary_define_word("HERE", &here);
    dictionary_define_word("COMPILE,", &comma);
    dictionary_define_word("CREATE", &create);
    dictionary_define_word("CHARS", &chars);
    dictionary_define_word("COUNT", &count);
    dictionary_define_word("CMOVE", &cmove);
    dictionary_define_word("MOVE", &move);
    dictionary_define_word("FILL", &fill);
    dictionary_define_word("DUP2", &dup2);
    dictionary_define_word("SWAP2", &swap2);
    dictionary_define_word("DROP2", &drop2);
    dictionary_define_word("OVER2", &over2);
    dictionary_define_word("PICK2", &pick2);
    dictionary_define_word("ROT2", &rot2);
    dictionary_define_word("MINUS", &minus);
    dictionary_define_word("NEGATE", &negate);
    dictionary_define_word("MAX2", &max2);
    dictionary_define_word("MIN2", &min2);
}

// Execute a Forth word
void forthvm_execute_word(const char* word) {
    dictionary_execute_word(word);
}

// Push a value onto the stack
void forthvm_push(int32_t value) {
    stack[++S] = value;
}

// Pop a value from the stack
int32_t forthvm_pop() {
    return stack[S--];
}
