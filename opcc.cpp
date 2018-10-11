
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <string>
#include <vector>
#include <map>

enum tokentype_t {
    TOK_NONE=0,                 // 0
    TOK_UINT,
    TOK_INT,
    TOK_FLOAT,
    TOK_STRING,
    TOK_ERROR,                  // 5
    TOK_MINUS,
    TOK_SEMICOLON,
    TOK_PREFIX,
    TOK_OPCODE,
    TOK_UNKNOWN,                // 10
    TOK_OPEN_PARENS,
    TOK_CLOSE_PARENS,
    TOK_SILENT,
    TOK_EXCEPTION,
    TOK_UD,                     // 15
    TOK_CODE,
    TOK_DEST,
    TOK_PARAM,
    TOK_MRM,
    TOK_MOD,                    // 20
    TOK_REG,
    TOK_RM,
    TOK_COMMA,
    TOK_AL,
    TOK_AH,                     // 25
    TOK_AX,
    TOK_EAX,
    TOK_AV,
    TOK_BL,
    TOK_BH,                     // 30
    TOK_BX,
    TOK_EBX,
    TOK_BV,
    TOK_CL,
    TOK_CH,                     // 35
    TOK_CX,
    TOK_ECX,
    TOK_CV,
    TOK_DL,
    TOK_DH,                     // 40
    TOK_DX,
    TOK_EDX,
    TOK_DV,
    TOK_SI,
    TOK_ESI,                    // 45
    TOK_SIV,
    TOK_DI,
    TOK_EDI,
    TOK_DIV,
    TOK_BP,                     // 50
    TOK_EBP,
    TOK_BPV,
    TOK_SP,
    TOK_ESP,
    TOK_SPV,                    // 55
    TOK_IP,
    TOK_EIP,
    TOK_IPV,
    TOK_FLAGS,
    TOK_EFLAGS,                 // 60
    TOK_FLAGSV,
    TOK_CS,
    TOK_DS,
    TOK_ES,
    TOK_FS,                     // 65
    TOK_GS,
    TOK_SS,
    TOK_A,
    TOK_B,
    TOK_C,                      // 70
    TOK_D,
    TOK_E,
    TOK_F,
    TOK_G,
    TOK_H,                      // 75
    TOK_I,
    TOK_J,
    TOK_K,
    TOK_L,
    TOK_M,                      // 80
    TOK_N,
    TOK_O,
    TOK_P,
    TOK_Q,
    TOK_R,                      // 85
    TOK_S,
    TOK_T,
    TOK_U,
    TOK_V,
    TOK_W,                      // 90
    TOK_X,
    TOK_Y,
    TOK_Z,
    TOK_ALL,
    TOK_EQUAL,                  // 95
    TOK_DW,
    TOK_TW,
    TOK_QW,
    TOK_HV,
    TOK_SB,                     // 100
    TOK_SW,
    TOK_SDW,
    TOK_SV,
    TOK_SHV,
    TOK_SQW,                    // 105
    TOK_FPW,
    TOK_FPDW,
    TOK_FPV,
    TOK_GENERAL,
    TOK_SEG,                    // 110
    TOK_F32,
    TOK_F64,
    TOK_F80,
    TOK_F87ENV,
    TOK_F87STATE,               // 115
    TOK_IMMEDIATE,
    TOK_DESC,
    TOK_COMMENT,
    TOK_LIMIT,
    TOK_WORD_NONE,              // 120 "none"
    TOK_IF,
    TOK_LESSTHAN,
    TOK_GREATERTHAN,
    TOK_VALUE,
    TOK_LOG,                    // 125
    TOK_PLUS,
    TOK_FORMAT,
    TOK_SET,
    TOK_UNSET,
    TOK_ISSET,                  // 130
    TOK_WORD_ERROR,
    TOK_BOOLEAN,
    TOK_WARNING,
    TOK_NOT,
    TOK_NEGATE,                 // 135
    TOK_HEXSTRING,
    TOK_UNSIGNED,
    TOK_SIGNED,
    TOK_OCTSTRING,
    TOK_BINSTRING,              // 140
    TOK_WORD_STRING,
    TOK_VALUETYPE,
    TOK_DOUBLEEQUALS,
    TOK_LESSTHANOREQUALS,
    TOK_GREATERTHANOREQUALS,    // 145
    TOK_AND,
    TOK_OR,
    TOK_XOR,
    TOK_AMPERSAND,
    TOK_PIPE,                   // 150
    TOK_CARET,

    TOK_MAX
};

const char *tokentype_str[TOK_MAX] = {
    "none",                     // 0
    "uint",
    "int",
    "float",
    "string",
    "error",                    // 5
    "minus",
    "semicolon",
    "prefix",
    "opcode",
    "unknown",                  // 10
    "open_parens",
    "close_parens",
    "silent",
    "exception",
    "ud",                       // 15
    "code",
    "dest",
    "param",
    "mrm",
    "mod",                      // 20
    "reg",
    "rm",
    "comma",
    "AL",
    "AH",                       // 25
    "AX",
    "EAX",
    "AV",
    "BL",
    "BH",                       // 30
    "BX",
    "EBX",
    "BV",
    "CL",
    "CH",                       // 35
    "CX",
    "ECX",
    "CV",
    "DL",
    "DH",                       // 40
    "DX",
    "EDX",
    "DV",
    "SI",
    "ESI",                      // 45
    "SIV",
    "DI",
    "EDI",
    "DIV",
    "BP",                       // 50
    "EBP",
    "BPV",
    "SP",
    "ESP",
    "SPV",                      // 55
    "IP",
    "EIP",
    "IPV",
    "FLAGS",
    "EFLAGS",                   // 60
    "FLAGSV",
    "CS",
    "DS",
    "ES",
    "FS",                       // 65
    "GS",
    "SS",
    "A",
    "B",
    "C",                        // 70
    "D",
    "E",
    "F",
    "G",
    "H",                        // 75
    "I",
    "J",
    "K",
    "L",
    "M",                        // 80
    "N",
    "O",
    "P",
    "Q",
    "R",                        // 85
    "S",
    "T",
    "U",
    "V",
    "W",                        // 90
    "X",
    "Y",
    "Z",
    "ALL",
    "EQUAL",                    // 95
    "DW",
    "TW",
    "QW",
    "HV",
    "SB",                       // 100
    "SW",
    "SDW",
    "SV",
    "SHV",
    "SQW",                      // 105
    "FPW",
    "FPDW",
    "FPV",
    "GENERAL",
    "SEG",                      // 110
    "F32",
    "F64",
    "F80",
    "F87ENV",
    "F87STATE",                 // 115
    "IMMEDIATE",
    "DESC",
    "COMMENT",
    "LIMIT",
    "NONE",                     // 120
    "IF",
    "LESSTHAN",
    "GREATERTHAN",
    "VALUE",
    "LOG",                      // 125
    "PLUS",
    "FORMAT",
    "SET",
    "UNSET",
    "ISSET",                    // 130
    "ERROR",
    "BOOLEAN",
    "WARNING",
    "NOT",
    "NEGATE",                   // 135
    "HEXSTRING",
    "UNSIGNED",
    "SIGNED",
    "OCTSTRING",
    "BINSTRING",                // 140
    "STRING",
    "VALUETYPE",
    "DOUBLEEQUALS",
    "LESSTHANOREQUALS",
    "GREATERTHANOREQUALS",      // 145
    "AND",
    "OR",
    "XOR",
    "AMPERSAND",
    "PIPE",                     // 150
    "CARET"
};

struct tokenstate_t {
    enum tokentype_t    type = TOK_NONE;
    std::string         string;
    union {
        uint64_t        u = 0;
        int64_t         i;
    } intval;
    long double         floatval = 0;

