#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
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
int32_t data[8192];
unsigned char cData[32768];

// Helper functions for terminal I/O
int readNumber() {
    int number;
    scanf("%d", &number);
    return number;
}

void printNumber(int number) {
    printf("%d", number);
}

void printString(const char* str) {
    printf("%s", str);
}

void printChar(char c) {
    printf("%c", c);
}

// Core Forth words
void HEADER(int lex, const char seq[]) {
    IP = UP >> 2;
    int i;
    int len = lex & 31;
    data[IP++] = thread;
    UP = IP << 2;
    thread = UP;
    cData[UP++] = lex;
    for (i = 0; i < len; i++) {
        cData[UP++] = seq[i];
    }
    while (UP & 3) {
        cData[UP++] = 0;
    }
}

int CODE(int len, ...) {
    int addr = UP;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        cData[UP++] = va_arg(argList, int);
    }
    va_end(argList);
    return addr;
}

int COLON(int len, ...) {
    int addr = UP;
    IP = UP >> 2;
    data[IP++] = as_dolist; // dolist
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
    return addr;
}

int LABEL(int len, ...) {
    int addr = UP;
    IP = UP >> 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
    return addr;
}

void BEGIN(int len, ...) {
    IP = UP >> 2;
    pushR = IP;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void AGAIN(int len, ...) {
    IP = UP >> 2;
    data[IP++] = BRAN;
    data[IP++] = popR << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void UNTIL(int len, ...) {
    IP = UP >> 2;
    data[IP++] = QBRAN;
    data[IP++] = popR << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void WHILE(int len, ...) {
    IP = UP >> 2;
    int k;
    data[IP++] = QBRAN;
    data[IP++] = 0;
    k = popR;
    pushR = (IP - 1);
    pushR = k;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void REPEAT(int len, ...) {
    IP = UP >> 2;
    data[IP++] = BRAN;
    data[IP++] = popR << 2;
    data[popR] = IP << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void IF(int len, ...) {
    IP = UP >> 2;
    data[IP++] = QBRAN;
    pushR = IP;
    data[IP++] = 0;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void ELSE(int len, ...) {
    IP = UP >> 2;
    data[IP++] = BRAN;
    data[IP++] = 0;
    data[popR] = IP << 2;
    pushR = IP - 1;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void THEN(int len, ...) {
    IP = UP >> 2;
    data[popR] = IP << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void FOR(int len, ...) {
    IP = UP >> 2;
    data[IP++] = TOR;
    pushR = IP;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void NEXT(int len, ...) {
    IP = UP >> 2;
    data[IP++] = DONXT;
    data[IP++] = popR << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void AFT(int len, ...) {
    IP = UP >> 2;
    int k;
    data[IP++] = BRAN;
    data[IP++] = 0;
    k = popR;
    pushR = IP;
    pushR = IP - 1;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        data[IP++] = va_arg(argList, int);
    }
    UP = IP << 2;
    va_end(argList);
}

void DOTQ(const char seq[]) {
    IP = UP >> 2;
    int i;
    int len = strlen(seq);
    data[IP++] = DOTQP;
    UP = IP << 2;
    cData[UP++] = len;
    for (i = 0; i < len; i++) {
        cData[UP++] = seq[i];
    }
    while (UP & 3) {
        cData[UP++] = 0;
    }
}

void STRQ(const char seq[]) {
    IP = UP >> 2;
    int i;
    int len = strlen(seq);
    data[IP++] = STRQP;
    UP = IP << 2;
    cData[UP++] = len;
    for (i = 0; i < len; i++) {
        cData[UP++] = seq[i];
    }
    while (UP & 3) {
        cData[UP++] = 0;
    }
}

void ABORQ(const char seq[]) {
    IP = UP >> 2;
    int i;
    int len = strlen(seq);
    data[IP++] = ABORQP;
    UP = IP << 2;
    cData[UP++] = len;
    for (i = 0; i < len; i++) {
        cData[UP++] = seq[i];
    }
    while (UP & 3) {
        cData[UP++] = 0;
    }
}

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
    forthvm_define_word("=", equals);
    forthvm_define_word("<>", notequ);
    forthvm_define_word(">", greater);
    forthvm_define_word("<", less);
    forthvm_define_word("+", plus);
    forthvm_define_word("-", minus);
    forthvm_define_word("*", multiply);
    forthvm_define_word("/", divide);
    forthvm_define_word("MOD", mod);
    forthvm_define_word("AND", andd);
    forthvm_define_word("OR", orr);
    forthvm_define_word("XOR", xorr);
    forthvm_define_word("NOT", nott);
    forthvm_define_word("NEGATE", negat);
    forthvm_define_word("U<", uless);
    forthvm_define_word("UM+", umplus);
    forthvm_define_word("CELLS", cells);
    forthvm_define_word("ALLOT", allot);
    forthvm_define_word("HERE", here);
    forthvm_define_word("COMPILE,", comma);
    forthvm_define_word("CREATE", create);
    forthvm_define_word("CHARS", chars);
    forthvm_define_word("COUNT", count);
    forthvm_define_word("CMOVE", cmove);
    forthvm_define_word("MOVE", move);
    forthvm_define_word("FILL", fill);
}

void forthvm_run(const char* input) {
    // Reset stack and memory
    top = 0;
    SP = -1;
    IP = 0;
    WP = 0;

    // Tokenize the input
    char* token = strtok(input, " ");
    while (token != NULL) {
        // Parse the token
        int32_t value;
        if (sscanf(token, "%d", &value) == 1) {
            // Push number onto the stack
            forthvm_push(value);
        } else {
            // Lookup the word in the dictionary
            forthvm_word* word = forthvm_find_word(token);
            if (word != NULL && word->func != NULL) {
                // Execute the word's function
                word->func();
            } else {
                // Word not found in the dictionary
                printf("Unknown word: %s\n", token);
            }
        }

        // Move to the next token
        token = strtok(NULL, " ");
    }
}
