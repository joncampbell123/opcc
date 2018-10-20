
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

#include <string>
#include <vector>
#include <stack>
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
    TOK_ASTERISK,
    TOK_SLASH,
    TOK_PERCENT,
    TOK_OPEN_CURLYBRACKET,      // 155
    TOK_CLOSE_CURLYBRACKET,
    TOK_DIALECT,
    TOK_ELSE,
    TOK_LEFT_SHIFT,
    TOK_RIGHT_SHIFT,            // 160
    TOK_READS,
    TOK_WRITES,
    TOK_MODIFIES,
    TOK_STACK,
    TOK_PUSH,                   // 165
    TOK_POP,
    TOK_CF,
    TOK_PF,
    TOK_AF,
    TOK_ZF,                     // 170
    TOK_SF,
    TOK_TF,
    TOK_DF,
    TOK_OF,
    TOK_IOPL,                   // 175
    TOK_NT,
    TOK_MEMORY,
    TOK_SREG,
    TOK_FAR,
    TOK_WAIT,                   // 180
    TOK_LOCK,
    TOK_REP,

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
    "CARET",
    "ASTERISK",
    "SLASH",
    "PERCENT",
    "OPEN_CURLYBRACKET",        // 155
    "CLOSE_CURLYBRACKET",
    "DIALECT",
    "ELSE",
    "LEFT_SHIFT",
    "RIGHT_SHIFT",              // 160
    "READS",
    "WRITES",
    "MODIFIES",
    "STACK",
    "PUSH",                     // 165
    "POP",
    "CF",
    "PF",
    "AF",
    "ZF",                       // 170
    "SF",
    "TF",
    "DF",
    "OF",
    "IOPL",                     // 175
    "NT",
    "MEMORY",
    "SREG",
    "FAR",
    "WAIT",                     // 180
    "LOCK",
    "REP"
};

bool debug_op = false;

bool supported_dialect(const std::string &d) {
    if (d == "intel-x86")
        return true;

    return false;
}

struct tokenstate_t {
    enum tokentype_t    type = TOK_NONE;
    std::string         string;
    union {
        uint64_t        u = 0;
        int64_t         i;
    } intval;
    long double         floatval = 0;

    tokenstate_t &operator=(const std::string &x) {
        type = TOK_STRING;
        string = x;
        return *this;
    }