    static void promote_for_comparison(tokenstate_t &p1,tokenstate_t &p2) {
        if (p1.type == TOK_STRING || p2.type == TOK_STRING) {
            p1.string = p1.to_string();
            p2.string = p2.to_string();
            p1.type = p2.type = TOK_STRING;
        }
        else if (p1.type == TOK_FLOAT || p2.type == TOK_FLOAT) {
            p1.floatval = p1.to_float();
            p2.floatval = p2.to_float();
            p1.type = p2.type = TOK_FLOAT;
        }
         else if (p1.is_number() || p2.is_number()) { // but not float
            p1.intval.u = p1.to_intval_u();
            p2.intval.u = p2.to_intval_u();
            p1.type = p2.type = p2.is_number() ? p2.type : p1.type;
        }
        else {
            p1.type = p2.type = TOK_NONE;
        }
    }

    bool operator==(const tokenstate_t &ext) const {
        if (type == ext.type) {
            if (type == TOK_UINT || type == TOK_INT)
                return intval.u == ext.intval.u;
            else if (type == TOK_BOOLEAN)
                return to_bool() == ext.to_bool();
            else if (type == TOK_FLOAT)
                return floatval == ext.floatval;
            else if (type == TOK_STRING)
                return string == ext.string;

            return true;
        }

        tokenstate_t in_copy = *this,ext_copy = ext;

        promote_for_comparison(in_copy,ext_copy);
        assert(in_copy.type == ext_copy.type);
        return in_copy == ext_copy;
    }

    bool operator>(const tokenstate_t &ext) const {
        if (type == ext.type) {
            if (type == TOK_UINT)
                return intval.u > ext.intval.u;
            else if (type == TOK_INT)
                return intval.i > ext.intval.i;
            else if (type == TOK_BOOLEAN)
                return to_bool() > ext.to_bool();
            else if (type == TOK_FLOAT)
                return floatval > ext.floatval;

            return true;
        }

        tokenstate_t in_copy = *this,ext_copy = ext;

        promote_for_comparison(in_copy,ext_copy);
        assert(in_copy.type == ext_copy.type);
        return in_copy > ext_copy;
    }

    bool operator<(const tokenstate_t &ext) const {
        if (type == ext.type) {
            if (type == TOK_UINT)
                return intval.u < ext.intval.u;
            else if (type == TOK_INT)
                return intval.i < ext.intval.i;
            else if (type == TOK_BOOLEAN)
                return to_bool() < ext.to_bool();
            else if (type == TOK_FLOAT)
                return floatval < ext.floatval;

            return true;
        }

        tokenstate_t in_copy = *this,ext_copy = ext;

        promote_for_comparison(in_copy,ext_copy);
        assert(in_copy.type == ext_copy.type);
        return in_copy < ext_copy;
    }

    bool operator>=(const tokenstate_t &ext) const {
        if (type == ext.type) {
            if (type == TOK_UINT)
                return intval.u >= ext.intval.u;
            else if (type == TOK_INT)
                return intval.i >= ext.intval.i;
            else if (type == TOK_BOOLEAN)
                return to_bool() >= ext.to_bool();
            else if (type == TOK_FLOAT)
                return floatval >= ext.floatval;

            return true;
        }

        tokenstate_t in_copy = *this,ext_copy = ext;

        promote_for_comparison(in_copy,ext_copy);
        assert(in_copy.type == ext_copy.type);
        return in_copy >= ext_copy;
    }

    bool operator<=(const tokenstate_t &ext) const {
        if (type == ext.type) {
            if (type == TOK_UINT)
                return intval.u <= ext.intval.u;
            else if (type == TOK_INT)
                return intval.i <= ext.intval.i;
            else if (type == TOK_BOOLEAN)
                return to_bool() <= ext.to_bool();
            else if (type == TOK_FLOAT)
                return floatval <= ext.floatval;

            return true;
        }

        tokenstate_t in_copy = *this,ext_copy = ext;

        promote_for_comparison(in_copy,ext_copy);
        assert(in_copy.type == ext_copy.type);
        return in_copy <= ext_copy;
    }

    inline const char *type_str(void) const {
        return tokentype_str[type];
    }

    bool is_number(void) const {
        return  (type == TOK_UINT) ||
                (type == TOK_INT) ||
                (type == TOK_FLOAT) ||
                (type == TOK_BOOLEAN);
    }

    unsigned long long to_intval_u(void) const {
        if (type == TOK_UINT)
            return intval.u;
        else if (type == TOK_INT)
            return (unsigned long long)intval.i;
        else if (type == TOK_FLOAT)
            return (unsigned long long)floatval;
        else if (type == TOK_STRING)
            return strtoull(string.c_str(),NULL,0);
        else if (type == TOK_BOOLEAN)
            return intval.u != 0ull ? 1ull : 0ull;

        return false;
    }

    long double to_float(void) const {
        if (type == TOK_UINT)
            return intval.u;
        else if (type == TOK_INT)
            return intval.i;
        else if (type == TOK_FLOAT)
            return floatval;
        else if (type == TOK_STRING)
            return atof(string.c_str());
        else if (type == TOK_BOOLEAN)
            return intval.u != 0ull ? 1 : 0;

        return false;
    }

    bool to_bool(void) const {
        if (type == TOK_UINT)
            return intval.u != 0ull;
        else if (type == TOK_INT)
            return intval.i > 0ll;
        else if (type == TOK_FLOAT)
            return floatval > 0;
        else if (type == TOK_STRING)
            return !string.empty();
        else if (type == TOK_BOOLEAN)
            return intval.u != 0ull;

        return false;
    }

    std::string int_to_hex_string(void) const {
        if (type == TOK_UINT || type == TOK_INT || type == TOK_BOOLEAN) {
            char tmp[64];
            sprintf(tmp,"0x%llx",(unsigned long long)intval.u);
            return std::string(tmp);
        }

        fprintf(stderr,"WARNING: hex() used to convert type '%s', ignoring\n",type_str());
        return to_string();
    }

    std::string int_to_oct_string(void) const {
        if (type == TOK_UINT || type == TOK_INT || type == TOK_BOOLEAN) {
            char tmp[64];
            sprintf(tmp,"0%llo",(unsigned long long)intval.u);
            return std::string(tmp);
        }

        fprintf(stderr,"WARNING: oct() used to convert type '%s', ignoring\n",type_str());
        return to_string();
    }

    std::string int_to_bin_string(void) const {
        if (type == TOK_UINT || type == TOK_INT || type == TOK_BOOLEAN) {
            char tmp[sizeof(unsigned long long)*8 + 8];
            int i=sizeof(tmp);

            unsigned long long tmpv = intval.u;

            tmp[--i] = 0;
            tmp[--i] = 'b';
            do {
                tmp[--i] = (char)((tmpv & 1ull) + '0');
                tmpv >>= 1ull;
            } while (tmpv != 0ull);
            assert(i >= 0);

            return std::string(tmp+i);
        }

        fprintf(stderr,"WARNING: bin() used to convert type '%s', ignoring\n",type_str());
        return to_string();
    }

    std::string to_string(void) const {
        if (type == TOK_UINT) {
            char tmp[64];
            sprintf(tmp,"%llu",(unsigned long long)intval.u);
            return std::string(tmp);
        }
        else if (type == TOK_INT) {
            char tmp[64];
            sprintf(tmp,"%lld",(signed long long)intval.i);
            return std::string(tmp);
        }
        else if (type == TOK_FLOAT) {
            char tmp[64];
            sprintf(tmp,"%Lf",floatval);
            return std::string(tmp);
        }
        else if (type == TOK_STRING) {
            return string;
        }
        else if (type == TOK_BOOLEAN) {
            return intval.u != 0ull ? "true" : "false";
        }

        return std::string();
    }
} tokenstate;

FILE*           srcfp = NULL;
std::string     srcfile;
int             untoke = -1;

std::map<std::string,tokenstate_t>  defines;

