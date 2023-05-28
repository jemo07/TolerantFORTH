#include <stdio.h>
#include <stdlib.h>
#include "forthvm.h"

#define FALSE 0
#define TRUE -1
#define LOGICAL ? TRUE : FALSE
#define LOWER(x, y) ((uint32_t)(x) < (uint32_t)(y))
#define pop top = stack[SP--]
#define push stack[++SP] = top; top =
#define popR rack[RP--]
#define pushR rack[++RP]

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

// Function implementations
void bye(void) { exit(0); }
void qrx(void) { push getchar(); if (top != 0) push TRUE; }
void txsto(void) { putchar(top); pop; }
void next(void) { UP = data[IP >> 2]; WP = UP + 4; IP += 4; }
void dovar(void) { push WP; }
void docon(void) { push data[WP >> 2]; }
void dolit(void) { push data[IP >> 2]; IP += 4; next(); }
void dolist(void) { rack[++RP] = IP; IP = WP; next(); }
void exitt(void) { IP = rack[RP--]; next(); }
void execu(void) { UP = top; WP = UP + 4; pop; }
void donext(void) { if (rack[RP]) { rack[RP] -= 1; IP = data[IP >> 2]; } else { IP += 4; RP--; } next(); }
void qbran(void) { if (top == 0) IP = data[IP >> 2]; else IP += 4; pop; next(); }
void bran(void) { IP = data[IP >> 2]; next(); }
void store(void) { data[top >> 2] = stack[SP--]; pop; }
void at(void) { top = data[top >> 2]; }
void cstor(void) { cData[top] = stack[SP--]; pop; }
void cat(void) { top = cData[top]; }
void rfrom(void) { pushR; }
void rat(void) { pushR; }
void tor(void) { pushR; pop; }
void drop(void) { pop; }
void dup(void) { stack[++SP] = top; }
void swap(void) { WP = top; top = stack[SP]; stack[SP] = WP; }
void over(void) { push stack[SP - 1]; }
void zless(void) { top = (top < 0) LOGICAL; }
void andd(void) { top &= stack[SP--]; }
void orr(void) { top |= stack[SP--]; }
void xorr(void) { top ^= stack[SP--]; }
void uplus(void) { stack[SP] += top; top = LOWER(stack[SP], top); }
void nop(void) { next(); }
void qdup(void) { if (top) stack[++SP] = top; }
void rot(void) { WP = stack[SP - 1]; stack[SP - 1] = stack[SP]; stack[SP] = top; top = WP; }
void ddrop(void) { drop(); drop(); }
void ddup(void) { over(); over(); }
void plus(void) { top += stack[SP--]; }
void inver(void) { top = -top - 1; }
void negat(void) { top = 0 - top; }
void dnega(void) { inver(); tor(); inver(); push 1; uplus(); rfrom(); plus(); }
void subb(void) { top = stack[SP--] - top; }
void abss(void) { if (top < 0) { top = -top; } }
void great(void) { top = (stack[SP--] > top) LOGICAL; }
void less(void) { top = (stack[SP--] < top) LOGICAL; }
void equal(void) { top = (stack[SP--] == top) LOGICAL; }
void uless(void) { top = LOWER(stack[SP], top) LOGICAL; SP--; }
void ummod(void) { int64_t d = (uint32_t)top; int64_t m = (uint32_t)stack[SP]; int64_t n = (uint32_t)stack[SP - 1]; n += m << 32; pop; top = (uint32_t)(n / d); stack[SP] = (uint32_t)(n % d); }
void msmod(void) { int64_t d = top; int64_t m = stack[SP]; int64_t n = stack[SP - 1]; n += m << 32; pop; top = (n / d); stack[SP] = (n % d); }
void slmod(void) { if (top != 0) { WP = stack[SP] / top; stack[SP] %= top; top = WP; } }
void mod(void) { if (top != 0) top = stack[SP--] % top; else top = stack[SP--]; }
void slash(void) { if (top != 0) top = stack[SP--] / top; else { SP--; top = 0; } }
void umsta(void) { int64_t d = (uint32_t)top; int64_t m = (uint32_t)stack[SP]; m *= d; top = (uint32_t)(m >> 32); stack[SP] = (uint32_t)m; }
void star(void) { top *= stack[SP--]; }
void mstar(void) { int64_t d = top; int64_t m = stack[SP]; m *= d; top = (m >> 32); stack[SP] = m; }
void ssmod(void) { int64_t d = top; int64_t m = stack[SP]; int64_t n = stack[SP - 1]; n *= m; pop; top = (n / d); stack[SP] = (n % d); }
void stasl(void) { int64_t d = top; int64_t m = stack[SP]; int64_t n = stack[SP - 1]; n *= m; pop; pop; top = (n / d); }
void pick(void) { top = stack[SP - top]; }
void pstor(void) { data[top >> 2] += stack[SP--], pop; }
void dstor(void) { data[(top >> 2) + 1] = stack[SP--]; data[top >> 2] = stack[SP--]; pop; }
void dat(void) { push data[top >> 2]; top = data[(top >> 2) + 1]; }
void count(void) { stack[++SP] = top + 1; top = cData[top]; }
void max(void) { if (top < stack[SP]) pop; else SP--; }
void min(void) { if (top < stack[SP]) SP--; else pop; }

// main
// TODO: Add fuctions for REPL and Intepreter here. 

int main() {
    printf("\nTolorantForth v0.01 \n");
    for (;;) {
        primitives[cData[UP++]]();
    }
}