    tokenstate_t &operator=(const signed long long v) {
        type = TOK_INT;
        intval.i = v;
        return *this;
    }

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

static tokenstate_t operator+(const tokenstate_t &in,const tokenstate_t &ext) {
    tokenstate_t ret;

    if (in.type == ext.type) {
        if (in.type == TOK_UINT) {
            ret.type = in.type;
            ret.intval.u = in.intval.u + ext.intval.u;
        }
        else if (in.type == TOK_INT) {
            ret.type = in.type;
            ret.intval.i = in.intval.i + ext.intval.i;
        }
        else if (in.type == TOK_BOOLEAN) {
            ret.type = in.type;
            ret.intval.u = in.intval.u | ext.intval.u;
        }
        else if (in.type == TOK_FLOAT) {
            ret.type = in.type;
            ret.floatval = in.floatval + ext.floatval;
        }
        else if (in.type == TOK_STRING) {
            ret.type = in.type;
            ret.string = in.string + ext.string;
        }

        return ret;
    }

    tokenstate_t in_copy = in,ext_copy = ext;

    tokenstate_t::promote_for_comparison(in_copy,ext_copy);
    assert(in_copy.type == ext_copy.type);
    return in_copy + ext_copy;
}

static tokenstate_t operator-(const tokenstate_t &in,const tokenstate_t &ext) {
    tokenstate_t ret;

    if (in.type == ext.type) {
        if (in.type == TOK_UINT) {
            ret.type = in.type;
            ret.intval.u = in.intval.u - ext.intval.u;
        }
        else if (in.type == TOK_INT) {
            ret.type = in.type;
            ret.intval.i = in.intval.i - ext.intval.i;
        }
        else if (in.type == TOK_BOOLEAN) {
            ret.type = in.type;
            ret.intval.u = in.intval.u - ext.intval.u;
        }
        else if (in.type == TOK_FLOAT) {
            ret.type = in.type;
            ret.floatval = in.floatval - ext.floatval;
        }

        return ret;
    }

    tokenstate_t in_copy = in,ext_copy = ext;

    tokenstate_t::promote_for_comparison(in_copy,ext_copy);
    assert(in_copy.type == ext_copy.type);
    return in_copy - ext_copy;
}

static tokenstate_t operator*(const tokenstate_t &in,const tokenstate_t &ext) {
    tokenstate_t ret;

    if (in.type == ext.type) {
        if (in.type == TOK_UINT) {
            ret.type = in.type;
            ret.intval.u = in.intval.u * ext.intval.u;
        }
        else if (in.type == TOK_INT) {
            ret.type = in.type;
            ret.intval.i = in.intval.i * ext.intval.i;
        }
        else if (in.type == TOK_BOOLEAN) {
            ret.type = in.type;
            ret.intval.u = in.intval.u * ext.intval.u;
        }
        else if (in.type == TOK_FLOAT) {
            ret.type = in.type;
            ret.floatval = in.floatval * ext.floatval;
        }

        return ret;
    }

    tokenstate_t in_copy = in,ext_copy = ext;

    tokenstate_t::promote_for_comparison(in_copy,ext_copy);
    assert(in_copy.type == ext_copy.type);
    return in_copy * ext_copy;
}

static tokenstate_t operator/(const tokenstate_t &in,const tokenstate_t &ext) {
    tokenstate_t ret;

    if (in.type == ext.type) {
        if (in.type == TOK_UINT) {
            ret.type = in.type;
            ret.intval.u = in.intval.u / ext.intval.u;
        }
        else if (in.type == TOK_INT) {
            ret.type = in.type;
            ret.intval.i = in.intval.i / ext.intval.i;
        }
        else if (in.type == TOK_BOOLEAN) {
            ret.type = in.type;
            ret.intval.u = in.intval.u / ext.intval.u;
        }
        else if (in.type == TOK_FLOAT) {
            ret.type = in.type;
            ret.floatval = in.floatval / ext.floatval;
        }

        return ret;
    }

    tokenstate_t in_copy = in,ext_copy = ext;

    tokenstate_t::promote_for_comparison(in_copy,ext_copy);
    assert(in_copy.type == ext_copy.type);
    return in_copy / ext_copy;
}

static tokenstate_t operator%(const tokenstate_t &in,const tokenstate_t &ext) {
    tokenstate_t ret;

    if (in.type == ext.type) {
        if (in.type == TOK_UINT) {
            ret.type = in.type;
            ret.intval.u = in.intval.u % ext.intval.u;
        }
        else if (in.type == TOK_INT) {
            ret.type = in.type;
            ret.intval.i = in.intval.i % ext.intval.i;
        }
        else if (in.type == TOK_BOOLEAN) {
            ret.type = in.type;
            ret.intval.u = in.intval.u % ext.intval.u;
        }
        else if (in.type == TOK_FLOAT) {
            ret.type = in.type;
            ret.floatval = fmodl(in.floatval,ext.floatval);
        }

        return ret;
    }

    tokenstate_t in_copy = in,ext_copy = ext;

    tokenstate_t::promote_for_comparison(in_copy,ext_copy);
    assert(in_copy.type == ext_copy.type);
    return in_copy % ext_copy;
}

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
        case '*': tok.type = TOK_ASTERISK;      return true;
        case '/': tok.type = TOK_SLASH;         return true;
        case '%': tok.type = TOK_PERCENT;       return true;
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
            else if (chr == '<') {
                tok.type = TOK_LEFT_SHIFT;
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
            else if (chr == '>') {
                tok.type = TOK_RIGHT_SHIFT;
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
        case '-': tok.type = TOK_MINUS;         return true;
        case '{': tok.type = TOK_OPEN_CURLYBRACKET; return true;
        case '}': tok.type = TOK_CLOSE_CURLYBRACKET; return true;
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
        if (tok.string == "DIALECT") {
            tok.type = TOK_DIALECT;
            return true;
        }
        if (tok.string == "ELSE") {
            tok.type = TOK_ELSE;
            return true;
        }
        if (tok.string == "READS") {
            tok.type = TOK_READS;
            return true;
        }
        if (tok.string == "WRITES") {
            tok.type = TOK_WRITES;
            return true;
        }
        if (tok.string == "MODIFIES") {
            tok.type = TOK_MODIFIES;
            return true;
        }
        if (tok.string == "STACK") {
            tok.type = TOK_STACK;
            return true;
        }
        if (tok.string == "PUSH") {
            tok.type = TOK_PUSH;
            return true;
        }
        if (tok.string == "POP") {
            tok.type = TOK_POP;
            return true;
        }
        if (tok.string == "CF") {
            tok.type = TOK_CF;
            return true;
        }
        if (tok.string == "PF") {
            tok.type = TOK_PF;
            return true;
        }
        if (tok.string == "AF") {
            tok.type = TOK_AF;
            return true;
        }
        if (tok.string == "ZF") {
            tok.type = TOK_ZF;
            return true;
        }
        if (tok.string == "SF") {
            tok.type = TOK_SF;
            return true;
        }
        if (tok.string == "TF") {
            tok.type = TOK_TF;
            return true;
        }
        if (tok.string == "IF") {
            tok.type = TOK_IF;
            return true;
        }
        if (tok.string == "DF") {
            tok.type = TOK_DF;
            return true;
        }
        if (tok.string == "OF") {
            tok.type = TOK_OF;
            return true;
        }
        if (tok.string == "IOPL") {
            tok.type = TOK_IOPL;
            return true;
        }
        if (tok.string == "NT") {
            tok.type = TOK_NT;
            return true;
        }
        if (tok.string == "MEMORY") {
            tok.type = TOK_MEMORY;
            return true;
        }
        if (tok.string == "SREG") {
            tok.type = TOK_SREG;
            return true;
        }
        if (tok.string == "FAR") {
            tok.type = TOK_FAR;
            return true;
        }
        if (tok.string == "WAIT") {
            tok.type = TOK_WAIT;
            return true;
        }
        if (tok.string == "LOCK") {
            tok.type = TOK_LOCK;
            return true;
        }
        if (tok.string == "REP") {
            tok.type = TOK_REP;
            return true;
        }
    }

    tok.type = TOK_ERROR;
    return false;
}

std::string march = "";
std::string fpuarch = "";

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
            else if (!strcmp(a,"march")) {
                a = argv[i++];
                if (a == NULL) return 1;
                march = a;
            }
            else if (!strcmp(a,"fpuarch")) {
                a = argv[i++];
                if (a == NULL) return 1;
                fpuarch = a;
            }
            else if (!strcmp(a,"dop")) {
                debug_op = true;
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

class SingleByteSpec {
public:
    unsigned long long          intval = 0;
    unsigned int                var_assign = 0;
    unsigned int                meaning = 0;            // if TOK_IMMEDIATE then size() == 0 and it's an immediate byte
    unsigned int                immediate_type = 0;
    unsigned int                memseg_type = 0;
    unsigned int                reg_type = 0;           // if TOK_REG
    unsigned int                rm_type = 0;            // if TOK_RM
    unsigned int                memory_type = 0;        // if TOK_MEMORY, var_expr says what to write
    std::vector<unsigned int>   flags;                  // if TOK_FLAGS
    std::vector<tokenstate_t>   var_expr;
public:
    std::string                 to_string(void);
};

class ByteSpec : public SingleByteSpec, public std::vector<uint8_t> {
public:
    std::string                 to_string(void);
};

class OpcodeSpec {
public:
    std::vector<ByteSpec>       bytes;                  // opcode byte sequence
    unsigned int                type = 0;               // token type (PREFIX, OPCODE, etc)
    std::string                 description;
    std::string                 comment;
    std::string                 name;
    SingleByteSpec              destination;            // destination (if writes) token
    std::vector<SingleByteSpec> param;                  // parameter (read only) tokens. variables are possible (TOK_D)
    std::vector<SingleByteSpec> reads;
    std::vector<SingleByteSpec> writes;
    std::vector<SingleByteSpec> modifies;
    std::vector<SingleByteSpec> stack_ops;              // push or pop
    unsigned int                stack_op_dir = 0;       // TOK_PUSH or TOK_POP
    unsigned int                prefix_seg_assign = 0;  // token segment override assignment (PREFIX)
    unsigned char               reg_constraint = 0;     // bitmask of valid reg values
    signed char                 mod3 = 0;               // -3 means mod!=3   3 means mod==3
    unsigned int                rep_condition = 0;      // 0 or TOK_Z
    bool                        wait = false;
    bool                        lock = false;
    bool                        rep_condition_negate = false;
public:
    void                        add_reg_constraint(const unsigned char reg);
    std::string                 to_string(void);
};

void OpcodeSpec::add_reg_constraint(const unsigned char reg) {
    reg_constraint |= 1u << reg;
}

std::string SingleByteSpec::to_string(void) {
    std::string res;

    if (var_assign != TOK_NONE) {
        if (!res.empty()) res += ",";
        res += "varassign=";
        res += tokentype_str[var_assign];
    }
    if (meaning != TOK_NONE) {
        if (!res.empty()) res += ",";
        res += "meaning=";
        res += tokentype_str[meaning];
        if (meaning == TOK_UINT) {
            char tmp[128];
            sprintf(tmp,"=%llu",(unsigned long long)intval);
            res += tmp;
        }
    }
    if (immediate_type != TOK_NONE) {
        if (!res.empty()) res += ",";
        res += "immediate=";
        res += tokentype_str[immediate_type];
    }
    if (reg_type != TOK_NONE) {
        if (!res.empty()) res += ",";
        res += "regtype=";
        res += tokentype_str[reg_type];
    }
    if (rm_type != TOK_NONE) {
        if (!res.empty()) res += ",";
        res += "rmtype=";
        res += tokentype_str[rm_type];
    }
    if (memseg_type != TOK_NONE) {
        if (!res.empty()) res += ",";
        res += "memsegtype=";
        res += tokentype_str[memseg_type];
    }
    if (memseg_type != TOK_NONE) {
        if (!res.empty()) res += ",";
        res += "memtype=";
        res += tokentype_str[memory_type];
    }
    if (!flags.empty()) {
        if (!res.empty()) res += ",";
        res += "flags=[";
        for (auto i=flags.begin();i!=flags.end();) {
            res += tokentype_str[*i];
            i++;
            if (i != flags.end()) res += ",";
        }
        res += "]";
    }
    if (!var_expr.empty()) {
        if (!res.empty()) res += ",";
        res += "var_expr=[";
        for (auto i=var_expr.begin();i!=var_expr.end();) {
            res += std::string((*i).type_str()) + "->" + (*i).to_string();
            i++;
            if (i != var_expr.end()) res += ",";
        }
        res += "]";
    }

    return res;
}

std::string ByteSpec::to_string(void) {
    std::string res;
    char tmp[64];

    if (size() != 0) {
        res += "[";
        for (auto i=begin();i!=end();) {
            sprintf(tmp,"0x%02x",*i);
            res += tmp;
            i++;
            if (i != end()) res += " ";
        }
        res += "]";
    }

    {
        std::string subres = SingleByteSpec::to_string();

        if (!subres.empty()) {
            if (!res.empty()) res += ",";
            res += subres;
        }
    }

    return res;
}

std::string OpcodeSpec::to_string(void) {
    std::string res;

    res += "bytes=(";
    for (auto i=bytes.begin();i!=bytes.end();) {
        res += (*i).to_string();
        i++;
        if (i != bytes.end()) res += " ";
    }
    res += ")";

    if (mod3 != 0) {
        if (!res.empty()) res += ",";
        if (mod3 == 3)
            res += "mod==3";
        else if (mod3 == -3)
            res += "mod!=3";
        else
            abort();
    }

    if (wait) {
        if (!res.empty()) res += ",";
        res += "wait=1";
    }

    if (lock) {
        if (!res.empty()) res += ",";
        res += "lock=1";
    }

    if (rep_condition != 0) {
        if (!res.empty()) res += ",";
        res += "rep=";
        if (rep_condition_negate) res += "!";
        res += tokentype_str[rep_condition];
    }

    if (reg_constraint != 0) {
        unsigned int c = 0;

        if (!res.empty()) res += ",";
        res += "regconstraint=[";
        for (unsigned int b=0;b < 16;b++) {
            if (reg_constraint & (1u << b)) {
                if (c != 0) res += ",";

                char tmp[16];
                sprintf(tmp,"%u",b);
                res += tmp;
                c++;
            }
        }
        res += "]";
    }

    {
        std::string subres = destination.to_string();

        if (!subres.empty()) {
            if (!res.empty()) res += ",";
            res += "dest=[";
            res += subres;
            res += "]";
        }
    }

    if (param.size() != 0) {
        if (!res.empty()) res += ",";
        res += "param=[";
        for (auto i=param.begin();i!=param.end();) {
            res += (*i).to_string();
            i++;
            if (i!=param.end()) res += " ";
        }
        res += "]";
    }

    if (reads.size() != 0) {
        if (!res.empty()) res += ",";
        res += "reads=[";
        for (auto i=reads.begin();i!=reads.end();) {
            res += (*i).to_string();
            i++;
            if (i!=reads.end()) res += " ";
        }
        res += "]";
    }

    if (writes.size() != 0) {
        if (!res.empty()) res += ",";
        res += "writes=[";
        for (auto i=writes.begin();i!=writes.end();) {
            res += (*i).to_string();
            i++;
            if (i!=writes.end()) res += " ";
        }
        res += "]";
    }

    if (modifies.size() != 0) {
        if (!res.empty()) res += ",";
        res += "modifies=[";
        for (auto i=modifies.begin();i!=modifies.end();) {
            res += (*i).to_string();
            i++;
            if (i!=modifies.end()) res += " ";
        }
        res += "]";
    }

    if (stack_ops.size() != 0) {
        if (!res.empty()) res += ",";
        res += "stack_ops(";
        if (stack_op_dir == TOK_PUSH)
            res += "push";
        else if (stack_op_dir == TOK_POP)
            res += "pop";
        res += ")=[";
        for (auto i=stack_ops.begin();i!=stack_ops.end();) {
            res += (*i).to_string();
            i++;
            if (i!=stack_ops.end()) res += " ";
        }
        res += "]";
    }

    return res;
}

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
    void clear(void) {
        std::vector<tokenstate_t>::clear();
        rewind();
    }
    void rewind(void) {
        read = 0;
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
    else if (t.type == TOK_OPEN_PARENS) {
        tokenstate_t subtoken;
        tokenlist subtokens;
        int parens = 1;

        do {
            t = tokens.next();
            if (t.type == TOK_ERROR || t.type == TOK_NONE)
                return false;
            else if (t.type == TOK_OPEN_PARENS)
                parens++;
            else if (t.type == TOK_CLOSE_PARENS) {
                if (parens-- <= 1)
                    break;
            }

            subtokens.push_back(t);
        } while (1);

        if (!eval_if_condition(subtoken,subtokens))
            return false;

        result = subtoken;
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

        if (!eval_if_condition_block(tmp,tokens))
            return false;

        result.type = TOK_BOOLEAN;
        result.intval.u = !tmp.to_bool();
        return true;
    }
    else if (t.type == TOK_NEGATE) {
        /* not ... */
        tokenstate_t tmp;

        if (!eval_if_condition_block(tmp,tokens))
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

        if (!eval_if_condition_block(tmp,tokens))
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
    else if (tokens.peek().type == TOK_LEFT_SHIFT) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition_block(res2,tokens))
            return false;

        unsigned long long res = result.to_intval_u() << res2.to_intval_u();

        result.intval.u = res;
        if (result.type == TOK_INT || res2.type == TOK_INT)
            result.type = TOK_INT;
        else if (result.type == TOK_FLOAT || res2.type == TOK_FLOAT)
            result.type = TOK_INT;
        else
            result.type = TOK_UINT;
    }
    else if (tokens.peek().type == TOK_RIGHT_SHIFT) {
        tokens.discard();

        tokenstate_t res2;

        if (!eval_if_condition_block(res2,tokens))
            return false;

        unsigned long long res = result.to_intval_u() >> res2.to_intval_u();

        result.intval.u = res;
        if (result.type == TOK_INT || res2.type == TOK_INT)
            result.type = TOK_INT;
        else if (result.type == TOK_FLOAT || res2.type == TOK_FLOAT)
            result.type = TOK_INT;
        else
            result.type = TOK_UINT;
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
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            bool expr_result = (result.to_bool() && res2.to_bool());

            result.type = TOK_BOOLEAN;
            result.intval.u = expr_result ? 1ull : 0ull;
        } while (tokens.peek().type == TOK_AND);
    }
    else if (tokens.peek().type == TOK_OR) {
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            bool expr_result = (result.to_bool() || res2.to_bool());

            result.type = TOK_BOOLEAN;
            result.intval.u = expr_result ? 1ull : 0ull;
        } while (tokens.peek().type == TOK_OR);
    }
    else if (tokens.peek().type == TOK_XOR) {
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            bool expr_result = ((result.to_bool()?1:0) ^ (res2.to_bool()?1:0));

            result.type = TOK_BOOLEAN;
            result.intval.u = expr_result ? 1ull : 0ull;
        } while (tokens.peek().type == TOK_XOR);
    }
    else if (tokens.peek().type == TOK_AMPERSAND) {
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            unsigned long long res = result.to_intval_u() & res2.to_intval_u();

            result.intval.u = res;
            if (result.type == TOK_INT || res2.type == TOK_INT)
                result.type = TOK_INT;
            else if (result.type == TOK_FLOAT || res2.type == TOK_FLOAT)
                result.type = TOK_INT;
            else
                result.type = TOK_UINT;
        } while (tokens.peek().type == TOK_AMPERSAND);
    }
    else if (tokens.peek().type == TOK_PIPE) {
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            unsigned long long res = result.to_intval_u() | res2.to_intval_u();

            result.intval.u = res;
            if (result.type == TOK_INT || res2.type == TOK_INT)
                result.type = TOK_INT;
            else if (result.type == TOK_FLOAT || res2.type == TOK_FLOAT)
                result.type = TOK_INT;
            else
                result.type = TOK_UINT;
        } while (tokens.peek().type == TOK_PIPE);
    }
    else if (tokens.peek().type == TOK_CARET) {
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            unsigned long long res = result.to_intval_u() ^ res2.to_intval_u();

            result.intval.u = res;
            if (result.type == TOK_INT || res2.type == TOK_INT)
                result.type = TOK_INT;
            else if (result.type == TOK_FLOAT || res2.type == TOK_FLOAT)
                result.type = TOK_INT;
            else
                result.type = TOK_UINT;
        } while (tokens.peek().type == TOK_CARET);
    }
    else if (tokens.peek().type == TOK_PLUS) {
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            result = result + res2;
        } while (tokens.peek().type == TOK_PLUS);
    }
    else if (tokens.peek().type == TOK_MINUS) {
        do {
            tokens.discard();

            tokenstate_t res2;

            /* eval pair by pair forwards, so that expressions like 8-4-2-1 evaluate in order and produce (((8-4)-2)-1) = 1
             * instead of recursion that would produce (8-(4-(2-1))) = 5 */
            if (!eval_if_condition_block(res2,tokens))
                return false;

            result = result - res2;
        } while (tokens.peek().type == TOK_MINUS);
    }
    else if (tokens.peek().type == TOK_ASTERISK) {
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            result = result * res2;
        } while (tokens.peek().type == TOK_ASTERISK);
    }
    else if (tokens.peek().type == TOK_SLASH) {
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            result = result / res2;
        } while (tokens.peek().type == TOK_SLASH);
    }
    else if (tokens.peek().type == TOK_PERCENT) {
        do {
            tokens.discard();

            tokenstate_t res2;

            if (!eval_if_condition_block(res2,tokens))
                return false;

            result = result % res2;
        } while (tokens.peek().type == TOK_PERCENT);
    }
 
    return true;
}