void untokechar(int c) {
    assert(untoke < 0);
    untoke = c;
}

int tokechar(void) {
    unsigned char c;

    if (untoke >= 0) {
        c = (unsigned char)untoke;
        untoke = -1;
        return c;
    }

    if (srcfp == NULL) return -1;
    if (feof(srcfp) || ferror(srcfp)) return -1;
    if (fread(&c,1,1,srcfp) != 1) return -1;
    return c;
}

bool toke(tokenstate_t &tok) {
    int chr;

    chr = tokechar();
    while (chr == '\t' || chr == '\n' || chr == '\r' || chr == ' ') chr = tokechar();
    if (chr < 0) {
        tok.type = TOK_NONE;
        return false;
    }

    switch ((unsigned char)chr) {
        case '+': tok.type = TOK_PLUS;          return true;
        case ';': tok.type = TOK_SEMICOLON;     return true;
        case '(': tok.type = TOK_OPEN_PARENS;   return true;
        case ')': tok.type = TOK_CLOSE_PARENS;  return true;
        case ',': tok.type = TOK_COMMA;         return true;
        case '=':
            tok.type = TOK_EQUAL;

            chr = tokechar();
            if (chr == '=') {
                tok.type = TOK_DOUBLEEQUALS;
            }
            else {
                untokechar(chr);
            }

            return true;
        case '<':
            tok.type = TOK_LESSTHAN;

            chr = tokechar();
            if (chr == '=') {
                tok.type = TOK_LESSTHANOREQUALS;
            }
            else {
                untokechar(chr);
            }

            return true;
        case '>':
            tok.type = TOK_GREATERTHAN;

            chr = tokechar();
            if (chr == '=') {
                tok.type = TOK_GREATERTHANOREQUALS;
            }
            else {
                untokechar(chr);
            }

            return true;
        case '^': tok.type = TOK_CARET;         return true;
        case '|': tok.type = TOK_PIPE;          return true;
        case '&': tok.type = TOK_AMPERSAND;     return true;
        case '!': tok.type = TOK_NOT;           return true;
        case '~': tok.type = TOK_NEGATE;        return true;
        case '-':
            tok.type = TOK_MINUS;

            /* minus immediately followed by numeric digits means a negative number */
            chr = tokechar();
            untokechar(chr); /* we're peeking, actually */

            if (isdigit(chr)) {
                if (!toke(tok)) return false; // recursion!
                if (tok.type == TOK_UINT) {
                    tok.type = TOK_INT;
                    tok.intval.i = -tok.intval.i;
                }
                else if (tok.type == TOK_FLOAT) {
                    tok.floatval = -tok.floatval;
                }
                else {
                    /* wait, what? */
                    fprintf(stderr,"Unexpected token error, negative number '-' case\n");
                    return false;
                }
            }
            return true;
        default:
            break;
    };

    if (chr == '\"') { /* it's a string */
        tok.type = TOK_STRING;
        tok.string.clear();

        do {
            chr = tokechar();
            if (chr < 0) {
                tok.type = TOK_ERROR;
                return false;
            }
            if (chr == '\"') break;

            if (chr == '\\') {
                chr = tokechar();
                if (chr == '\\' || chr == '\'' || chr == '\"') {
                    tok.string += (char)chr;
                }
                else {
                    tok.type = TOK_ERROR;
                    return false;
                }
            }
            else {
                tok.string += (char)chr;
            }
        } while (true);

        return true;
    }
    else if (isdigit((char)chr)) {
        tok.type = TOK_UINT;
        tok.string = (char)chr;
        tok.intval.u = 0;

        if (chr == '0') {
            chr = tokechar();
            if (chr == 'x') {
                /* hexadecimal */
                tok.string += (char)chr;

                chr = tokechar();
                if (!isxdigit(chr)) {
                    tok.type = TOK_ERROR;
                    return false;
                }

                tok.string += (char)chr;

                do {
                    chr = tokechar();
                    if (!isxdigit(chr)) {
                        untokechar(chr);
                        break;
                    }

                    tok.string += (char)chr;
                } while (true);

                tok.intval.u = strtoull(tok.string.c_str(),NULL,0);
                return true;
            }
            else if (isdigit(chr)) {
                /* octal */
                tok.string += (char)chr;
            }
            else if (chr == '.') {
                tok.type = TOK_FLOAT;
                tok.string += (char)chr;
            }
            else {
                untokechar(chr);
                return true;
            }
        }

        /* decimal */
        do {
            chr = tokechar();
            if (chr == '.' && tok.type == TOK_UINT) {
                tok.type = TOK_FLOAT;
                tok.floatval = 0;
            }
            else if (!isdigit(chr)) {
                untokechar(chr);
                break;
            }

            tok.string += (char)chr;
        } while (true);

        if (tok.type == TOK_UINT)
            tok.intval.u = strtoull(tok.string.c_str(),NULL,0);
        else if (tok.type == TOK_FLOAT)
            tok.floatval = strtof(tok.string.c_str(),NULL);
        else
            abort();

        return true;
    }
    else if (isalpha((char)chr)) {
        tok.type = TOK_ERROR;
        tok.string = (char)toupper((char)chr);

        do {
            chr = tokechar();
            if (!isalpha(chr) && !isdigit(chr) && chr != '_') {
                untokechar(chr);
                break;
            }

            tok.string += (char)toupper((char)chr);
        } while (true);

        if (tok.string == "PREFIX") {
            tok.type = TOK_PREFIX;
            return true;
        }
        if (tok.string == "OPCODE") {
            tok.type = TOK_OPCODE;
            return true;
        }
        if (tok.string == "UNKNOWN") {
            tok.type = TOK_UNKNOWN;
            return true;
        }
        if (tok.string == "SILENT") {
            tok.type = TOK_SILENT;
            return true;
        }
        if (tok.string == "EXCEPTION") {
            tok.type = TOK_EXCEPTION;
            return true;
        }
        if (tok.string == "UD") {
            tok.type = TOK_UD;
            return true;
        }
        if (tok.string == "CODE") {
            tok.type = TOK_CODE;
            return true;
        }
        if (tok.string == "DEST") {
            tok.type = TOK_DEST;
            return true;
        }
        if (tok.string == "PARAM") {
            tok.type = TOK_PARAM;
            return true;
        }
        if (tok.string == "MRM") {
            tok.type = TOK_MRM;
            return true;
        }
        if (tok.string == "MOD") {
            tok.type = TOK_MOD;
            return true;
        }
        if (tok.string == "REG") {
            tok.type = TOK_REG;
            return true;
        }
        if (tok.string == "RM") {
            tok.type = TOK_RM;
            return true;
        }
        if (tok.string == "AL") {
            tok.type = TOK_AL;
            return true;
        }
        if (tok.string == "AH") {
            tok.type = TOK_AH;
            return true;
        }
        if (tok.string == "AX") {
            tok.type = TOK_AX;
            return true;
        }
        if (tok.string == "EAX") {
            tok.type = TOK_EAX;
            return true;
        }
        if (tok.string == "AV") {
            tok.type = TOK_AV;
            return true;
        }
        if (tok.string == "BL") {
            tok.type = TOK_BL;
            return true;
        }
        if (tok.string == "BH") {
            tok.type = TOK_BH;
            return true;
        }
        if (tok.string == "BX") {
            tok.type = TOK_BX;
            return true;
        }
        if (tok.string == "EBX") {
            tok.type = TOK_EBX;
            return true;
        }
        if (tok.string == "BV") {
            tok.type = TOK_BV;
            return true;
        }
        if (tok.string == "CL") {
            tok.type = TOK_CL;
            return true;
        }
        if (tok.string == "CH") {
            tok.type = TOK_CH;
            return true;
        }
        if (tok.string == "CX") {
            tok.type = TOK_CX;
            return true;
        }
        if (tok.string == "ECX") {
            tok.type = TOK_ECX;
            return true;
        }
        if (tok.string == "CV") {
            tok.type = TOK_CV;
            return true;
        }
        if (tok.string == "DL") {
            tok.type = TOK_DL;
            return true;
        }
        if (tok.string == "DH") {
            tok.type = TOK_DH;
            return true;
        }
        if (tok.string == "DX") {
            tok.type = TOK_DX;
            return true;
        }
        if (tok.string == "EDX") {
            tok.type = TOK_EDX;
            return true;
        }
        if (tok.string == "DV") {
            tok.type = TOK_DV;
            return true;
        }
        if (tok.string == "SI") {
            tok.type = TOK_SI;
            return true;
        }
        if (tok.string == "ESI") {
            tok.type = TOK_ESI;
            return true;
        }
        if (tok.string == "SIV") {
            tok.type = TOK_SIV;
            return true;
        }
        if (tok.string == "DI") {
            tok.type = TOK_DI;
            return true;
        }
        if (tok.string == "EDI") {
            tok.type = TOK_EDI;
            return true;
        }
        if (tok.string == "DIV") {
            tok.type = TOK_DIV;
            return true;
        }
        if (tok.string == "BP") {
            tok.type = TOK_BP;
            return true;
        }
        if (tok.string == "EBP") {
            tok.type = TOK_EBP;
            return true;
        }
        if (tok.string == "BPV") {
            tok.type = TOK_BPV;
            return true;
        }
        if (tok.string == "SP") {
            tok.type = TOK_SP;
            return true;
        }
        if (tok.string == "ESP") {
            tok.type = TOK_ESP;
            return true;
        }
        if (tok.string == "SPV") {
            tok.type = TOK_SPV;
            return true;
        }
        if (tok.string == "IP") {
            tok.type = TOK_IP;
            return true;
        }
        if (tok.string == "EIP") {
            tok.type = TOK_EIP;
            return true;
        }
        if (tok.string == "IPV") {
            tok.type = TOK_IPV;
            return true;
        }
        if (tok.string == "FLAGS") {
            tok.type = TOK_FLAGS;
            return true;
        }
        if (tok.string == "EFLAGS") {
            tok.type = TOK_EFLAGS;
            return true;
        }
        if (tok.string == "FLAGSV") {
            tok.type = TOK_FLAGSV;
            return true;
        }
        if (tok.string == "CS") {
            tok.type = TOK_CS;
            return true;
        }
        if (tok.string == "DS") {
            tok.type = TOK_DS;
            return true;
        }
        if (tok.string == "ES") {
            tok.type = TOK_ES;
            return true;
        }
        if (tok.string == "FS") {
            tok.type = TOK_FS;
            return true;
        }
        if (tok.string == "GS") {
            tok.type = TOK_GS;
            return true;
        }
        if (tok.string == "SS") {
            tok.type = TOK_SS;
            return true;
        }
        if (tok.string == "A") {
            tok.type = TOK_A;
            return true;
        }
        if (tok.string == "B") {
            tok.type = TOK_B;
            return true;
        }
        if (tok.string == "C") {
            tok.type = TOK_C;
            return true;
        }
        if (tok.string == "D") {
            tok.type = TOK_D;
            return true;
        }
        if (tok.string == "E") {
            tok.type = TOK_E;
            return true;
        }
        if (tok.string == "F") {
            tok.type = TOK_F;
            return true;
        }
        if (tok.string == "G") {
            tok.type = TOK_G;
            return true;
        }
        if (tok.string == "H") {
            tok.type = TOK_H;
            return true;
        }
        if (tok.string == "I") {
            tok.type = TOK_I;
            return true;
        }
        if (tok.string == "J") {
            tok.type = TOK_J;
            return true;
        }
        if (tok.string == "K") {
            tok.type = TOK_K;
            return true;
        }
        if (tok.string == "L") {
            tok.type = TOK_L;
            return true;
        }
        if (tok.string == "M") {
            tok.type = TOK_M;
            return true;
        }
        if (tok.string == "N") {
            tok.type = TOK_N;
            return true;
        }
        if (tok.string == "O") {
            tok.type = TOK_O;
            return true;
        }
        if (tok.string == "P") {
            tok.type = TOK_P;
            return true;
        }
        if (tok.string == "Q") {
            tok.type = TOK_Q;
            return true;
        }
        if (tok.string == "R") {
            tok.type = TOK_R;
            return true;
        }
        if (tok.string == "S") {
            tok.type = TOK_S;
            return true;
        }
        if (tok.string == "T") {
            tok.type = TOK_T;
            return true;
        }
        if (tok.string == "U") {
            tok.type = TOK_U;
            return true;
        }
        if (tok.string == "V") {
            tok.type = TOK_V;
            return true;
        }
        if (tok.string == "W") {
            tok.type = TOK_W;
            return true;
        }
        if (tok.string == "X") {
            tok.type = TOK_X;
            return true;
        }
        if (tok.string == "Y") {
            tok.type = TOK_Y;
            return true;
        }
        if (tok.string == "Z") {
            tok.type = TOK_Z;
            return true;
        }
        if (tok.string == "ALL") {
            tok.type = TOK_ALL;
            return true;
        }
        if (tok.string == "DW") {
            tok.type = TOK_DW;
            return true;
        }
        if (tok.string == "TW") {
            tok.type = TOK_TW;
            return true;
        }
        if (tok.string == "QW") {
            tok.type = TOK_QW;
            return true;
        }
        if (tok.string == "HV") {
            tok.type = TOK_HV;
            return true;
        }
        if (tok.string == "SB") {
            tok.type = TOK_SB;
            return true;
        }
        if (tok.string == "SW") {
            tok.type = TOK_SW;
            return true;
        }
        if (tok.string == "SDW") {
            tok.type = TOK_SDW;
            return true;
        }
        if (tok.string == "SV") {
            tok.type = TOK_SV;
            return true;
        }
        if (tok.string == "SHV") {
            tok.type = TOK_SHV;
            return true;
        }
        if (tok.string == "SQW") {
            tok.type = TOK_SQW;
            return true;
        }
        if (tok.string == "FPW") {
            tok.type = TOK_FPW;
            return true;
        }
        if (tok.string == "FPDW") {
            tok.type = TOK_FPDW;
            return true;
        }
        if (tok.string == "FPV") {
            tok.type = TOK_FPV;
            return true;
        }
        if (tok.string == "GENERAL") {
            tok.type = TOK_GENERAL;
            return true;
        }
        if (tok.string == "SEG") {
            tok.type = TOK_SEG;
            return true;
        }
        if (tok.string == "F32") {
            tok.type = TOK_F32;
            return true;
        }
        if (tok.string == "F64") {
            tok.type = TOK_F64;
            return true;
        }
        if (tok.string == "F80") {
            tok.type = TOK_F80;
            return true;
        }
        if (tok.string == "F87ENV") {
            tok.type = TOK_F87ENV;
            return true;
        }
        if (tok.string == "F87STATE") {
            tok.type = TOK_F87STATE;
            return true;
        }
        if (tok.string == "IMMEDIATE") {
            tok.type = TOK_IMMEDIATE;
            return true;
        }
        if (tok.string == "DESC") {
            tok.type = TOK_DESC;
            return true;
        }
        if (tok.string == "COMMENT") {
            tok.type = TOK_COMMENT;
            return true;
        }
        if (tok.string == "LIMIT") {
            tok.type = TOK_LIMIT;
            return true;
        }
        if (tok.string == "NONE") {
            tok.type = TOK_WORD_NONE;
            return true;
        }
        if (tok.string == "IF") {
            tok.type = TOK_IF;
            return true;
        }
        if (tok.string == "VALUE") {
            tok.type = TOK_VALUE;
            return true;
        }
        if (tok.string == "LOG") {
            tok.type = TOK_LOG;
            return true;
        }
        if (tok.string == "FORMAT") {
            tok.type = TOK_FORMAT;
            return true;
        }
        if (tok.string == "SET") {
            tok.type = TOK_SET;
            return true;
        }
        if (tok.string == "UNSET") {
            tok.type = TOK_UNSET;
            return true;
        }
        if (tok.string == "ISSET") {
            tok.type = TOK_ISSET;
            return true;
        }
        if (tok.string == "ERROR") {
            tok.type = TOK_WORD_ERROR;
            return true;
        }
        if (tok.string == "TRUE") {
            tok.type = TOK_BOOLEAN;
            tok.intval.u = 1;
            return true;
        }
        if (tok.string == "FALSE") {
            tok.type = TOK_BOOLEAN;
            tok.intval.u = 0;
            return true;
        }
        if (tok.string == "WARNING") {
            tok.type = TOK_WARNING;
            return true;
        }
        if (tok.string == "NOT") {
            tok.type = TOK_NOT;
            return true;
        }
        if (tok.string == "XOR") {
            tok.type = TOK_XOR;
            return true;
        }
        if (tok.string == "AND") {
            tok.type = TOK_AND;
            return true;
        }
        if (tok.string == "OR") {
            tok.type = TOK_OR;
            return true;
        }
        if (tok.string == "NEGATE") {
            tok.type = TOK_NEGATE;
            return true;
        }
        if (tok.string == "HEXSTRING") {
            tok.type = TOK_HEXSTRING;
            return true;
        }
        if (tok.string == "UNSIGNED") {
            tok.type = TOK_UNSIGNED;
            return true;
        }
        if (tok.string == "SIGNED") {
            tok.type = TOK_SIGNED;
            return true;
        }
        if (tok.string == "OCTSTRING") {
            tok.type = TOK_OCTSTRING;
            return true;
        }
        if (tok.string == "BINSTRING") {
            tok.type = TOK_BINSTRING;
            return true;
        }
        if (tok.string == "STRING") {
            tok.type = TOK_WORD_STRING;
            return true;
        }
        if (tok.string == "VALUETYPE") {
            tok.type = TOK_VALUETYPE;
            return true;
        }
    }

    tok.type = TOK_ERROR;
    return false;
}