bool is_valid_immediate_assign_var(const unsigned int c) {
    switch (c) {
        case TOK_A: return true;
        case TOK_B: return true;
        case TOK_C: return true;
        case TOK_D: return true;
        case TOK_E: return true;
        case TOK_F: return true;
        case TOK_G: return true;
        case TOK_H: return true;
        case TOK_I: return true;
        case TOK_J: return true;
        case TOK_K: return true;
        case TOK_L: return true;
        case TOK_M: return true;
        case TOK_N: return true;
        case TOK_O: return true;
        case TOK_P: return true;
        case TOK_Q: return true;
        case TOK_R: return true;
        case TOK_S: return true;
        case TOK_T: return true;
        case TOK_U: return true;
        case TOK_V: return true;
        case TOK_W: return true;
        case TOK_X: return true;
        case TOK_Y: return true;
        case TOK_Z: return true;
        case TOK_REG: return true;
        default: break;
    };

    return false;
}

bool parse_code_flags_spec(std::vector<unsigned int> &flags,tokenlist &tokens) {
    /* caller ate TOK_FLAGS */
    if (tokens.next().type != TOK_OPEN_PARENS) return false;

    do {
        auto &n = tokens.next();

        if (n.type == TOK_CLOSE_PARENS) break;

        switch (n.type) {
            case TOK_ALL:
            case TOK_CF:
            case TOK_PF:
            case TOK_AF:
            case TOK_ZF:
            case TOK_SF:
            case TOK_TF:
            case TOK_IF:
            case TOK_DF:
            case TOK_OF:
            case TOK_IOPL:
            case TOK_NT:
                flags.push_back(n.type);
                break;
            default:
                return false;
        };

        if (tokens.peek().type == TOK_COMMA) {
            tokens.discard();
        }
        else if (tokens.peek().type == TOK_CLOSE_PARENS) {
            tokens.discard();
            break;
        }
        else {
            break;
        }
    } while (1);

    return true;
}