int parse_argv(int argc,char **argv) {
    char *a;
    int i;

    for (i=1;i < argc;) {
        a = argv[i++];

        if (*a == '-') {
            do { a++; } while (*a == '-');

            if (!strcmp(a,"i")) {
                a = argv[i++];
                if (a == NULL) return 1;
                srcfile = a;
            }
            else {
                fprintf(stderr,"Unknown sw %s\n",a);
                return 1;
            }
        }
        else {
            fprintf(stderr,"Unexpected arg %s\n",a);
            return 1;
        }
    }

    if (srcfile.empty()) {
        fprintf(stderr,"Source file -i required\n");
        return 1;
    }

    return 0;
}

typedef std::vector<uint8_t> ByteSpec;

class OpcodeSpec {
public:
    std::vector<ByteSpec>       bytes;                  // opcode byte sequence
    unsigned int                immed_byte_1 = 0;       // if set, immediate byte follows mod/reg/rm (token)
    unsigned int                immed_byte_2 = 0;       // if set, immediate byte follows mod/reg/rm (token)
    unsigned int                type = 0;               // token type (PREFIX, OPCODE, etc)
    std::string                 description;
    std::string                 comment;
    std::string                 name;
    bool                        modregrm = false;       // if mod/reg/rm byte follows opcode
    unsigned int                prefix_seg_assign = 0;  // token segment override assignment (PREFIX)
};

static tokenstate_t tokenstate_t_none;

std::vector<OpcodeSpec>         opcodes;

int                             unknown_opcode = -1;
int                             opcode_limit = -1;

class tokenlist : public std::vector<tokenstate_t> {
public:
    tokenlist() : std::vector<tokenstate_t>() { }
    ~tokenlist() { }
public:
    tokenstate_t &get(const size_t i) {
        if (i >= size()) return tokenstate_t_none;
        return (std::vector<tokenstate_t>::operator[])(i);
    }
    tokenstate_t &peek(const size_t i=0) {
        return get(read+i);
    }
    tokenstate_t &next(void) {
        return get(read++);
    }
    void discard(const size_t i=1) {
        read += i;
    }
    bool eof(void) const {
        return read >= size();
    }
public:
    size_t              read = 0;
};

bool valid_immediate_size_token(unsigned int tok) {
    switch (tok) {
        case TOK_B:
        case TOK_SB:
        case TOK_W:
        case TOK_SW:
        case TOK_DW:
        case TOK_SDW:
        case TOK_V:
        case TOK_SV:
        case TOK_HV:
        case TOK_SHV:
        case TOK_FPV:
        case TOK_FPW:
        case TOK_FPDW:
            return true;
        default:
            break;
    };

    return false;
}

/* immediate(token).
 * immediate token already parsed by caller, we're at ( now */
unsigned int parse_code_immediate_spec(tokenlist &tokens) {
    unsigned int r = TOK_NONE;

    if (tokens.peek().type != TOK_OPEN_PARENS) return TOK_NONE;
    tokens.discard();

    auto &p1 = tokens.next();
    if (!valid_immediate_size_token(p1.type)) return TOK_NONE;
    r = p1.type;

    /* should be ) now */
    if (tokens.peek().type != TOK_CLOSE_PARENS) return TOK_NONE;
    tokens.discard();

    return r;
}

bool eval_isset(tokenstate_t &token,tokenlist &tokens);
bool eval_value(tokenstate_t &token,tokenlist &tokens);
bool eval_format(std::string &msg,tokenlist &tokens);
bool eval_valuetype(tokenstate_t &token,tokenlist &tokens);

bool eval_if_condition(tokenstate_t &result,tokenlist &tokens);