bool parse_far_mem_spec(SingleByteSpec &bs,tokenlist &tokens) {
    // caller already ate TOK_FAR TOK_MEMORY

    /* caller ate TOK_FLAGS */
    if (tokens.next().type != TOK_OPEN_PARENS) return false;

    // first param, is type
    auto &n = tokens.next();
    if (!valid_immediate_size_token(n.type)) return false;
    bs.memory_type = n.type;

    // second param is segment token
    if (tokens.peek().type != TOK_COMMA) return false;
    tokens.discard();

    n = tokens.next();
    bs.memseg_type = n.type;

    // third param is offset expression/token
    if (tokens.peek().type != TOK_COMMA) return false;
    tokens.discard();

    if (tokens.peek().type == TOK_OPEN_PARENS) {
        tokens.discard();
        int parens = 1;

        do {
            auto &next = tokens.next();

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

            bs.var_expr.push_back(next);
        } while (1);
    }
    else {
        auto &nn = tokens.next();
        if (nn.type == TOK_CLOSE_PARENS) return false;
        bs.var_expr.push_back(nn);
    }

    /* caller ate TOK_FLAGS */
    if (tokens.next().type != TOK_CLOSE_PARENS) return false;

    return true;
}

bool parse_mem_spec(SingleByteSpec &bs,tokenlist &tokens) {
    // caller already ate TOK_MEMORY

    /* caller ate TOK_FLAGS */
    if (tokens.next().type != TOK_OPEN_PARENS) return false;

    // first param, is type
    auto &n = tokens.next();
    if (!valid_immediate_size_token(n.type)) return false;
    bs.memory_type = n.type;

    // optionally, a comma, and then var_expr describing the address
    if (tokens.peek().type == TOK_COMMA) {
        tokens.discard();

        if (tokens.peek().type == TOK_OPEN_PARENS) {
            tokens.discard();
            int parens = 1;

            do {
                auto &next = tokens.next();

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

                bs.var_expr.push_back(next);
            } while (1);
        }
        else {
            auto &nn = tokens.next();
            if (nn.type == TOK_CLOSE_PARENS) return false;
            bs.var_expr.push_back(nn);
        }
    }

    /* caller ate TOK_FLAGS */
    if (tokens.next().type != TOK_CLOSE_PARENS) return false;

    return true;
}