bool eval_if_condition_block(tokenstate_t &result,tokenlist &tokens) {
    // caller has already parsed TOK_IF
    // we parse the tokens following "IF"

    tokenstate_t t = tokens.next();

    if (t.type == TOK_UINT || t.type == TOK_INT || t.type == TOK_FLOAT || t.type == TOK_STRING || t.type == TOK_BOOLEAN) {
        result = t;
        return true;
    }
    else if (t.type == TOK_ISSET) {
        tokenstate_t subtoken;

        if (!eval_isset(subtoken,tokens))
            return false;

        result = subtoken;
        return true;
    }
    else if (t.type == TOK_VALUE) {
        tokenstate_t subtoken;

        if (!eval_value(subtoken,tokens))
            return false;

        result = subtoken;
        return true;
    }
    else if (t.type == TOK_VALUETYPE) {
        tokenstate_t subtoken;

        if (!eval_valuetype(subtoken,tokens))
            return false;

        result = subtoken;
        return true;
    }
    else if (t.type == TOK_FORMAT) {
        std::string submsg;

        if (!eval_format(submsg,tokens))
            return false;

        result.type = TOK_STRING;
        result.string = submsg;
        return true;
    }
    else if (t.type == TOK_NOT) {
        /* not ... */
        tokenstate_t tmp;

        if (!eval_if_condition(tmp,tokens))
            return false;

        result.type = TOK_BOOLEAN;
        result.intval.u = !tmp.to_bool();
        return true;
    }
    else if (t.type == TOK_NEGATE) {
        /* not ... */
        tokenstate_t tmp;

        if (!eval_if_condition(tmp,tokens))
            return false;

        if (tmp.type == TOK_UINT || tmp.type == TOK_INT || tmp.type == TOK_BOOLEAN) {
            result = tmp;
            result.intval.u = ~result.intval.u;
            return true;
        }
        else if (tmp.type == TOK_FLOAT) {
            fprintf(stderr,"Cannot negate a float\n");
            return false;
        }
        else {
            fprintf(stderr,"Cannot negate a non-number\n");
            return false;
        }
    }
    else if (t.type == TOK_MINUS) {
        /* not ... */
        tokenstate_t tmp;

        if (!eval_if_condition(tmp,tokens))
            return false;

        if (tmp.type == TOK_UINT || tmp.type == TOK_INT || tmp.type == TOK_BOOLEAN) {
            result = tmp;
            result.intval.i = -result.intval.i;
            result.type = TOK_INT; // becomes signed
            return true;
        }
        else if (tmp.type == TOK_FLOAT) {
            result = tmp;
            result.floatval = -result.floatval;
            return true;
        }
        else {
            fprintf(stderr,"Cannot negate a non-number\n");
            return false;
        }
    }
    /* hex(expr) */
    else if (t.type == TOK_HEXSTRING) {
        if (tokens.next().type != TOK_OPEN_PARENS)
            return false;

        tokenstate_t tmp;

        if (!eval_if_condition(tmp,tokens))
            return false;

        result.type = TOK_STRING;
        result.string = tmp.int_to_hex_string();

        if (tokens.next().type != TOK_CLOSE_PARENS)
            return false;

        return true;
    }
    /* oct(expr) */
    else if (t.type == TOK_OCTSTRING) {
        if (tokens.next().type != TOK_OPEN_PARENS)
            return false;

        tokenstate_t tmp;

        if (!eval_if_condition(tmp,tokens))
            return false;

        result.type = TOK_STRING;
        result.string = tmp.int_to_oct_string();

        if (tokens.next().type != TOK_CLOSE_PARENS)
            return false;

        return true;
    }
    /* bin(expr) */
    else if (t.type == TOK_BINSTRING) {
        if (tokens.next().type != TOK_OPEN_PARENS)
            return false;

        tokenstate_t tmp;

        if (!eval_if_condition(tmp,tokens))
            return false;

        result.type = TOK_STRING;
        result.string = tmp.int_to_bin_string();

        if (tokens.next().type != TOK_CLOSE_PARENS)
            return false;

        return true;
    }
    /* string(expr) */
    else if (t.type == TOK_WORD_STRING) {
        if (tokens.next().type != TOK_OPEN_PARENS)
            return false;

        tokenstate_t tmp;

        if (!eval_if_condition(tmp,tokens))
            return false;

        result.type = TOK_STRING;
        result.string = tmp.to_string();

        if (tokens.next().type != TOK_CLOSE_PARENS)
            return false;

        return true;
    }
    /* unsigned(expr) */
    else if (t.type == TOK_UNSIGNED) {
        if (tokens.next().type != TOK_OPEN_PARENS)
            return false;

        tokenstate_t tmp;

        if (!eval_if_condition(tmp,tokens))
            return false;

        result = tmp;
        if (result.type == TOK_INT) result.type = TOK_UINT;

        if (tokens.next().type != TOK_CLOSE_PARENS)
            return false;

        return true;
    }
    /* signed(expr) */
    else if (t.type == TOK_SIGNED) {
        if (tokens.next().type != TOK_OPEN_PARENS)
            return false;

        tokenstate_t tmp;

        if (!eval_if_condition(tmp,tokens))
            return false;

        result = tmp;
        if (result.type == TOK_UINT || result.type == TOK_BOOLEAN) result.type = TOK_INT;

        if (tokens.next().type != TOK_CLOSE_PARENS)
            return false;

        return true;
    }

    return false;
}

bool eval_if_condition(tokenstate_t &result,tokenlist &tokens) {
    if (!eval_if_condition_block(result,tokens))
        return false;

    /* if the next token is == then this is a comparison */
    if (tokens.peek().type == TOK_DOUBLEEQUALS) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition_block(res2,tokens))
            return false;

        bool expr_result = (result == res2);

        result.type = TOK_BOOLEAN;
        result.intval.u = expr_result ? 1ull : 0ull;
    }
    else if (tokens.peek().type == TOK_GREATERTHAN) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition_block(res2,tokens))
            return false;

        bool expr_result = (result > res2);

        result.type = TOK_BOOLEAN;
        result.intval.u = expr_result ? 1ull : 0ull;
    }
    else if (tokens.peek().type == TOK_LESSTHAN) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition_block(res2,tokens))
            return false;

        bool expr_result = (result < res2);

        result.type = TOK_BOOLEAN;
        result.intval.u = expr_result ? 1ull : 0ull;
    }
    else if (tokens.peek().type == TOK_GREATERTHANOREQUALS) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition_block(res2,tokens))
            return false;

        bool expr_result = (result >= res2);

        result.type = TOK_BOOLEAN;
        result.intval.u = expr_result ? 1ull : 0ull;
    }
    else if (tokens.peek().type == TOK_LESSTHANOREQUALS) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition_block(res2,tokens))
            return false;

        bool expr_result = (result <= res2);

        result.type = TOK_BOOLEAN;
        result.intval.u = expr_result ? 1ull : 0ull;
    }
    else if (tokens.peek().type == TOK_AND) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition(res2,tokens))
            return false;

        bool expr_result = (result.to_bool() && res2.to_bool());

        result.type = TOK_BOOLEAN;
        result.intval.u = expr_result ? 1ull : 0ull;
    }
    else if (tokens.peek().type == TOK_OR) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition(res2,tokens))
            return false;

        bool expr_result = (result.to_bool() || res2.to_bool());

        result.type = TOK_BOOLEAN;
        result.intval.u = expr_result ? 1ull : 0ull;
    }
    else if (tokens.peek().type == TOK_XOR) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition(res2,tokens))
            return false;

        bool expr_result = ((result.to_bool()?1:0) ^ (res2.to_bool()?1:0));

        result.type = TOK_BOOLEAN;
        result.intval.u = expr_result ? 1ull : 0ull;
    }
    else if (tokens.peek().type == TOK_AMPERSAND) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition(res2,tokens))
            return false;

        unsigned long long res = result.to_intval_u() & res2.to_intval_u();

        result.intval.u = res;
        if (result.type == TOK_INT || res2.type == TOK_INT)
            result.type = TOK_INT;
        else if (result.type == TOK_FLOAT || res2.type == TOK_FLOAT)
            result.type = TOK_INT;
        else
            result.type = TOK_UINT;
    }
    else if (tokens.peek().type == TOK_PIPE) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition(res2,tokens))
            return false;

        unsigned long long res = result.to_intval_u() | res2.to_intval_u();

        result.intval.u = res;
        if (result.type == TOK_INT || res2.type == TOK_INT)
            result.type = TOK_INT;
        else if (result.type == TOK_FLOAT || res2.type == TOK_FLOAT)
            result.type = TOK_INT;
        else
            result.type = TOK_UINT;
    }
    else if (tokens.peek().type == TOK_CARET) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition(res2,tokens))
            return false;

        unsigned long long res = result.to_intval_u() ^ res2.to_intval_u();

        result.intval.u = res;
        if (result.type == TOK_INT || res2.type == TOK_INT)
            result.type = TOK_INT;
        else if (result.type == TOK_FLOAT || res2.type == TOK_FLOAT)
            result.type = TOK_INT;
        else
            result.type = TOK_UINT;
    }
 
    return true;
}