bool parse_sbl_list(std::vector<SingleByteSpec> &sbl,tokenlist &tokens) {
    while (!tokens.eof()) {
        SingleByteSpec bs;

        auto &n = tokens.next();
        bs.meaning = n.type;

        if (bs.meaning == TOK_IMMEDIATE) {
            if ((bs.immediate_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid immediate spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_UINT) {
            bs.intval = n.intval.u;
        }
        else if (bs.meaning == TOK_FLAGS) {
            if (!parse_code_flags_spec(bs.flags,/*&*/tokens)) {
                fprintf(stderr,"Invalid flags spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_REG || bs.meaning == TOK_SREG) {
            if ((bs.reg_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_RM) {
            if ((bs.rm_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_MEMORY) {
            if (!parse_mem_spec(bs,/*&*/tokens)) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_FAR) {
            n = tokens.next();
            if (n.type == TOK_MEMORY) {
                bs.meaning = n.type;
                if (!parse_far_mem_spec(bs,/*&*/tokens)) {
                    fprintf(stderr,"Invalid reg spec\n");
                    return false;
                }
            }
            else {
                fprintf(stderr,"far... unexpected tokens\n");
                return false;
            }
        }

        sbl.push_back(bs);

        if (tokens.peek().type == TOK_COMMA) {
            tokens.discard();
        }
        else {
            break;
        }
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

    if (tokens.eof())
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

    /* wait=val */
    if (tokens.peek(0).type == TOK_WAIT && tokens.peek(1).type == TOK_EQUAL) {
        tokens.discard(2);

        if (spec.type != TOK_PREFIX) {
            fprintf(stderr,"Segment override specifications only allowed for prefixes\n");
            return false;
        }

        auto &n = tokens.next();
        if (n.type == TOK_UINT) {
            spec.wait = n.intval.u > 0ull;
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

    /* lock=val */
    if (tokens.peek(0).type == TOK_LOCK && tokens.peek(1).type == TOK_EQUAL) {
        tokens.discard(2);

        if (spec.type != TOK_PREFIX) {
            fprintf(stderr,"Segment override specifications only allowed for prefixes\n");
            return false;
        }

        auto &n = tokens.next();
        if (n.type == TOK_UINT) {
            spec.lock = n.intval.u > 0ull;
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

    /* rep=z or rep=!z */
    if (tokens.peek(0).type == TOK_REP && tokens.peek(1).type == TOK_EQUAL) {
        tokens.discard(2);

        if (spec.type != TOK_PREFIX) {
            fprintf(stderr,"Segment override specifications only allowed for prefixes\n");
            return false;
        }

        auto &n = tokens.next();
        if (n.type == TOK_NOT) {
            spec.rep_condition_negate = true;
            n = tokens.next();
        }

        if (n.type == TOK_Z) {
            spec.rep_condition = n.type;
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

    /* reads ... */
    if (tokens.peek().type == TOK_READS) {
        tokens.discard();

        if (!parse_sbl_list(spec.reads,tokens))
            return false;

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        return true;
    }

    /* writes ... */
    if (tokens.peek().type == TOK_WRITES) {
        tokens.discard();

        if (!parse_sbl_list(spec.writes,tokens))
            return false;

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        return true;
    }

    /* modifies ... */
    if (tokens.peek().type == TOK_MODIFIES) {
        tokens.discard();

        if (!parse_sbl_list(spec.modifies,tokens))
            return false;

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        return true;
    }

    /* stack push ... */
    /* stack pop ... */
    if (tokens.peek().type == TOK_STACK) {
        tokens.discard();

        if (tokens.peek().type == TOK_PUSH) {
            tokens.discard();

            if (spec.stack_op_dir != TOK_NONE && spec.stack_op_dir != TOK_PUSH) {
                fprintf(stderr,"Stack ops can be push or pop, not both\n");
                return false;
            }

            spec.stack_op_dir = TOK_PUSH;
        }
        else if (tokens.peek().type == TOK_POP) {
            tokens.discard();

            if (spec.stack_op_dir != TOK_NONE && spec.stack_op_dir != TOK_POP) {
                fprintf(stderr,"Stack ops can be push or pop, not both\n");
                return false;
            }

            spec.stack_op_dir = TOK_POP;
        }
        else {
            return false;
        }

        if (!parse_sbl_list(spec.stack_ops,tokens))
            return false;

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        return true;
    }

    /* dest=register/mem/etc */
    if (tokens.peek(0).type == TOK_DEST && tokens.peek(1).type == TOK_EQUAL) {
        tokens.discard(2);

        if (spec.destination.meaning != TOK_NONE) {
            fprintf(stderr,"Destination already specified\n");
            return false;
        }

        SingleByteSpec bs;

        auto &n = tokens.next();
        bs.meaning = n.type;

        if (bs.meaning == TOK_IMMEDIATE) {
            /* come on now.... in what way is it valid to assign to an immediate? */
            fprintf(stderr,"Immediate in dest not supported\n");
            return false;
        }
        else if (bs.meaning == TOK_UINT) {
            bs.intval = n.intval.u;
        }
        else if (bs.meaning == TOK_REG || bs.meaning == TOK_SREG) {
            if ((bs.reg_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_RM) {
            if ((bs.rm_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_MEMORY) {
            if (!parse_mem_spec(bs,/*&*/tokens)) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_FAR) {
            n = tokens.next();
            if (n.type == TOK_MEMORY) {
                bs.meaning = n.type;
                if (!parse_far_mem_spec(bs,/*&*/tokens)) {
                    fprintf(stderr,"Invalid reg spec\n");
                    return false;
                }
            }
            else {
                fprintf(stderr,"far... unexpected tokens\n");
                return false;
            }
        }

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        spec.destination = std::move(bs);
        return true;
    }

    /* param=register/mem/etc */
    if (tokens.peek(0).type == TOK_PARAM && tokens.peek(1).type == TOK_EQUAL) {
        size_t i = (size_t)0;
        tokens.discard(2);

        SingleByteSpec bs;

        while (spec.param.size() <= i)
            spec.param.push_back(bs);

        if (spec.param[i].meaning != TOK_NONE) {
            fprintf(stderr,"param(%zu) already specified\n",i);
            return false;
        }

        auto &n = tokens.next();
        bs.meaning = n.type;

        if (bs.meaning == TOK_IMMEDIATE) {
            fprintf(stderr,"Immediate in param not supported, use var assignment in code or mod/reg/rm\n");
            return false;
        }
        else if (bs.meaning == TOK_UINT) {
            bs.intval = n.intval.u;
        }
        else if (bs.meaning == TOK_REG || bs.meaning == TOK_SREG) {
            if ((bs.reg_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_RM) {
            if ((bs.rm_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_MEMORY) {
            if (!parse_mem_spec(bs,/*&*/tokens)) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_FAR) {
            n = tokens.next();
            if (n.type == TOK_MEMORY) {
                bs.meaning = n.type;
                if (!parse_far_mem_spec(bs,/*&*/tokens)) {
                    fprintf(stderr,"Invalid reg spec\n");
                    return false;
                }
            }
            else {
                fprintf(stderr,"far... unexpected tokens\n");
                return false;
            }
        }

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        spec.param[i] = std::move(bs);
        return true;
    }

    /* param(i)=register/mem/etc */
    if (tokens.peek(0).type == TOK_PARAM && tokens.peek(1).type == TOK_OPEN_PARENS &&
        tokens.peek(2).type == TOK_UINT && tokens.peek(3).type == TOK_CLOSE_PARENS &&
        tokens.peek(4).type == TOK_EQUAL) {
        if (tokens.peek(3).intval.u > 8) {
            fprintf(stderr,"Out of range parens\n");
            return false;
        }
        size_t i = (size_t)tokens.peek(3).intval.u;
        tokens.discard(5);

        SingleByteSpec bs;

        while (spec.param.size() <= i)
            spec.param.push_back(bs);

        if (spec.param[i].meaning != TOK_NONE) {
            fprintf(stderr,"param(%zu) already specified\n",i);
            return false;
        }

        auto &n = tokens.next();
        bs.meaning = n.type;

        if (bs.meaning == TOK_IMMEDIATE) {
            fprintf(stderr,"Immediate in param not supported, use var assignment in code or mod/reg/rm\n");
            return false;
        }
        else if (bs.meaning == TOK_UINT) {
            bs.intval = n.intval.u;
        }
        else if (bs.meaning == TOK_REG || bs.meaning == TOK_SREG) {
            if ((bs.reg_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_RM) {
            if ((bs.rm_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_MEMORY) {
            if (!parse_mem_spec(bs,/*&*/tokens)) {
                fprintf(stderr,"Invalid reg spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_FAR) {
            n = tokens.next();
            if (n.type == TOK_MEMORY) {
                bs.meaning = n.type;
                if (!parse_far_mem_spec(bs,/*&*/tokens)) {
                    fprintf(stderr,"Invalid reg spec\n");
                    return false;
                }
            }
            else {
                fprintf(stderr,"far... unexpected tokens\n");
                return false;
            }
        }

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            return false;
        }

        spec.param[i] = std::move(bs);
        return true;
    }

    /* code (byte or byte ranges) [mod/reg/rm] [immediate(...)] */
    if (tokens.peek(0).type == TOK_CODE) {
        tokens.discard(1);

        if (spec.bytes.size() != 0) {
            fprintf(stderr,"The same opcode cannot have multiple code specs\n");
            return false;
        }

        while (!tokens.eof()) {
            ByteSpec bs;

            if (is_valid_immediate_assign_var(tokens.peek(0).type) && tokens.peek(1).type == TOK_EQUAL) {
                bs.var_assign = tokens.peek().type;
                tokens.discard(2);

                /* allow var=(...) expressions */
                if (tokens.peek().type == TOK_OPEN_PARENS) {
                    tokens.discard();
                    int parens = 1;
                    do {
                        auto &n = tokens.next();

                        if (n.type == TOK_NONE || n.type == TOK_ERROR)
                            return false;
                        else if (n.type == TOK_CLOSE_PARENS) {
                            if (parens-- <= 1)
                                break;
                        }
                        else if (n.type == TOK_OPEN_PARENS)
                            parens++;

                        bs.var_expr.push_back(n);
                    } while (1);

                    spec.bytes.push_back(bs);
                    continue;
                }
            }

            if (tokens.peek().type == TOK_UINT) {
                do {
                    auto &next = tokens.next();

                    /* byte range (0xA0-0xA3) */
                    if (tokens.peek().type == TOK_MINUS) {
                        unsigned int start = next.intval.u;
                        tokens.discard(); // TOK_MINUS
                        auto &next2 = tokens.next();
                        if (next2.type != TOK_UINT) {
                            fprintf(stderr,"Unexpected minus sign\n");
                            return false;
                        }
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
                } while (1);
            }
            else if (tokens.peek().type == TOK_MRM) {
                tokens.discard();

                bs.meaning = TOK_MRM;
                spec.bytes.push_back(bs);
            }
            else if (tokens.peek(0).type == TOK_REG && tokens.peek(1).type == TOK_OPEN_PARENS) {
                tokens.discard(2);

                do {
                    auto &n = tokens.next();

                    if (n.type == TOK_UINT) {
                        if (n.intval.u < 16) {
                            spec.add_reg_constraint((unsigned char)n.intval.u);
                        }
                        else {
                            fprintf(stderr,"reg() spec out of range\n");
                            return false;
                        }
                    }
                    else if (n.type == TOK_CLOSE_PARENS) {
                        break;
                    }
                    else if (n.type == TOK_COMMA) {
                        continue;
                    }
                    else {
                        fprintf(stderr,"reg() spec unexpected token\n");
                        return false;
                    }

                    n = tokens.next();
                    if (n.type == TOK_COMMA) {
                        continue;
                    }
                    else if (n.type == TOK_CLOSE_PARENS) {
                        break;
                    }
                    else {
                        fprintf(stderr,"reg() spec expected comma\n");
                        return false;
                    }
                } while (1);
            }
            else if (tokens.peek(0).type == TOK_MOD && tokens.peek(1).type == TOK_OPEN_PARENS) {
                tokens.discard(2);

                /* !3 (to say the mod field cannot be 3) or 3 (to say the mod field must be 3).
                 * this is used to indicate an instruction is not valid if the instruction encodes
                 * a register to register operation (i.e. LEA). */
                if (tokens.peek(0).type == TOK_UINT && tokens.peek(0).intval.u == 3) {
                    tokens.discard(1);
                    spec.mod3 = 3;
                }
                else if (tokens.peek(0).type == TOK_NOT && tokens.peek(1).type == TOK_UINT && tokens.peek(1).intval.u == 3) {
                    tokens.discard(2);
                    spec.mod3 = -3;
                }
                else {
                    fprintf(stderr,"mod() unexpected tokens\n");
                    return false;
                }

                if (tokens.peek().type != TOK_CLOSE_PARENS) {
                    fprintf(stderr,"mod() no closing parens\n");
                    return false;
                }
                tokens.discard();
            }
            else if (tokens.peek().type == TOK_IMMEDIATE) {
                tokens.discard();

                bs.meaning = TOK_IMMEDIATE;
                if ((bs.immediate_type=parse_code_immediate_spec(/*&*/tokens)) == TOK_NONE) {
                    fprintf(stderr,"Invalid immediate spec\n");
                    return false;
                }

                spec.bytes.push_back(bs);
            }
            else {
                break;
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
    if (tokens.eof())
        return true; /* not an error */

    /* prefix "name" (...) (...) (...) */
    /* opcode "name" (...) (...) (...) */
    if ((tokens.peek(0).type == TOK_PREFIX || tokens.peek(0).type == TOK_OPCODE) && tokens.peek(1).type == TOK_STRING) {
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
}

bool do_opcode_spec(tokenlist &tokens) {
    OpcodeSpec spec;

    if (!read_opcode_spec(/*&*/spec,/*&*/tokens))
        return false;
    if (spec.type == TOK_NONE)
        return true;

    if (debug_op)
        fprintf(stderr,"%s %s: %s\n",tokentype_str[spec.type],spec.name.c_str(),spec.to_string().c_str());

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

        if (tokens.peek(0).type == TOK_CLOSE_PARENS) {
            tokens.discard();
            break;
        }
        else if (tokens.peek(0).type == TOK_COMMA) {
            tokens.discard();
            // ignore
        }
        else {
            if (!eval_if_condition(/*&*/result,/*&*/tokens)) {
                fprintf(stderr,"'If' in format condition error\n");
                return false;
            }

            msg += result.to_string();

            // followed by a comma, or else
            if (tokens.peek(0).type == TOK_COMMA) {
                tokens.discard();
                // good!
            }
            else if (tokens.peek(0).type == TOK_CLOSE_PARENS) {
                tokens.discard();
                break;
            }
            else {
                fprintf(stderr,"Format parsing, unexpected token\n");
                return false;
            }
        }
    } while (1);

    return true;
}

bool process_if_statement(tokenlist &tokens,bool suppress=false);

bool process_block(tokenlist &tokens);
bool read_opcode_token_block(tokenlist &tokens);

bool process_if_block(const bool result_bool,tokenlist &tokens) {
    while (true) {
        if (!read_opcode_token_block(tokens)) // will clear and refill tokens
            return false;

        if (tokens.peek(0).type == TOK_CLOSE_CURLYBRACKET && tokens.peek(1).type == TOK_IF) {
            tokens.discard(2);
            if (!tokens.eof()) {
                fprintf(stderr,"IF end unexpected tokens\n");
                return false;
            }

            break;
        }

        if (result_bool) {
            if (!process_block(tokens))
                return false;
        }
        else if (tokens.peek().type == TOK_IF) { /* if condition if condition (nested) */
            tokens.discard();
            if (!process_if_statement(tokens,/*suppress=*/true))
                return false;
        }
    }

    return true;
}

bool process_if_statement(tokenlist &tokens,bool suppress) {
    tokenstate_t result;

    /* TOK_IF has been consumed by caller */

    if (!eval_if_condition(/*&*/result,/*&*/tokens)) {
        fprintf(stderr,"'If' condition error\n");
        return false;
    }

    bool result_bool = result.to_bool() && !suppress;

    if (tokens.peek().type == TOK_OPEN_CURLYBRACKET) {
        tokens.discard();

        if (!tokens.eof()) {
            fprintf(stderr,"if { unexpected token '%s'\n",tokens.peek().type_str());
            return false;
        }

        if (!process_if_block(result_bool,tokens))
            return false;
    }
    else {
        if (tokens.peek().type == TOK_IF) {
            /* supporting IF condition IF condition .... ELSE condition ELSE condition would be way too complicated to support
             * especially if the ELSE is optional at any level.
             *
             * if you need nested IFs use the IF condition { ..... } IF form; */
            fprintf(stderr,"nested IFs in a single block not allowed\n");
            return false;
        }

        if (result_bool && !suppress) {
            if (!process_block(tokens))
                return false;
        }
    }

    /* read the next block.
     * if it's ELSE, then evaluate if the condition is NOT true.
     * if it's not ELSE, then it's the next statement and process as normal. */
    if (!read_opcode_token_block(tokens))
        return false;

    if (tokens.peek().type == TOK_ELSE) {
        tokens.discard();

        if (tokens.peek().type == TOK_OPEN_CURLYBRACKET) {
            tokens.discard();

            if (!tokens.eof()) {
                fprintf(stderr,"if { unexpected token '%s'\n",tokens.peek().type_str());
                return false;
            }

            if (!process_if_block(!result_bool && !suppress,tokens))
                return false;
        }
        else {
            if (!result_bool && !suppress) {
                if (!process_block(tokens))
                    return false;
            }
            else if (tokens.peek().type == TOK_IF) { /* if condition if condition (nested) */
                tokens.discard();
                if (!process_if_statement(tokens,/*suppress=*/true))
                    return false;
            }
        }
    }
    else {
        if (!suppress) {
            if (!process_block(tokens))
                return false;
        }
        else if (tokens.peek().type == TOK_IF) { /* if condition if condition (nested) */
            tokens.discard();
            if (!process_if_statement(tokens,suppress))
                return false;
        }
    }

    return true;
}

bool process_block(tokenlist &tokens) {
    if (tokens.peek(0).type == TOK_IF) {
        tokens.discard(); // discard IF

        if (!process_if_statement(tokens))
            return false;

        return true;
    }

    /* dialect "string" */
    if (tokens.peek(0).type == TOK_DIALECT && tokens.peek(1).type == TOK_STRING) {
        std::string dialect = tokens.peek(1).string;
        tokens.discard(2);

        if (!tokens.eof()) {
            fprintf(stderr,"Desc unexpected tokens\n");
            return false;
        }

        if (!supported_dialect(dialect)) {
            fprintf(stderr,"This compiler does not support dialect '%s'\n",dialect.c_str());
            return false;
        }

        return true;
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

bool read_opcode_token_block(tokenlist &tokens) {
    tokens.clear();

    do {
        tokenstate_t tok;

        if (!toke(/*&*/tok)) {
            if (tok.type == TOK_ERROR)
                goto token_error;
            if (tokens.eof())
                return false;

            goto unexpected_end;
        }

        if (tok.type == TOK_SEMICOLON)
            return true;

        tokens.push_back(std::move(tok));
    } while(1);

token_error:
    fprintf(stderr,"Token error\n");
    return false;
unexpected_end:
    fprintf(stderr,"Unexpected end of opcode\n");
    return false;
}

bool read_opcode_block(void) {
    do {
        tokenlist tokens;

        if (!read_opcode_token_block(/*&*/tokens))
            return false;

        if (!process_block(/*&*/tokens))
            return false;
    } while (1);

    return true;
}

int main(int argc,char **argv) {
    /* setup predefined values */
    defines["dialect"] = "intel-x86";

    if (parse_argv(argc,argv))
        return 1;

    if (march.empty())
        march = "8086";

    if (march == "8086") {
        if (fpuarch.empty())
            fpuarch = "8087";

        defines["cpulevel"] = 86;
    }
    else if (march == "80186") {
        if (fpuarch.empty())
            fpuarch = "8087";

        defines["cpulevel"] = 186;
    }
    else if (march == "80286" || march == "286") {
        if (fpuarch.empty())
            fpuarch = "80287";

        defines["cpulevel"] = 286;
    }
    else if (march == "80386" || march == "386") {
        if (fpuarch.empty())
            fpuarch = "80387";

        defines["cpulevel"] = 386;
    }
    else if (march == "80486" || march == "486") {
        if (fpuarch.empty())
            fpuarch = "80387";

        defines["cpulevel"] = 486;
    }
    else if (march == "80586" || march == "586") {
        if (fpuarch.empty())
            fpuarch = "80587";

        defines["cpulevel"] = 586;
    }
    else if (march == "pentium") {
        if (fpuarch.empty())
            fpuarch = "80587";

        defines["cpulevel"] = 586;
    }
    else if (march == "pentium-mmx") {
        if (fpuarch.empty())
            fpuarch = "80587";

        defines["cpulevel"] = 586;
        defines["mmx"] = 1;
    }
    else if (march == "80686" || march == "686") {
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["cpulevel"] = 686;
    }
    else if (march == "pentium-pro") {
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["cpulevel"] = 686;
        defines["cmov"] = 1;
    }
    else if (march == "pentium-pro-mmx") {
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["cpulevel"] = 686;
        defines["cmov"] = 1;
        defines["mmx"] = 1;
    }
    else {
        fprintf(stderr,"Unknown march '%s'\n",march.c_str());
        return 1;
    }

    if (fpuarch == "8087" || fpuarch == "80187") {
        defines["fpulevel"] = 87;
    }
    else if (fpuarch == "80287" || fpuarch == "287") {
        defines["fpulevel"] = 287;
    }
    else if (fpuarch == "80387" || fpuarch == "387" ||
             fpuarch == "80487" || fpuarch == "487" ||
             fpuarch == "80587") {
        defines["fpulevel"] = 387;
    }
    else if (fpuarch == "80687" ) {
        defines["fpulevel"] = 687;
    }
    else {
        fprintf(stderr,"Unknown fpuarch '%s'\n",fpuarch.c_str());
        return 1;
    }

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

    fclose(srcfp);
    return 0;
}