bool read_opcode_spec_opcode_parens(tokenlist &parent_tokens,OpcodeSpec &spec) {
    /* caller already read '(' */
    tokenlist tokens;
    int parens = 1;

    do {
        auto &next = parent_tokens.next();

        if (next.type == TOK_CLOSE_PARENS) {
            if (--parens <= 0)
                break;
        }
        else if (next.type == TOK_OPEN_PARENS) {
            parens++;
        }
        else if (next.type == TOK_NONE) {
            fprintf(stderr,"Opcode parens unexpected end\n");
            return false;
        }

        tokens.push_back(next);
    } while (1);

    if (tokens.empty())
        return true;

    /* desc "string" */
    if (tokens.peek(0).type == TOK_DESC && tokens.peek(1).type == TOK_STRING) {
        spec.description = tokens.peek(1).string;
        tokens.discard(2);

        if (!tokens.eof()) {
            fprintf(stderr,"Desc unexpected tokens\n");
            return false;
        }

        return true;
    }

    /* comment "string" */
    if (tokens.peek(0).type == TOK_COMMENT && tokens.peek(1).type == TOK_STRING) {
        if (!spec.comment.empty()) spec.comment += "\n";
        spec.comment += tokens.peek(1).string;
        tokens.discard(2);

        if (!tokens.eof()) {
            fprintf(stderr,"Comment unexpected tokens\n");
            return false;
        }

        return true;
    }

    /* seg=(register) */
    if (tokens.peek(0).type == TOK_SEG && tokens.peek(1).type == TOK_EQUAL) {
        tokens.discard(2);

        if (spec.type != TOK_PREFIX) {
            fprintf(stderr,"Segment override specifications only allowed for prefixes\n");
            return false;
        }

        auto &n = tokens.next();
        if (n.type == TOK_CS || n.type == TOK_DS || n.type == TOK_ES || n.type == TOK_FS || n.type == TOK_GS || n.type == TOK_SS) {
            spec.prefix_seg_assign = n.type;
        }
        else {
            fprintf(stderr,"Unexpected token past equals\n");
            return false;
        }

        if (!tokens.eof()) {
            fprintf(stderr,"Seg unexpected tokens\n");
            return false;
        }

        return true;
    }

    /* code (byte or byte ranges) [mod/reg/rm] [immediate(...)] */
    if (tokens.peek(0).type == TOK_CODE) {
        tokens.discard(1);

        while (!tokens.eof() && tokens.peek().type == TOK_UINT) {
            auto &next = tokens.next();
            ByteSpec bs;

            /* byte range (0xA0-0xA3) */
            if (tokens.peek().type == TOK_MINUS) {
                unsigned int start = next.intval.u;
                tokens.discard(); // TOK_MINUS
                auto &next2 = tokens.next();
                if (next2.type != TOK_UINT) break;
                unsigned int end = next2.intval.u;

                if (start > end || start > 255 || end > 255) {
                    fprintf(stderr,"Invalid range\n");
                    return false;
                }

                for (unsigned int c=start;c <= end;c++)
                    bs.push_back((uint8_t)c);
            }
            /* single byte */
            else {
                bs.push_back((uint8_t)next.intval.u);
            }

            /* store it */
            spec.bytes.push_back(bs);

            /* if the next token is a comma, then discard it and loop again.
             * else exit the loop */
            if (tokens.peek().type == TOK_COMMA) {
                tokens.discard();
                continue;
            }
            else {
                break;
            }
        }

        /* whether or not the instruction has a mod/reg/rm byte */
        if (tokens.peek().type == TOK_MRM) {
            tokens.discard();
            spec.modregrm = true;
        }

        /* whether the instruction has immediate operands "immediate(v)" */
        if (tokens.peek().type == TOK_IMMEDIATE) {
            tokens.discard();
            if ((spec.immed_byte_1=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid immediate spec\n");
                return false;
            }
        }
        if (tokens.peek().type == TOK_IMMEDIATE) {
            tokens.discard();
            if ((spec.immed_byte_2=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid immediate spec\n");
                return false;
            }
        }

        if (!tokens.eof()) {
            fprintf(stderr,"Code unexpected tokens\n");
            return false;
        }

        return true;
    }

    fprintf(stderr,"Opcode parens unexpected token %s\n",tokens.peek().type_str());
    return false;
}

bool read_opcode_spec(OpcodeSpec &spec,tokenlist &tokens) {
    if (tokens.empty())
        return true; /* not an error */

    /* prefix "name" (...) (...) (...) */
    /* opcode "name" (...) (...) (...) */
    if ((tokens.peek(0).type == TOK_PREFIX || tokens.peek(1).type == TOK_OPCODE) && tokens.peek(1).type == TOK_STRING) {
        spec.type = tokens.peek(0).type;
        spec.name = tokens.peek(1).string;
        for (auto &c : spec.name) c = toupper(c);
        tokens.discard(2);

        if (spec.name.empty()) {
            fprintf(stderr,"Prefix requires name\n");
            return false;
        }

        while (!tokens.eof() && tokens.peek().type == TOK_OPEN_PARENS) {
            tokens.discard();
            if (!read_opcode_spec_opcode_parens(/*&*/tokens,/*&*/spec))
                return false;
        }

        if (!tokens.eof())
            goto parse_error;

        return true;
    }
    /* unknown opcode (silent|ud); */
    else if (tokens.peek(0).type == TOK_UNKNOWN && tokens.peek(1).type == TOK_OPCODE) {
        tokens.discard(2);

        if (!tokens.eof()) {
            if (tokens.peek().type == TOK_UD || tokens.peek().type == TOK_SILENT) {
                if (unknown_opcode >= 0 && unknown_opcode != tokens.peek().type)
                    fprintf(stderr,"WARNING: Unknown opcode behavior, conflicting specification\n");

                unknown_opcode = tokens.peek().type;
                tokens.discard();
            }
            else {
                goto parse_error;
            }
        }

        if (!tokens.eof())
            goto parse_error;

        return true;
    }
    /* opcode limit (none|number); */
    else if (tokens.peek(0).type == TOK_OPCODE && tokens.peek(1).type == TOK_LIMIT) {
        tokens.discard(2);

        if (!tokens.eof()) {
            if (tokens.peek().type == TOK_WORD_NONE) {
                if (opcode_limit > 0) fprintf(stderr,"Opcode limit specified multiple conflicting times\n");
                opcode_limit = 0;
                tokens.discard();
            }
            else if (tokens.peek().type == TOK_UINT) {
                if (tokens.peek().intval.u < 1 || tokens.peek().intval.u > 65536)
                    goto parse_error;

                if (opcode_limit >= 0 && opcode_limit != (int)tokens.peek().intval.u)
                    fprintf(stderr,"Opcode limit specified multiple conflicting times\n");

                opcode_limit = tokens.peek().intval.u;
                tokens.discard();
            }
            else {
                goto parse_error;
            }
        }

        if (!tokens.eof())
            goto parse_error;

        return true;
    }

parse_error:
    fprintf(stderr,"Parse error\n");
    return false;
unexpected_end:
    fprintf(stderr,"Unexpected end of opcode\n");
    return false;
unexpected_token:
    fprintf(stderr,"Unexpected token\n");
    return false;
}

bool do_opcode_spec(tokenlist &tokens) {
    OpcodeSpec spec;

    if (!read_opcode_spec(/*&*/spec,/*&*/tokens))
        return false;

    opcodes.push_back(std::move(spec));
    return true;
}

/* VALUETYPE ( STRING ) */
bool eval_valuetype(tokenstate_t &token,tokenlist &tokens) {
    token.type = TOK_NONE;

    // caller has already consumed TOK_VALUE
    // next token should be parenthesis
    // end of the message should be closed paranethesis
    if (tokens.peek(0).type != TOK_OPEN_PARENS) return false;
    tokens.discard();

    if (tokens.peek(0).type == TOK_STRING) {
        std::string name = tokens.peek(0).string;
        tokens.discard();

        if (name.empty()) return false;

        if (tokens.peek(0).type != TOK_CLOSE_PARENS) return false;
        tokens.discard();

        auto i = defines.find(name);
        if (i != defines.end()) {
            token.type = TOK_STRING;
            token.string = i->second.type_str();
        }

        return true;
    }

    return false;
}

/* VALUE ( STRING ) */
bool eval_value(tokenstate_t &token,tokenlist &tokens) {
    token.type = TOK_NONE;

    // caller has already consumed TOK_VALUE
    // next token should be parenthesis
    // end of the message should be closed paranethesis
    if (tokens.peek(0).type != TOK_OPEN_PARENS) return false;
    tokens.discard();

    if (tokens.peek(0).type == TOK_STRING) {
        std::string name = tokens.peek(0).string;
        tokens.discard();

        if (name.empty()) return false;

        if (tokens.peek(0).type != TOK_CLOSE_PARENS) return false;
        tokens.discard();

        auto i = defines.find(name);
        if (i != defines.end()) token = i->second;

        return true;
    }

    return false;
}

/* ISSET ( STRING ) */
bool eval_isset(tokenstate_t &token,tokenlist &tokens) {
    token.type = TOK_NONE;

    // caller has already consumed TOK_ISSET
    // next token should be parenthesis
    // end of the message should be closed paranethesis
    if (tokens.peek(0).type != TOK_OPEN_PARENS) return false;
    tokens.discard();

    if (tokens.peek(0).type == TOK_STRING) {
        std::string name = tokens.peek(0).string;
        tokens.discard();

        if (name.empty()) return false;

        if (tokens.peek(0).type != TOK_CLOSE_PARENS) return false;
        tokens.discard();

        auto i = defines.find(name);
        token.type = TOK_BOOLEAN;
        token.intval.u = (i != defines.end()) ? 1u : 0u;

        return true;
    }

    return false;
}

void LOG_OUTPUT(const std::string &msg) {
    fprintf(stderr,"log output: %s\n",msg.c_str());
}

void WARN_OUTPUT(const std::string &msg) {
    fprintf(stderr,"warning output: %s\n",msg.c_str());
}

void ERR_OUTPUT(const std::string &msg) {
    fprintf(stderr,"error output: %s\n",msg.c_str());
}

bool eval_format(std::string &msg,tokenlist &tokens) {
    msg.clear();

    // caller has already consumed TOK_FORMAT
    // next token should be parenthesis
    // end of the message should be closed paranethesis
    if (tokens.peek(0).type != TOK_OPEN_PARENS) return false;
    tokens.discard();

    do {
        tokenstate_t result;

        if (tokens.peek(0).type == TOK_FORMAT) {
            tokens.discard();

            // subexpression
            std::string submsg;

            if (!eval_format(submsg,tokens))
                return false;

            msg += submsg;
        }
        else if (tokens.peek(0).type == TOK_CLOSE_PARENS) {
            tokens.discard();
            break;
        }
        else {
            if (!eval_if_condition(/*&*/result,/*&*/tokens)) {
                fprintf(stderr,"'If' in format condition error\n");
                return false;
            }

            msg += result.to_string();
        }
    } while (1);

    return true;
}

bool process_block(tokenlist &tokens) {
    /* IF ... */
    /* if condition is true, then continue parsing below the remaining tokens.
     * if condition is false, ignore the tokens and move on. */
    if (tokens.peek(0).type == TOK_IF && tokens.peek(1).type != TOK_NONE) {
        tokens.discard(); // discard IF

        tokenstate_t result;

        if (!eval_if_condition(/*&*/result,/*&*/tokens)) {
            fprintf(stderr,"'If' condition error\n");
            return false;
        }

        if (!result.to_bool())
            return true;

        /* fall through to parse tokens after IF statement */
    }

    /* log ... */
    /* error ... */
    /* warning ... */
    if (tokens.peek().type == TOK_LOG || tokens.peek().type == TOK_WORD_ERROR || tokens.peek().type == TOK_WARNING) {
        unsigned int tok = tokens.peek().type;
        tokens.discard();

        std::string msg;

        tokenstate_t result;

        if (!eval_if_condition(/*&*/result,/*&*/tokens)) {
            fprintf(stderr,"'If' condition error\n");
            return false;
        }

        msg = result.to_string();

        if (tok == TOK_LOG)
            LOG_OUTPUT(msg);
        else if (tok == TOK_WARNING)
            WARN_OUTPUT(msg);
        else if (tok == TOK_WORD_ERROR)
            ERR_OUTPUT(msg);
        else
            abort();//BUG

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        return true;
    }

    /* unset "name" */
    if (tokens.peek(0).type == TOK_UNSET && tokens.peek(1).type == TOK_STRING) {
        std::string name = tokens.peek(1).string;
        tokens.discard(2);

        if (name.empty()) {
            fprintf(stderr,"name is empty\n");
            return false;
        }

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        {
            auto i = defines.find(name);
            if (i != defines.end())
                defines.erase(i);
        }

        return true;
    }
    /* set "name" ... */
    if (tokens.peek(0).type == TOK_SET && tokens.peek(1).type == TOK_STRING) {
        std::string name = tokens.peek(1).string;
        tokens.discard(2);

        if (name.empty()) {
            fprintf(stderr,"name is empty\n");
            return false;
        }

        tokenstate_t result;

        if (!eval_if_condition(/*&*/result,/*&*/tokens)) {
            fprintf(stderr,"'If' condition error\n");
            return false;
        }
        defines[name] = result;

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        return true;
    }
    /* comment "string" */
    if (tokens.peek(0).type == TOK_COMMENT && tokens.peek(1).type == TOK_STRING) {
        tokens.discard(2);

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        return true;
    }

    if (!do_opcode_spec(/*&*/tokens))
        return false;

    return true;
}

bool read_opcode_block(void) {
    do {
        tokenlist tokens;

        do {
            tokenstate_t tok;

            if (!toke(/*&*/tok)) {
                if (tok.type == TOK_ERROR)
                    goto token_error;
                if (tokens.empty())
                    return false;

                goto unexpected_end;
            }

            if (tok.type == TOK_SEMICOLON)
                break;

            tokens.push_back(std::move(tok));
        } while(1);

        if (!process_block(/*&*/tokens))
            break;
    } while (1);

parse_error:
    fprintf(stderr,"Parse error\n");
    return false;
token_error:
    fprintf(stderr,"Token error\n");
    return false;
unexpected_end:
    fprintf(stderr,"Unexpected end of opcode\n");
    return false;
unexpected_token:
    fprintf(stderr,"Unexpected token\n");
    return false;
}

int main(int argc,char **argv) {
    if (parse_argv(argc,argv))
        return 1;

    if ((srcfp=fopen(srcfile.c_str(),"r")) == NULL) {
        fprintf(stderr,"Unable to open file '%s', %s\n",srcfile.c_str(),strerror(errno));
        return 1;
    }

    while (read_opcode_block());

    if (opcode_limit < 0) {
        opcode_limit = 0; // none
        fprintf(stderr,"WARNING: Opcode limit not specified 'opcode limit ...'\n");
    }
    if (unknown_opcode < 0) {
        unknown_opcode = TOK_SILENT;
        fprintf(stderr,"WARNING: Unknown opcode behavior not specified 'unknown opcode ...'\n");
    }

#if 0
    {
        tokenstate_t tok;

        while (toke(/*&*/tok)) {
            fprintf(stderr,"%s(%d)\n",tok.type_str(),tok.type);
            if (tok.type == TOK_STRING) fprintf(stderr," \"%s\"\n",tok.string.c_str());
            if (tok.type == TOK_UINT) fprintf(stderr," 0x%llx/%llu\n",(unsigned long long)tok.intval.u,(unsigned long long)tok.intval.u);
            if (tok.type == TOK_INT) fprintf(stderr," 0x%llx/%lld\n",(signed long long)tok.intval.i,(signed long long)tok.intval.i);
            if (tok.type == TOK_FLOAT) fprintf(stderr," %.30Lf\n",(long double)tok.floatval);
        }

        if (tok.type == TOK_ERROR)
            fprintf(stderr,"Parse error\n");
    }
#endif

    fclose(srcfp);
    return 0;
}

