
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

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <list>
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
    TOK_FPU,
    TOK_TOP,
    TOK_ST,                     // 185
    TOK_CONSTANT,
    TOK_F80BCD,
    TOK_CW,
    TOK_MACRO,
    TOK_CR,                     // 190
    TOK_DR,
    TOK_TR,
    TOK_MM,
    TOK_IMPLIED,
    TOK_XMM,                    // 195
    TOK_DQW,

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
    "REP",
    "FPU",
    "TOP",
    "ST",                       // 185
    "CONSTANT",
    "F80BCD",
    "CW",
    "MACRO",
    "CR",                       // 190
    "DR",
    "TR",
    "MM",
    "IMPLIED",
    "XMM",                      // 195
    "DQW"
};

bool list_op = false;
bool debug_op = false;
bool read_error = false;
bool verbose_op = false;

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
        if (tok.string == "FPU") {
            tok.type = TOK_FPU;
            return true;
        }
        if (tok.string == "TOP") {
            tok.type = TOK_TOP;
            return true;
        }
        if (tok.string == "ST") {
            tok.type = TOK_ST;
            return true;
        }
        if (tok.string == "CONSTANT") {
            tok.type = TOK_CONSTANT;
            return true;
        }
        if (tok.string == "F80BCD") {
            tok.type = TOK_F80BCD;
            return true;
        }
        if (tok.string == "CW") {
            tok.type = TOK_CW;
            return true;
        }
        if (tok.string == "MACRO") {
            tok.type = TOK_MACRO;
            return true;
        }
        if (tok.string == "CR") {
            tok.type = TOK_CR;
            return true;
        }
        if (tok.string == "DR") {
            tok.type = TOK_DR;
            return true;
        }
        if (tok.string == "TR") {
            tok.type = TOK_TR;
            return true;
        }
        if (tok.string == "MM") {
            tok.type = TOK_MM;
            return true;
        }
        if (tok.string == "IMPLIED") {
            tok.type = TOK_IMPLIED;
            return true;
        }
        if (tok.string == "XMM") {
            tok.type = TOK_XMM;
            return true;
        }
        if (tok.string == "DQW") {
            tok.type = TOK_DQW;
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
            else if (!strcmp(a,"lop")) {
                list_op = true;
            }
            else if (!strcmp(a,"vop")) {
                verbose_op = true;
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
    std::vector<unsigned int>   fpu;                    // if TOK_FPU
    tokenstate_t                fpu_st;                 // if TOK_ST or TOK_MM or TOK_XMM
    std::vector<tokenstate_t>   var_expr;
    std::vector<tokenstate_t>   constant;
public:
    std::string                 to_string(void);
    std::string                 pretty_string(void);
};

class ByteSpec : public SingleByteSpec, public std::vector<uint8_t> {
public:
    std::string                 to_string(void);
};

bool operator<(const ByteSpec &a,const ByteSpec &b) {
         if (a.meaning                          < b.meaning)                    return true;
    else if (a.meaning                          > b.meaning)                    return false;

         if ((std::vector<uint8_t>)a            < (std::vector<uint8_t>)b)      return true;
    else if ((std::vector<uint8_t>)a            > (std::vector<uint8_t>)b)      return false;

    return false;
}

class OpcodeSpec {
public:
    std::vector<ByteSpec>       bytes;                  // opcode byte sequence
    std::vector<ByteSpec>       assign;                 // assignments
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
    unsigned char               rm_constraint = 0;
    signed char                 mod3 = 0;               // -3 means mod!=3   3 means mod==3
    unsigned int                rep_condition = 0;      // 0 or TOK_Z
    bool                        wait = false;
    bool                        lock = false;
    bool                        rep_condition_negate = false;
    bool                        fpu = false;
    std::vector<SingleByteSpec> fpu_stack_ops;              // push or pop
    unsigned int                fpu_stack_op_dir = 0;       // TOK_PUSH or TOK_POP
public:
    void                        add_reg_constraint(const unsigned char reg);
    void                        add_rm_constraint(const unsigned char reg);
    std::string                 to_string(void);
    std::string                 pretty_string(void);
};

void OpcodeSpec::add_reg_constraint(const unsigned char reg) {
    reg_constraint |= 1u << reg;
}

void OpcodeSpec::add_rm_constraint(const unsigned char reg) {
    rm_constraint |= 1u << reg;
}

const char *regrmtype_str(unsigned int type) {
    switch (type) {
        case TOK_B:     return "u8";
        case TOK_SB:    return "i8";
        case TOK_W:     return "u16";
        case TOK_SW:    return "i16";
        case TOK_V:     return "uv";
        case TOK_SV:    return "iv";
        case TOK_DW:    return "u32";
        case TOK_SDW:   return "i32";
        case TOK_TW:    return "u48";
        case TOK_QW:    return "u64";
        case TOK_SQW:   return "i64";
        case TOK_F32:   return "f32";
        case TOK_F64:   return "f64";
        case TOK_F80:   return "f80";
        case TOK_F80BCD:return "fbcd";
        case TOK_F87ENV:return "f87env";
        case TOK_F87STATE:return "f87state";
        case TOK_FPW:   return "far16";
        case TOK_FPDW:  return "far32";
        case TOK_FPV:   return "farptr";
        case TOK_REG:   return "reg";
        case TOK_RM:    return "r/m";
        case TOK_DQW:   return "u128";
    };

    return "";
}

std::string SingleByteSpec::pretty_string(void) {
    std::string res;

    if (meaning == TOK_RM) {
        if (!res.empty()) res += " ";
        res += "r/m(";
        res += regrmtype_str(rm_type);
        res += ")";
    }
    else if (meaning == TOK_TR) {
        if (!res.empty()) res += " ";
        res += "tr(";
        res += regrmtype_str(reg_type);
        res += ")";
    }
    else if (meaning == TOK_CR) {
        if (!res.empty()) res += " ";
        res += "cr(";
        res += regrmtype_str(reg_type);
        res += ")";
    }
    else if (meaning == TOK_DR) {
        if (!res.empty()) res += " ";
        res += "dr(";
        res += regrmtype_str(reg_type);
        res += ")";
    }
    else if (meaning == TOK_REG) {
        if (!res.empty()) res += " ";
        res += "reg(";
        res += regrmtype_str(reg_type);
        res += ")";
    }
    else if (meaning == TOK_SREG) {
        if (!res.empty()) res += " ";
        res += "segreg(";
        res += regrmtype_str(reg_type);
        res += ")";
    }
    else if (meaning == TOK_IMMEDIATE) {
        if (!res.empty()) res += " ";
        res += "imm(";
        res += regrmtype_str(immediate_type);
        res += ")";
    }
    else if (meaning == TOK_UINT) {
        char tmp[64];
        sprintf(tmp,"%llu",(unsigned long long)intval);
        if (!res.empty()) res += " ";
        res += tmp;
    }
    else if (meaning == TOK_AV) {
        if (!res.empty()) res += " ";
        res += "Av";
    }
    else if (meaning == TOK_ST) {
        if (fpu_st.type != 0) {
            if (fpu_st.type == TOK_REG ||
                fpu_st.type == TOK_RM) {
                res += "st(i)";
            }
            else if (fpu_st.type == TOK_UINT) {
                char tmp[64];
                sprintf(tmp,"st(%llu)",(unsigned long long)fpu_st.intval.u);
                if (!res.empty()) res += " ";
                res += tmp;
            }
            else {
                res += "st(?)";
            }
        }
        else {
            res += "st(?)";
        }
    }
    else if (meaning == TOK_MM) {
        if (fpu_st.type != 0) {
            if (fpu_st.type == TOK_REG ||
                fpu_st.type == TOK_RM) {
                res += "mm(";
                res += tokentype_str[fpu_st.type];
                res += ")";
            }
            else if (fpu_st.type == TOK_IMPLIED) { // for Cyrix "implied" mmx register i.e. reg ^ 1
                // example: mm1 is implied by mm0
                res += "mm(reg^1)";
            }
            else if (fpu_st.type == TOK_UINT) {
                char tmp[64];
                sprintf(tmp,"mm(%llu)",(unsigned long long)fpu_st.intval.u);
                if (!res.empty()) res += " ";
                res += tmp;
            }
            else {
                res += "mm(?)";
            }
        }
        else {
            res += "mm(?)";
        }
    }
    else if (meaning == TOK_XMM) {
        if (fpu_st.type != 0) {
            if (fpu_st.type == TOK_REG ||
                fpu_st.type == TOK_RM) {
                res += "xmm(";
                res += tokentype_str[fpu_st.type];
                res += ")";
            }
            else if (fpu_st.type == TOK_UINT) {
                char tmp[64];
                sprintf(tmp,"xmm(%llu)",(unsigned long long)fpu_st.intval.u);
                if (!res.empty()) res += " ";
                res += tmp;
            }
            else {
                res += "xmm(?)";
            }
        }
        else {
            res += "xmm(?)";
        }
    }
    else if (meaning == TOK_MEMORY) {
        res += regrmtype_str(memory_type);
        if (!res.empty()) res += " ";

        if (memseg_type != 0 && memseg_type != TOK_SEG) {
            res += tokentype_str[memseg_type];
            res += ":";
        }

        res += "[";
        for (auto i=var_expr.begin();i!=var_expr.end();i++) {
            if (i != var_expr.begin())
                res += " ";

            if ((*i).type == TOK_AMPERSAND)
                res += "&";
            else
                res += std::string((*i).type_str());
        }
        res += "]";
    }
    else if (meaning == TOK_CONSTANT) {
        res += "const(";
        for (auto i=constant.begin();i!=constant.end();i++) {
            if (i != constant.begin())
                res + " ";

            if ((*i).type == TOK_STRING)
                res += "\""+(*i).string+"\"";
            else
                res += (*i).to_string();
        }
        res += ")";
    }
    else if (meaning == 0) {
        return "";
    }
    else {
        res += tokentype_str[meaning];
    }

    return res;
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
    if (!fpu.empty()) {
        if (!res.empty()) res += ",";
        res += "fpu=[";
        for (auto i=fpu.begin();i!=fpu.end();) {
            res += tokentype_str[*i];
            i++;
            if (i != fpu.end()) res += ",";
        }
        res += "]";
    }
    if (fpu_st.type != 0) {
        if (!res.empty()) res += ",";
        res += "fpu_st=";
        if (fpu_st.type == TOK_UINT) {
            char tmp[64];
            sprintf(tmp,"%llu",(unsigned long long)(fpu_st.intval.u));
            res += tmp;
        }
        else {
            res += tokentype_str[fpu_st.type];
        }
    }
    if (!constant.empty()) {
        if (!res.empty()) res += ",";
        res += "constant=[";
        for (auto i=constant.begin();i!=constant.end();) {
            if ((*i).type == TOK_UINT) {
                char tmp[64];
                sprintf(tmp,"%llu",(unsigned long long)((*i).intval.u));
                res += tmp;
            }
            else if ((*i).type == TOK_STRING) {
                res += "\"";
                res += (*i).string;
                res += "\"";
            }
            else {
                res += tokentype_str[(*i).type];
            }

            i++;
            if (i != constant.end()) res += ",";
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

const size_t        opspec_ins_col_len = 60;

std::string OpcodeSpec::pretty_string(void) {
    std::string res;
    std::string params;
    std::string byte_str;
    std::string byte_strvas;
    bool i07_key=false;
    char tmp[64];

    if (destination.meaning != 0) {
        if (!params.empty()) params += ", ";
        params += destination.pretty_string();
    }
    for (auto i=param.begin();i!=param.end();i++) {
        if ((*i).meaning != 0) {
            if (!params.empty()) params += ", ";
            params += (*i).pretty_string();
        }
    }

    unsigned int tok_regand7_assign = 0;
    unsigned int tok_opbyte_assign = 0;
    unsigned int tok_opbyte_assign_base = 0;
    unsigned int tok_opbyte_index = 0;

    for (size_t i=0;i < bytes.size();i++) {
        if (bytes[i].meaning == 0 && bytes[i].var_assign != 0 && bytes[i].size() > 0) {
            tok_opbyte_index = i;
            tok_opbyte_assign = bytes[i].var_assign;
            tok_opbyte_assign_base = bytes[i][0];
            break;
        }
        else if (bytes[i].size() > 1) {
            /* [range] [range] cannot be expressed as opbyte+i */
            break;
        }
    }

    for (auto i=assign.begin();i!=assign.end();i++) {
        if ((*i).meaning == TOK_EQUAL) {
            if (!byte_strvas.empty())
                byte_strvas += " ";

            if ((*i).var_assign != 0) {
                byte_strvas += tokentype_str[(*i).var_assign];
                byte_strvas += "=";
            }

            byte_strvas += "(";
            for (auto j=(*i).var_expr.begin();j!=(*i).var_expr.end();j++) {
                if ((*j).type == TOK_AMPERSAND)
                    byte_strvas += "&";
                else if ((*j).type == TOK_UINT)
                    byte_strvas += (*j).to_string();
                else if ((*j).type == TOK_PLUS)
                    byte_strvas += "+";
                else
                    byte_strvas += tokentype_str[(*j).type];
            }
            byte_strvas += ")";

            if ((*i).var_assign == TOK_REG) {
                if ((*i).var_expr.size() == 3 && tok_opbyte_assign != 0) {
                    if ((*i).var_expr[0].type == tok_opbyte_assign &&
                        (*i).var_expr[1].type == TOK_AMPERSAND &&
                        (*i).var_expr[2].type == TOK_UINT &&
                        (*i).var_expr[2].intval.u == 7) {
                        tok_regand7_assign = 1;
                    }
                }
            }
        }
    }

    if (tok_regand7_assign) {
        assert(tok_opbyte_index < bytes.size());
        auto &x = bytes[tok_opbyte_index];
        unsigned int min,expect;
        bool ok = false;

        if (x.size() > 1) {
            auto i = x.begin();
            min = *i; i++; expect = min + 1;
            ok = true;
            while (i != x.end()) {
                if ((min & (~7)) != ((*i) & (~7))) {
                    ok = false;
                    break;
                }
                if (*i != expect) {
                    ok = false;
                    break;
                }
                expect = *i + 1;
                i++;
            }
        }

        if (!ok) tok_regand7_assign = false;
    }

    for (auto i=bytes.begin();i!=bytes.end();i++) {
        if ((*i).meaning == 0) {
            if ((*i).size() > 1) {
                if (!byte_str.empty())
                     byte_str += " ";

                if ((*i).var_assign != 0) {
                    byte_str += tokentype_str[(*i).var_assign];
                    byte_str += "=";
                }

                byte_str += "[";
                for (auto j=(*i).begin();j!=(*i).end();j++) {
                    if (j != (*i).begin()) byte_str += "|";
                    sprintf(tmp,"%02x",(*j));
                    byte_str += tmp;
                }
                byte_str += "]";
            }
            else if ((*i).size() == 1) {
                if (!byte_str.empty())
                     byte_str += " ";

                if ((*i).var_assign != 0) {
                    byte_str += tokentype_str[(*i).var_assign];
                    byte_str += "=";
                }

                sprintf(tmp,"%02x",(*i)[0]);
                byte_str += tmp;
            }
        }
        else if ((*i).meaning == TOK_MRM) {
            unsigned int c,v;
            char tmp[64];

            if (!byte_str.empty())
                 byte_str += " ";

            if (mod3 == 3 &&
                reg_constraint != 0 && (reg_constraint & (reg_constraint - 1)) == 0 &&
                 rm_constraint != 0 && ( rm_constraint & ( rm_constraint - 1)) == 0) {
                unsigned char b = 0xC0;

                c = 0;
                v = reg_constraint;
                while (v > 1) {
                    v >>= 1;
                    c++;
                }
                b += c << 3;

                c = 0;
                v = rm_constraint;
                while (v > 1) {
                    v >>= 1;
                    c++;
                }
                b += c;

                sprintf(tmp,"%02x",b);
                byte_str += tmp;
            }
            else if (mod3 == 3 &&
                reg_constraint != 0 && (reg_constraint & (reg_constraint - 1)) == 0 &&
                 rm_constraint == 0) {
                unsigned char b = 0xC0;
                bool is_i = false;

                c = 0;
                v = reg_constraint;
                while (v > 1) {
                    v >>= 1;
                    c++;
                }
                b += c << 3;

                if (destination.fpu_st.type == TOK_RM)
                    is_i = true;
                else if (param.size() == 1 &&
                         param[0].fpu_st.type == TOK_RM)
                    is_i = true;

                if (is_i)
                    sprintf(tmp,"%02x+i",b);
                else
                    sprintf(tmp,"%02x+rm",b);

                if (is_i)
                    i07_key=true;

                byte_str += tmp;
            }
            else if (reg_constraint != 0) {
                if ((reg_constraint & (reg_constraint - 1)) == 0) {/*power of 2*/
                    c = 0;
                    v = reg_constraint;
                    while (v > 1) {
                        v >>= 1;
                        c++;
                    }

                    sprintf(tmp,"/%u",c);
                    byte_str += tmp;
                }
                else {
                    // TODO: Find a way to indicate a range of reg values
                    byte_str += "/r"; /* Intel uses this syntax, so follow it */
                }

                if (rm_constraint != 0)
                    byte_str += "=rm";

                if (mod3 == 3)
                    byte_str += "!m";
                else if (mod3 == -3)
                    byte_str += "=m";
            }
            else {
                byte_str += "/r"; /* Intel uses this syntax, so follow it */

                if (rm_constraint != 0)
                    byte_str += "=rm";

                if (mod3 == 3)
                    byte_str += "!m";
                else if (mod3 == -3)
                    byte_str += "=m";
            }
        }
        else if ((*i).meaning == TOK_IMMEDIATE) {
            if (!byte_str.empty())
                 byte_str += " ";

            if ((*i).var_assign != 0) {
                byte_str += tokentype_str[(*i).var_assign];
                byte_str += "=";
            }

            byte_str += "imm(";
            byte_str += regrmtype_str((*i).immediate_type);
            byte_str += ")";
        }
    }

    if (tok_regand7_assign) {
        sprintf(tmp,"reg=%u-7",tok_opbyte_assign_base&7);
        byte_strvas = tmp;

        assert(tok_opbyte_index < bytes.size());

        byte_str.clear();
        for (size_t i=0;i < tok_opbyte_index;i++) {
            if (bytes[i].size() > 0) {
                sprintf(tmp,"%02x ",bytes[i][0]);
                byte_str += tmp;
            }
        }

        sprintf(tmp,"%02x+reg",tok_opbyte_assign_base&(~7));
        byte_str += tmp;
    }

    while (params.size() < (opspec_ins_col_len-9)) params += " ";
    if (type == TOK_PREFIX) params += "; prefix ";

    while (params.size() < opspec_ins_col_len) params += " ";

    if (i07_key) {
        if (!byte_strvas.empty())
            byte_strvas += " ";

        byte_strvas += "i=0-7";
    }

    res += params;
    res += "; ";
    res += byte_str;
    if (!byte_strvas.empty()) {
        res += "; ";
        res += byte_strvas;
    }

    return res;
}

std::string OpcodeSpec::to_string(void) {
    std::string res;

    if (!bytes.empty()) {
        res += "bytes=(";
        for (auto i=bytes.begin();i!=bytes.end();) {
            res += (*i).to_string();
            i++;
            if (i != bytes.end()) res += " ";
        }
        res += ")";
    }

    if (!assign.empty()) {
        if (!res.empty()) res += ",";
        res += "assign=(";
        for (auto i=assign.begin();i!=assign.end();) {
            res += (*i).to_string();
            i++;
            if (i != assign.end()) res += " ";
        }
        res += ")";
    }

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

    if (prefix_seg_assign != 0) {
        if (!res.empty()) res += ",";
        res += "seg=";
        res += tokentype_str[prefix_seg_assign];
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

    if (rm_constraint != 0) {
        unsigned int c = 0;

        if (!res.empty()) res += ",";
        res += "rmconstraint=[";
        for (unsigned int b=0;b < 16;b++) {
            if (rm_constraint & (1u << b)) {
                if (c != 0) res += ",";

                char tmp[16];
                sprintf(tmp,"%u",b);
                res += tmp;
                c++;
            }
        }
        res += "]";
    }

    if (mod3 == -3 || mod3 == 3) {
        unsigned char minval=(mod3 == 3) ? 0xC0 : 0x00;
        unsigned char maxval=(mod3 == 3) ? 0xFF : 0xBF;

        if (reg_constraint != 0 && /*power of 2*/(reg_constraint & (reg_constraint - 1)) == 0) {
            unsigned int c,v;

            c = 0;
            v = reg_constraint;
            while (v > 1) {
                v >>= 1;
                c++;
            }
            minval += (c << 3);
            maxval  = minval | 0x7;

            if (rm_constraint != 0 && (rm_constraint & (rm_constraint - 1)) == 0) {
                c = 0;
                v = rm_constraint;
                while (v > 1) {
                    v >>= 1;
                    c++;
                }
                minval += c;
                maxval  = minval;
            }

            if (!res.empty()) res += ",";
            res += "mrm-comb-range=[";

            for (unsigned int a=0;a < ((mod3!=3)?0xC0:0x40);a += 0x40) {
                if (a != 0) res += ",";

                char tmp[64];
                if (minval == maxval)
                    sprintf(tmp,"%02x",minval+a);
                else
                    sprintf(tmp,"%02x-%02x",minval+a,maxval+a);

                res += tmp;
            }

            res += "]";
        }
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

    if (fpu_stack_ops.size() != 0) {
        if (!res.empty()) res += ",";
        res += "fpu_stack_ops(";
        if (stack_op_dir == TOK_PUSH)
            res += "push";
        else if (stack_op_dir == TOK_POP)
            res += "pop";
        res += ")=[";
        for (auto i=fpu_stack_ops.begin();i!=fpu_stack_ops.end();) {
            res += (*i).to_string();
            i++;
            if (i!=fpu_stack_ops.end()) res += " ";
        }
        res += "]";
    }

    return res;
}

static tokenstate_t tokenstate_t_none;

std::vector<ByteSpec> bytes_only_bytes(const std::vector<ByteSpec> &s) {
    std::vector<ByteSpec> r;

    for (const auto &b : s) {
        if (b.meaning == 0 ||
            b.meaning == TOK_MRM) {
            r.push_back(b);
        }
    }

    return r;
}

bool opcode_sort_func(const OpcodeSpec &a,const OpcodeSpec &b) {
    {
        std::vector<ByteSpec> ab = bytes_only_bytes(a.bytes);
        std::vector<ByteSpec> bb = bytes_only_bytes(b.bytes);

             if (ab                 < bb)                       return true;
        else if (ab                 > bb)                       return false;
    }

    if (a.mod3 != 0 && b.mod3 != 0) {
             if (a.mod3             < b.mod3)                   return true;
        else if (a.mod3             > b.mod3)                   return false;
    }

         if (a.reg_constraint       < b.reg_constraint)         return true;
    else if (a.reg_constraint       > b.reg_constraint)         return false;

         if (a.rm_constraint        < b.rm_constraint)          return true;
    else if (a.rm_constraint        > b.rm_constraint)          return false;

    return false;
}

bool opcode_sort_func_by_name(const OpcodeSpec &a,const OpcodeSpec &b) {
         if (a.name                 < b.name)                   return true;
    else if (a.name                 > b.name)                   return false;

    return opcode_sort_func(a,b);
}

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

class Macro {
public:
    std::vector<tokenlist>      tlist;
    std::vector<unsigned int>   param_tokens;
};

std::map<std::string,Macro>     macros;

/* temporary replacement of stream by a list of tokenlists.
 * this is needed for the macro system to work. */
bool tokens_macro_enable = false;
std::vector<tokenlist> tokens_macro;
size_t tokens_macro_read = 0;

bool tokens_unput_valid = false;
tokenlist tokens_unput;

void tokens_macro_begin(void) {
    tokens_macro_enable = true;
    tokens_macro_read = 0;
}

void tokens_macro_clear(void) {
    tokens_macro_enable = false;
    tokens_macro.clear();
}

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
        case TOK_DV:
        case TOK_TW:
        case TOK_SHV:
        case TOK_FPV:
        case TOK_FPW:
        case TOK_FPDW:
        case TOK_F32:
        case TOK_F64:
        case TOK_F80:
        case TOK_F80BCD:
        case TOK_F87ENV:
        case TOK_F87STATE:
        case TOK_QW:
        case TOK_DQW:
        case TOK_SQW:
        case TOK_REG:
        case TOK_RM:
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

    if (t.type == TOK_UINT || t.type == TOK_INT || t.type == TOK_FLOAT || t.type == TOK_STRING || t.type == TOK_BOOLEAN ||
        t.type == TOK_ES || t.type == TOK_DS || t.type == TOK_CS || t.type == TOK_SS) {
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

bool parse_code_constant_spec(std::vector<tokenstate_t> &constant,tokenlist &tokens) {
    /* caller ate TOK_FLAGS */
    if (tokens.next().type != TOK_OPEN_PARENS) return false;

    do {
        auto &n = tokens.next();

        if (n.type == TOK_CLOSE_PARENS) break;

        switch (n.type) {
            case TOK_UINT:
            case TOK_FLOAT:
            case TOK_STRING:
                constant.push_back(n);
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

bool parse_code_st_spec(tokenstate_t &st,tokenlist &tokens) {
    /* caller ate TOK_FLAGS */
    if (tokens.next().type != TOK_OPEN_PARENS) return false;

    st.type = TOK_NONE;
    do {
        auto &n = tokens.next();

        if (n.type == TOK_CLOSE_PARENS) break;

        switch (n.type) {
            case TOK_UINT:
            case TOK_REG:
            case TOK_RM:
            case TOK_IMPLIED: // allowed for Cyrix "implied mmx register"
            case TOK_ALL:
                if (st.type != 0)
                    return false;

                st = n;
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

bool parse_code_fpu_spec(std::vector<unsigned int> &fpu,tokenlist &tokens) {
    /* caller ate TOK_FLAGS */
    if (tokens.next().type != TOK_OPEN_PARENS) return false;

    do {
        auto &n = tokens.next();

        if (n.type == TOK_CLOSE_PARENS) break;

        switch (n.type) {
            case TOK_ALL:
            case TOK_TOP:
            case TOK_CW:
            case TOK_SW:
#if 0
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
#endif
                fpu.push_back(n.type);
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
        else if (bs.meaning == TOK_ST || bs.meaning == TOK_MM || bs.meaning == TOK_XMM) {
            if (!parse_code_st_spec(bs.fpu_st,/*&*/tokens)) {
                fprintf(stderr,"Invalid st() spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_CONSTANT) {
            if (!parse_code_constant_spec(bs.constant,/*&*/tokens)) {
                fprintf(stderr,"Invalid constant() spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_FPU) {
            if (!parse_code_fpu_spec(bs.fpu,/*&*/tokens)) {
                fprintf(stderr,"Invalid fpu spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_REG || bs.meaning == TOK_SREG || bs.meaning == TOK_CR || bs.meaning == TOK_DR || bs.meaning == TOK_TR) {
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
                fprintf(stderr,"far... unexpected token %s\n",n.type_str());
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

bool code_subexpr(tokenstate_t &subtoken,tokenlist &tokens) {
    tokenlist subtokens;
    int parens = 1;

    do {
        auto &t = tokens.next();
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

        if (n.type == TOK_Z || n.type == TOK_C/*NEC v20*/) {
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

    /* fpu stack push ... */
    /* fpu stack pop ... */
    if (tokens.peek(0).type == TOK_FPU && tokens.peek(1).type == TOK_STACK) {
        tokens.discard(2);

        if (tokens.peek().type == TOK_PUSH) {
            tokens.discard();

            if (spec.fpu_stack_op_dir != TOK_NONE && spec.fpu_stack_op_dir != TOK_PUSH) {
                fprintf(stderr,"Stack ops can be push or pop, not both\n");
                return false;
            }

            spec.fpu_stack_op_dir = TOK_PUSH;
        }
        else if (tokens.peek().type == TOK_POP) {
            tokens.discard();

            if (spec.fpu_stack_op_dir != TOK_NONE && spec.fpu_stack_op_dir != TOK_POP) {
                fprintf(stderr,"Stack ops can be push or pop, not both\n");
                return false;
            }

            spec.fpu_stack_op_dir = TOK_POP;
        }
        else {
            return false;
        }

        if (!parse_sbl_list(spec.fpu_stack_ops,tokens))
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
        else if (bs.meaning == TOK_REG || bs.meaning == TOK_SREG || bs.meaning == TOK_CR || bs.meaning == TOK_DR || bs.meaning == TOK_TR) {
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
        else if (bs.meaning == TOK_ST || bs.meaning == TOK_MM || bs.meaning == TOK_XMM) {
            if (!parse_code_st_spec(bs.fpu_st,/*&*/tokens)) {
                fprintf(stderr,"Invalid st() spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_CONSTANT) {
            if (!parse_code_constant_spec(bs.constant,/*&*/tokens)) {
                fprintf(stderr,"Invalid constant() spec\n");
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
        else if (bs.meaning == TOK_REG || bs.meaning == TOK_SREG || bs.meaning == TOK_CR || bs.meaning == TOK_DR || bs.meaning == TOK_TR) {
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
        else if (bs.meaning == TOK_ST || bs.meaning == TOK_MM || bs.meaning == TOK_XMM) {
            if (!parse_code_st_spec(bs.fpu_st,/*&*/tokens)) {
                fprintf(stderr,"Invalid st() spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_CONSTANT) {
            if (!parse_code_constant_spec(bs.constant,/*&*/tokens)) {
                fprintf(stderr,"Invalid constant() spec\n");
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
        else if (bs.meaning == TOK_ST || bs.meaning == TOK_MM || bs.meaning == TOK_XMM) {
            if (!parse_code_st_spec(bs.fpu_st,/*&*/tokens)) {
                fprintf(stderr,"Invalid st() spec\n");
                return false;
            }
        }
        else if (bs.meaning == TOK_CONSTANT) {
            if (!parse_code_constant_spec(bs.constant,/*&*/tokens)) {
                fprintf(stderr,"Invalid constant() spec\n");
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

                    bs.meaning = TOK_EQUAL; /* don't make it a byte sequence */
                    spec.assign.push_back(bs);
                    continue;
                }
            }

            if (tokens.peek().type == TOK_OPEN_PARENS) {
                tokens.discard();

                tokenstate_t subtoken;

                if (!code_subexpr(subtoken,tokens))
                    return false;

                if (subtoken.type == TOK_UINT) {
                    bs.push_back((uint8_t)subtoken.intval.u);
                }
                else {
                    fprintf(stderr,"code eval, invalid type %s\n",subtoken.type_str());
                    return false;
                }

                if (tokens.peek().type == TOK_MINUS ||
                    tokens.peek().type == TOK_COMMA) {
                    fprintf(stderr,"ranges or multiples not supported at this time\n");
                    return false;
                }

                spec.bytes.push_back(bs);
            }
            else if (tokens.peek().type == TOK_UINT) {
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
            else if (tokens.peek().type == TOK_FPU) {
                tokens.discard();

                spec.fpu = true;
            }
            else if (tokens.peek().type == TOK_MRM) {
                tokens.discard();

                bs.meaning = TOK_MRM;
                spec.bytes.push_back(bs);
            }
            else if ((tokens.peek(0).type == TOK_REG || tokens.peek(0).type == TOK_RM) && tokens.peek(1).type == TOK_OPEN_PARENS) {
                unsigned int typ = tokens.peek(0).type;
                tokens.discard(2);

                do {
                    auto &n = tokens.next();

                    if (n.type == TOK_UINT) {
                        if (n.intval.u < 16) {
                            if (typ == TOK_RM)
                                spec.add_rm_constraint((unsigned char)n.intval.u);
                            else
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
    fprintf(stderr,"Parse error, next token %s\n",tokens.peek().type_str());
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
                read_error = true;
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
                read_error = true;
                return false;
            }
        }
    } while (1);

    return true;
}

bool read_opcode_block(void);

bool process_if_statement(tokenlist &tokens,bool suppress=false);

bool process_block(tokenlist &tokens);
bool read_opcode_token_block(tokenlist &tokens);
void unput_opcode_token_block(tokenlist &tokens);

bool process_if_block(const bool result_bool,tokenlist &tokens) {
    while (true) {
        if (!read_opcode_token_block(tokens)) // will clear and refill tokens
            return false;

        if (tokens.peek(0).type == TOK_CLOSE_CURLYBRACKET && tokens.peek(1).type == TOK_IF) {
            tokens.discard(2);
            if (!tokens.eof()) {
                fprintf(stderr,"IF end unexpected tokens\n");
                read_error = true;
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
        read_error = true;
        return false;
    }

    bool result_bool = result.to_bool() && !suppress;

    if (tokens.peek().type == TOK_OPEN_CURLYBRACKET) {
        tokens.discard();

        if (!tokens.eof()) {
            fprintf(stderr,"if { unexpected token '%s'\n",tokens.peek().type_str());
            read_error = true;
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
            read_error = true;
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
                read_error = true;
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
        unput_opcode_token_block(tokens);
    }

    return true;
}

/* caller already ate SET MACRO "name" {; */
bool process_macro_def(Macro &macro) {
    tokenlist tokens;

    do {
        if (!read_opcode_token_block(/*&*/tokens))
            return false;

        /* end of macro is "} macro" */
        if (tokens.peek(0).type == TOK_CLOSE_CURLYBRACKET && tokens.peek(1).type == TOK_MACRO) {
            tokens.discard(2);
            if (!tokens.eof()) {
                fprintf(stderr,"End macro unexpected tokens\n");
                read_error = true;
                return false;
            }

            return true;
        }
        /* no recursive macros allowed! */
        if (tokens.peek(0).type == TOK_SET && tokens.peek(1).type == TOK_MACRO && tokens.peek(2).type == TOK_STRING) {
            fprintf(stderr,"Recursive macro definitions not allowed\n");
            read_error = true;
            return false;
        }

        macro.tlist.push_back(tokens);
    } while(1);

    return false;
}

bool process_block(tokenlist &tokens) {
    if (tokens.peek(0).type == TOK_IF) {
        tokens.discard(); // discard IF

        if (!process_if_statement(tokens))
            return false;

        return true;
    }
    /* set macro "name" {; ...; } macro; */
    if (tokens.peek(0).type == TOK_SET && tokens.peek(1).type == TOK_MACRO && tokens.peek(2).type == TOK_STRING) {
        std::string name = tokens.peek(2).string;
        tokens.discard(3);

        if (name.empty()) {
            fprintf(stderr,"name is empty\n");
            read_error = true;
            return false;
        }

        /* do not allow re-defining macros */
        if (macros.find(name) != macros.end()) {
            fprintf(stderr,"macro '%s' already defined\n",name.c_str());
            read_error = true;
            return false;
        }

        Macro &macro = macros[name]; /* operator[] will make it exist */

        if (tokens.peek().type == TOK_OPEN_PARENS) {
            /* comma separated parameters */
            tokens.discard();

            do {
                auto &n = tokens.next();

                if (n.type == TOK_CLOSE_PARENS) {
                    break;
                }
                else if (is_valid_immediate_assign_var(n.type)) {
                    if (std::find(macro.param_tokens.begin(),macro.param_tokens.end(),n.type) != macro.param_tokens.end()) {
                        fprintf(stderr,"Macro param %s already used\n",tokentype_str[n.type]);
                        read_error = true;
                        return false;
                    }
                    else {
                        macro.param_tokens.push_back(n.type);
                    }
                }
                else {
                    fprintf(stderr,"Invalid token in macro param list\n");
                    read_error = true;
                    return false;
                }

                n = tokens.peek();
                if (n.type == TOK_COMMA) {
                    tokens.discard();
                }
                else if (n.type == TOK_CLOSE_PARENS) {
                    tokens.discard();
                    break;
                }
                else {
                    fprintf(stderr,"Unexpected token in macro params\n");
                    read_error = true;
                    return false;
                }
            } while (1);
        }

        if (tokens.peek().type != TOK_OPEN_CURLYBRACKET) {
            fprintf(stderr,"macro def syntax error\n");
            read_error = true;
            return false;
        }
        tokens.discard();

        /* nothing after curly brace */
        if (!tokens.eof()) {
            fprintf(stderr,"Extra tokens after beginning of macro def\n");
            read_error = true;
            return false;
        }

        if (!process_macro_def(macro))
            return false;

        return true;
    }
    /* macro "name" */
    if (tokens.peek(0).type == TOK_MACRO && tokens.peek(1).type == TOK_STRING) {
        std::string name = tokens.peek(1).string;
        std::vector<tokenstate_t> params;
        tokens.discard(2);

        if (name.empty()) {
            fprintf(stderr,"name is empty\n");
            read_error = true;
            return false;
        }

        if (macros.find(name) == macros.end()) {
            fprintf(stderr,"macro '%s' not defined\n",name.c_str());
            read_error = true;
            return false;
        }

        if (tokens.peek().type == TOK_OPEN_PARENS) {
            tokens.discard();

            do {
                auto &n = tokens.peek();

                if (n.type == TOK_CLOSE_PARENS) {
                    tokens.discard();
                    break;
                }
                else if (n.type == TOK_COMMA) {
                    tokens.discard();
                    params.push_back(tokenstate_t());
                }
                else {
                    tokenstate_t result;

                    if (!eval_if_condition(/*&*/result,/*&*/tokens)) {
                        fprintf(stderr,"Macro param if condition error\n");
                        read_error = true;
                        return false;
                    }

                    params.push_back(result);

                    n = tokens.peek();
                    if (n.type == TOK_COMMA) {
                        tokens.discard();
                    }
                    else if (n.type == TOK_CLOSE_PARENS) {
                        tokens.discard();
                        break;
                    }
                    else {
                        fprintf(stderr,"Unexpected tokens in macro call\n");
                        read_error = true;
                        return false;
                    }
                }
            } while (1);
        }

        if (!tokens.eof()) {
            fprintf(stderr,"Extra tokens after beginning of macro def\n");
            read_error = true;
            return false;
        }

        Macro &macro = macros[name];

        if (params.size() != macro.param_tokens.size()) {
            fprintf(stderr,"Macro '%s' param count mismatch (got %zu, expected %zu)\n",name.c_str(),params.size(),macro.param_tokens.size());
            read_error = true;
            return false;
        }

        size_t o_read = tokens_macro_read;
        bool o_enable = tokens_macro_enable;
        auto o_macro = tokens_macro;

        tokens_macro.clear();
        for (auto &tlist : macro.tlist) {
            tokenlist tcopy;

            while (!tlist.eof()) {
                auto &n = tlist.next();

                if (n.type == TOK_VALUE && tlist.peek(0).type == TOK_OPEN_PARENS &&
                    is_valid_immediate_assign_var(tlist.peek(1).type) &&
                    tlist.peek(2).type == TOK_CLOSE_PARENS) {
                    unsigned int tok = tlist.peek(1).type;
                    tlist.discard(3);

                    int paramidx = -1;
                    for (size_t i=0;i < macro.param_tokens.size();i++) {
                        if (macro.param_tokens[i] == tok) {
                            paramidx = (int)i;
                            break;
                        }
                    }

                    if (paramidx == -1) {
                        fprintf(stderr,"macro value() eval no such parameter %s\n",tokentype_str[tok]);
                        read_error = true;
                        return false;
                    }

                    assert((size_t)paramidx < params.size());
                    assert(params.size() == macro.param_tokens.size());

                    tcopy.push_back(params[paramidx]);
                }
                else {
                    tcopy.push_back(n);
                }
            }

            tokens_macro.push_back(tcopy);
            tlist.rewind();
        }

        tokens_macro_begin();

        while (read_opcode_block());

        tokens_macro = o_macro;
        tokens_macro_enable = o_enable;
        tokens_macro_read = o_read;

        if (read_error) {
            fprintf(stderr,"Error processing macro\n");
            read_error = true;
            return false;
        }

        return true;
    }

    /* dialect "string" */
    if (tokens.peek(0).type == TOK_DIALECT && tokens.peek(1).type == TOK_STRING) {
        std::string dialect = tokens.peek(1).string;
        tokens.discard(2);

        if (!tokens.eof()) {
            fprintf(stderr,"Desc unexpected tokens\n");
            read_error = true;
            return false;
        }

        if (!supported_dialect(dialect)) {
            fprintf(stderr,"This compiler does not support dialect '%s'\n",dialect.c_str());
            read_error = true;
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
            read_error = true;
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
            read_error = true;
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
            read_error = true;
            return false;
        }

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            read_error = true;
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
            read_error = true;
            return false;
        }

        tokenstate_t result;

        if (!eval_if_condition(/*&*/result,/*&*/tokens)) {
            fprintf(stderr,"'If' condition error\n");
            read_error = true;
            return false;
        }
        defines[name] = result;

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            read_error = true;
            return false;
        }

        return true;
    }
    /* comment "string" */
    if (tokens.peek(0).type == TOK_COMMENT && tokens.peek(1).type == TOK_STRING) {
        tokens.discard(2);

        if (!tokens.eof()) {
            fprintf(stderr,"Unexpected tokens\n");
            read_error = true;
            return false;
        }

        return true;
    }

    if (!do_opcode_spec(/*&*/tokens))
        return false;

    return true;
}

void unput_opcode_token_block(tokenlist &tokens) {
    assert(tokens_unput_valid == false);
    tokens_unput_valid = true;
    tokens_unput = tokens;
}

bool read_opcode_token_block(tokenlist &tokens) {
    tokens.clear();

    if (tokens_unput_valid) {
        tokens_unput_valid = false;
        tokens = tokens_unput;
        tokens_unput.clear();
        return true;
    }

    if (tokens_macro_enable) {
        // NTS: Macro evaluation should be it's own process block loop
        if (tokens_macro_read >= tokens_macro.size())
            return false;

        tokens = tokens_macro[tokens_macro_read++];
        return true;
    }

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
    read_error = true;
    return false;
unexpected_end:
    fprintf(stderr,"Unexpected end of opcode\n");
    read_error = true;
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

class OpcodeGroupBlock {
public:
    std::vector< std::shared_ptr<OpcodeGroupBlock> >        map;
public:
    enum mapping_type {
        NONE=0,                 // nothing
        LEAF,                   // leaf node
        LINEAR,                 // map[byte]
        MRMLINEAR,              // mod/reg/rm map[byte]
        MODREGRM,               // map[modregrm(mod,reg,rm)]
        PREFIX                  // prefix (no map)

        // future plan: MANDATORY_PREFIX (for weird Pentium III SSE encodings)
    };
    enum mapping_type maptype = NONE;
public:
    bool                        overlap_error = false;
    size_t                      opcode_index = 0;
public:
    inline static unsigned char modregrm(const unsigned char mod,const unsigned char reg,const unsigned char rm) {
        return (mod << 6) + (reg << 3) + rm;
    }
    std::shared_ptr<OpcodeGroupBlock> map_get(const size_t i) {
        if (map.size() <= i)
            map.resize(i+1);

        return map[i];
    }
    std::shared_ptr<OpcodeGroupBlock> map_get_alloc(const size_t i) {
         if (map.size() <= i)
            map.resize(i+1);

         if (map[i].get() == nullptr)
             map[i] = std::make_shared<OpcodeGroupBlock>();

         return map[i];
    }
    const OpcodeSpec &get_opcode(void) {
        if (maptype != LEAF || opcode_index >= opcodes.size())
            abort();

        return opcodes[opcode_index];
    }
};

std::shared_ptr<OpcodeGroupBlock>           opcode_groups;

bool enter_opcode_byte_spec(const OpcodeSpec &opcode,size_t opcode_index,std::shared_ptr<OpcodeGroupBlock> groups,std::vector<ByteSpec>::const_iterator oi) {
    if (oi == opcode.bytes.end()) {
        {
            auto &gs = *groups;

            if (opcode.type == TOK_PREFIX) {
                if (gs.maptype == OpcodeGroupBlock::NONE)
                    gs.maptype = OpcodeGroupBlock::PREFIX;
                else if (gs.maptype != OpcodeGroupBlock::PREFIX) {
                    gs.overlap_error = true;
                    fprintf(stderr,"map overlap error for opcode '%s'\n",opcode.name.c_str());
                    return false;
                }
            }
            else {
                if (gs.maptype == OpcodeGroupBlock::NONE)
                    gs.maptype = OpcodeGroupBlock::LEAF;
                else if (gs.maptype != OpcodeGroupBlock::LEAF) {
                    gs.overlap_error = true;
                    fprintf(stderr,"map overlap error for opcode '%s'\n",opcode.name.c_str());
                    return false;
                }
            }

            gs.opcode_index = opcode_index;
        }
        return true;
    }
    if (groups.get() == nullptr)
        return false;

    const ByteSpec &bs = *oi;

    if (bs.meaning == 0) {
        {
            auto &gs = *groups;

            if (gs.maptype == OpcodeGroupBlock::NONE)
                gs.maptype = OpcodeGroupBlock::LINEAR;
            else if (gs.maptype == OpcodeGroupBlock::PREFIX) {
                /* mandatory prefix (i.e. to turn MMX instructions into SSE) */
                /* it's ok */
            }
            else if (gs.maptype != OpcodeGroupBlock::LINEAR) {
                gs.overlap_error = true;
                fprintf(stderr,"map overlap error for opcode '%s'\n",opcode.name.c_str());
                return false;
            }
        }

        for (const auto &b : bs) {
            auto gsr = (*groups).map_get_alloc(b);
            if (gsr.get() == NULL) return false;

            if (!enter_opcode_byte_spec(opcode,opcode_index,gsr,oi+1))
                return false;
        }
    }
    else if (bs.meaning == TOK_IMMEDIATE) {
        {
            auto &gs = *groups;

            if (opcode.type == TOK_PREFIX) {
                fprintf(stderr,"Prefixes cannot have immediate operands\n");
                return false;
            }

            if (gs.maptype == OpcodeGroupBlock::NONE)
                gs.maptype = OpcodeGroupBlock::LEAF;
            else if (gs.maptype != OpcodeGroupBlock::LEAF) {
                gs.overlap_error = true;
                fprintf(stderr,"map overlap error for opcode '%s'\n",opcode.name.c_str());
                return false;
            }

            gs.opcode_index = opcode_index;
        }

        /* pass the immediate specs.
         * NTS: Some instructions, such as "ENTER", have more than one immediate */
        while (oi != opcode.bytes.end() && (*oi).meaning == TOK_IMMEDIATE) oi++;

        if (oi != opcode.bytes.end()) {
            fprintf(stderr,"immediate followed by more\n");
            return false;
        }
    }
    else if (bs.meaning == TOK_MRM) {
        {
            auto &gs = *groups;

            if (opcode.type == TOK_PREFIX) {
                fprintf(stderr,"Prefixes cannot have mod/reg/rm\n");
                return false;
            }

            if (opcode.mod3 == 0 && opcode.reg_constraint == 0 && opcode.rm_constraint == 0) {
                assert(oi != opcode.bytes.end());

                /* AMD 3DNow! encoding check:
                 * current *oi is TOK_MRM, next one should be byte range,
                 * and then THAT'S IT. */
                if (  (oi+1) != opcode.bytes.end() &&       // this token is MRM...
                    (*(oi+1)).meaning == 0) {               // and the next is a opcode byte range

                    if (gs.maptype == OpcodeGroupBlock::NONE) {
                        gs.maptype = OpcodeGroupBlock::MRMLINEAR;
                    }
                    else if (gs.maptype != OpcodeGroupBlock::MRMLINEAR) {
                        gs.overlap_error = true;
                        fprintf(stderr,"map overlap error for opcode '%s'\n",opcode.name.c_str());
                        return false;
                    }

                    for (const auto &b : (*(oi+1))) {
                        auto gsr = (*groups).map_get_alloc(b);
                        if (gsr.get() == NULL) return false;

                        if (!enter_opcode_byte_spec(opcode,opcode_index,gsr,oi+2))
                            return false;
                    }

                    return true;
                }

                if (gs.maptype == OpcodeGroupBlock::NONE)
                    gs.maptype = OpcodeGroupBlock::LEAF;
                else if (gs.maptype != OpcodeGroupBlock::LEAF) {
                    gs.overlap_error = true;
                    fprintf(stderr,"map overlap error for opcode '%s'\n",opcode.name.c_str());
                    return false;
                }

                return true;
            }

            if (gs.maptype == OpcodeGroupBlock::NONE)
                gs.maptype = OpcodeGroupBlock::MODREGRM;
            else if (gs.maptype != OpcodeGroupBlock::MODREGRM) {
                gs.overlap_error = true;
                fprintf(stderr,"map overlap error for opcode '%s' mrm\n",opcode.name.c_str());
                return false;
            }
        }

        for (unsigned char mod=0;mod <= 3;mod++) {
            if (opcode.mod3 == -3 && mod == 3) /* mod(!3) */
                continue;
            else if (opcode.mod3 == 3 && mod != 3) /* mod(3) */
                continue;

            for (unsigned char reg=0;reg <= 7;reg++) {
                for (unsigned char rm=0;rm <= 7;rm++) {
                    if (opcode.reg_constraint != 0) {
                        if (!(opcode.reg_constraint & (1 << reg)))
                            continue;
                    }
                    if (opcode.rm_constraint != 0) {
                        if (!(opcode.rm_constraint & (1 << rm)))
                            continue;
                    }

                    unsigned char b = (mod << 6) + (reg << 3) + rm;

                    auto gsr = (*groups).map_get_alloc(b);
                    if (gsr.get() == NULL) return false;

                    if (!enter_opcode_byte_spec(opcode,opcode_index,gsr,oi+1))
                        return false;
                }
            }
        }
    }
    else {
        fprintf(stderr,"bs %s, not supported\n",tokentype_str[bs.meaning]);
        return false;
    }

    return true;
}

bool enter_opcode_bytes(const OpcodeSpec &opcode,size_t opcode_index,std::shared_ptr<OpcodeGroupBlock> groups) {
    if (opcode.bytes.size() == 0)
        return false;

    if (!enter_opcode_byte_spec(opcode,opcode_index,groups,opcode.bytes.begin()))
        return false;

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
    else if (march == "necv20") {
        if (fpuarch.empty())
            fpuarch = "8087";

        defines["cpulevel"] = 186;
        defines["necv20"] = 1;
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
    else if (march == "80486" || march == "486" || march == "486new") {
        if (fpuarch.empty())
            fpuarch = "80387";

        defines["cpulevel"] = 486;
        defines["cpuyear"] = 1992;
        defines["cpuid"] = 1;
    }
    else if (march == "80486old" || march == "486old") {
        if (fpuarch.empty())
            fpuarch = "80387";

        defines["cpulevel"] = 486;
        defines["cpuyear"] = 1988;
    }
    else if (march == "80586" || march == "586") {
        if (fpuarch.empty())
            fpuarch = "80587";

        defines["cpulevel"] = 586;
        defines["cpuid"] = 1;
    }
    else if (march == "pentium") {
        if (fpuarch.empty())
            fpuarch = "80587";

        defines["cpulevel"] = 586;
        defines["cpuid"] = 1;
    }
    else if (march == "pentium-mmx") {
        if (fpuarch.empty())
            fpuarch = "80587";

        defines["cpulevel"] = 586;
        defines["cpuid"] = 1;
        defines["mmx"] = 1;
    }
    else if (march == "amd-k6") {
        if (fpuarch.empty())
            fpuarch = "80587";

        defines["vendor"] = "amd";
        defines["cpulevel"] = 586;
        defines["syscall"] = 1;
        defines["cpuid"] = 1;
        defines["mmx"] = 1;
    }
    else if (march == "amd-k6-2") {
        if (fpuarch.empty())
            fpuarch = "80587";

        defines["vendor"] = "amd";
        defines["cpulevel"] = 586;
        defines["syscall"] = 1;
        defines["3dnow"] = 1;
        defines["cpuid"] = 1;
        defines["mmx"] = 1;
    }
    else if (march == "amd-k6-2+") {
        if (fpuarch.empty())
            fpuarch = "80587";

        defines["vendor"] = "amd";
        defines["cpulevel"] = 586;
        defines["syscall"] = 1;
        defines["3dnow+"] = 1;
        defines["3dnow"] = 1;
        defines["cpuid"] = 1;
        defines["mmx"] = 1;
    }
    else if (march == "80686" || march == "686") {
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["cpulevel"] = 686;
        defines["cpuid"] = 1;
    }
    else if (march == "pentium-pro") {
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["cpulevel"] = 686;
        defines["cpuid"] = 1;
        defines["cmov"] = 1;
    }
    else if (march == "pentium-pro-mmx") {
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["cpulevel"] = 686;
        defines["cpuid"] = 1;
        defines["cmov"] = 1;
        defines["mmx"] = 1;
    }
    else if (march == "pentium-2") {
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["cpulevel"] = 686;
        defines["sysenter"] = 1;
        defines["pentium"] = 2;
        defines["cpuid"] = 1;
        defines["cmov"] = 1;
        defines["mmx"] = 1;
    }
    else if (march == "pentium-3") {
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["cpulevel"] = 686;
        defines["sysenter"] = 1;
        defines["pentium"] = 3;
        defines["cpuid"] = 1;
        defines["cmov"] = 1;
        defines["mmx"] = 1;
        defines["sse"] = 1;
    }
    else if (march == "cyrix-6x86-mmx") {
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["cpulevel"] = 686;
        defines["sysenter"] = 1;
        defines["cpuid"] = 1;
        defines["emmi"] = 1;
        defines["cmov"] = 1;
        defines["mmx"] = 1;
    }
    else if (march == "everything") { /* catch-all for generic decompilers. cover as much as possible without overlapping opcodes. */
        if (fpuarch.empty())
            fpuarch = "80687";

        defines["everything"] = 1;
        defines["cpulevel"] = 686;
        defines["sysenter"] = 1;
        defines["syscall"] = 1;
        defines["3dnow+"] = 1;
        defines["cpuid"] = 1;
        defines["3dnow"] = 1;
        defines["cmov"] = 1;
        defines["mmx+"] = 1;
        defines["sse"] = 1;
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

    if (read_error) {
        fprintf(stderr,"Parse error, exiting\n");
        return 1;
    }

    std::sort(opcodes.begin(),opcodes.end(),opcode_sort_func);

    /* build opcodes into the group table */
    opcode_groups = std::make_shared<OpcodeGroupBlock>();
    {
        auto grref = *opcode_groups;
        grref.map.resize(256);
        grref.maptype = OpcodeGroupBlock::LINEAR;
    }
    for (auto op_i=opcodes.begin();op_i!=opcodes.end();op_i++) {
        const auto &opcode = *op_i;

        if (opcode.bytes.size() == 0) {
            fprintf(stderr,"WARNING: opcode '%s' without bytes\n",opcode.name.c_str());
            continue;
        }

        /* bytes[0] must be actual bytes */
        if (opcode.bytes[0].meaning != 0) {
            fprintf(stderr,"ERROR: opcode '%s' first entry not a byte value\n",opcode.name.c_str());
            continue;
        }
        {
            auto i = opcode.bytes.begin();

            /* must be <byte-range> [<byte-range>] [mrm] [immediate]
             * exception: AMD 3DNow! 0x0F 0x0F mrm <byte-range> */
            while (i != opcode.bytes.end() && (*i).meaning == 0) i++;
            if ((i+0) != opcode.bytes.end() &&
                (i+1) != opcode.bytes.end() &&
                (*(i+0)).meaning == TOK_MRM &&      // mrm
                opcode.mod3 == 0 &&                 // no constraints on mrm
                opcode.reg_constraint == 0 &&
                opcode.rm_constraint == 0 &&
                (*(i+1)).meaning == 0) {            // then byte
                /* allow */
                assert((i+0) != opcode.bytes.end());
                assert((i+1) != opcode.bytes.end());
                i += 2;
            }
            else {
                if (i != opcode.bytes.end() && (*i).meaning == TOK_MRM) i++;
                while (i != opcode.bytes.end() && (*i).meaning == TOK_IMMEDIATE) i++;
            }

            if (i != opcode.bytes.end()) {
                fprintf(stderr,"ERROR: opcode '%s' unexpected byte entries\n",opcode.name.c_str());
                continue;
            }
        }

        if (!enter_opcode_bytes(opcode,(size_t)(op_i-opcodes.begin()),opcode_groups)) {
            fprintf(stderr,"Opcode byte to map error\n");
            continue;
        }
    }

    if (list_op) {
        printf("Opcodes by byte:\n");
        printf("----------------\n");

        if (verbose_op) {
            for (auto i=opcodes.begin();i!=opcodes.end();i++)
                printf("    %s %s: %s\n",tokentype_str[(*i).type],(*i).name.c_str(),(*i).to_string().c_str());
        }
        else {
            for (auto i=opcodes.begin();i!=opcodes.end();i++)
                printf("%12s %s\n",(*i).name.c_str(),(*i).pretty_string().c_str());
        }
        printf("\n");

        {
            auto opcodes_by_name = opcodes;
            std::sort(opcodes_by_name.begin(),opcodes_by_name.end(),opcode_sort_func_by_name);

            printf("Opcodes by name:\n");
            printf("----------------\n");
            if (verbose_op) {
                for (auto i=opcodes_by_name.begin();i!=opcodes_by_name.end();i++)
                    printf("    %s %s: %s\n",tokentype_str[(*i).type],(*i).name.c_str(),(*i).to_string().c_str());
            }
            else {
                for (auto i=opcodes_by_name.begin();i!=opcodes_by_name.end();i++)
                    printf("%12s %s\n",(*i).name.c_str(),(*i).pretty_string().c_str());
            }
            printf("\n");
        }

        // NTS: cannot use std::vector for the loop below. holding an iterator while pushing additional
        //      entries can invalidate the iterator (because std::vector has to reallocate).
        //      use a list instead because the iterator is a pointer to a node in a linked list which
        //      does not invalidate when extending the list.
        std::list< std::pair< std::vector<uint8_t>, std::shared_ptr<OpcodeGroupBlock> > > subl;
        std::list< std::pair< std::vector<uint8_t>, std::shared_ptr<OpcodeGroupBlock> > > submrm;

        {
            std::pair< std::vector<uint8_t>, std::shared_ptr<OpcodeGroupBlock> > p;
            p.second = opcode_groups;
            subl.push_back(p);
        }

        for (auto si=subl.begin();si!=subl.end();si++) {
            assert((*si).second.get() != nullptr);
            auto sgroup = (*si).second;

            printf("Opcode coverage (");
            if ((*si).first.empty()) {
                printf("single opcode");
            }
            else {
                for (auto bi=(*si).first.begin();bi!=(*si).first.end();bi++)
                    printf("%02x ",*bi);

                printf("...");

                if ((*sgroup).maptype == OpcodeGroupBlock::MRMLINEAR)
                    printf(" with mod/reg/rm before last byte");
                else if ((*sgroup).maptype == OpcodeGroupBlock::PREFIX)
                    printf(" mandatory prefix");
            }
            printf("):\n");
            printf("------------------------------\n");
            printf("X = coverage  O = overlap(!)  M = multi-byte  R = group by mod/reg/rm\n");
            printf("P = prefix\n");
            printf("\n");

            {
                printf("    ");
                for (unsigned int x=0;x < 16;x++) printf("%x ",x);
                printf("\n");

                printf("   ");
                for (unsigned int x=0;x < 16;x++) printf("--");
                printf("\n");

                for (unsigned int y=0;y < 16;y++) {
                    printf("  %x|",y);
                    for (unsigned int x=0;x < 16;x++) {
                        unsigned char c = ' ';
                        unsigned char c2 = ' ';

                        auto gsr = (*sgroup).map_get((y*16)+x);
                        if (gsr.get() != nullptr) {
                            const auto &gs = *gsr;

                            if (gs.maptype == OpcodeGroupBlock::LINEAR ||
                                gs.maptype == OpcodeGroupBlock::MRMLINEAR ||
                                (gs.maptype == OpcodeGroupBlock::PREFIX && !gs.map.empty())) {
                                c = 'M';

                                std::pair< std::vector<uint8_t>, std::shared_ptr<OpcodeGroupBlock> > p;
                                p.first = (*si).first;
                                p.first.push_back((y*16)+x);
                                p.second = gsr;
                                assert(p.second.get() != nullptr);
                                subl.push_back(p);
                            }
                            else if (gs.maptype == OpcodeGroupBlock::MODREGRM) {
                                c = 'R';

                                std::pair< std::vector<uint8_t>, std::shared_ptr<OpcodeGroupBlock> > p;
                                p.first = (*si).first;
                                p.first.push_back((y*16)+x);
                                p.second = gsr;
                                assert(p.second.get() != nullptr);
                                submrm.push_back(p);
                            }
                            else if (gs.maptype == OpcodeGroupBlock::LEAF) {
                                c = 'X';
                            }
                            else if (gs.maptype == OpcodeGroupBlock::PREFIX) {
                                c = 'P';
                            }
                            else {
                                c = '?';
                            }
                        }
                        else {
                            c = ' ';
                        }

                        printf("%c%c",(char)c,(char)c2);
                    }
                    printf("\n");
                }
            }

            printf("\n");
        }

        for (auto si=submrm.begin();si!=submrm.end();si++) {
            assert((*si).second.get() != nullptr);
            auto sgroup = (*si).second;

            printf("Opcode coverage mod/reg/rm (");
            if ((*si).first.empty()) {
                printf("single opcode");
            }
            else {
                for (auto bi=(*si).first.begin();bi!=(*si).first.end();bi++)
                    printf("%02x ",*bi);

                printf("...");
            }
            printf("):\n");
            printf("------------------------------\n");
            printf("X = coverage  O = overlap(!)  M = multi-byte  R = group by mod/reg/rm\n");
            printf("P = prefix\n");
            printf("\n");

            for (unsigned int mod=0;mod < 4;mod++) {
                printf("    ");
                for (unsigned int x=0;x < 8;x++) printf("%x ",x);
                printf(" rm with mod==%u\n",mod);

                printf("   ");
                for (unsigned int x=0;x < 8;x++) printf("--");
                printf("\n");

                for (unsigned int y=0;y < 8;y++) {
                    printf(" %02x|",(y * 8) + (mod * 64));
                    for (unsigned int x=0;x < 8;x++) {
                        unsigned char c = ' ';
                        unsigned char c2 = ' ';

                        auto gsr = (*sgroup).map_get((mod*64)+(y*8)+x);
                        if (gsr.get() != nullptr) {
                            const auto &gs = *gsr;

                            if (gs.maptype == OpcodeGroupBlock::LINEAR) {
                                c = 'M';
                                c2 = '!';
                            }
                            else if (gs.maptype == OpcodeGroupBlock::MODREGRM) {
                                c = 'R';
                                c2 = '!';
                            }
                            else if (gs.maptype == OpcodeGroupBlock::LEAF) {
                                c = 'X';
                            }
                            else if (gs.maptype == OpcodeGroupBlock::PREFIX) {
                                c = 'P';
                                c2 = '!';
                            }
                            else {
                                c = '?';
                                c2 = '!';
                            }
                        }
                        else {
                            c = ' ';
                        }

                        printf("%c%c",(char)c,(char)c2);
                    }
                    printf(" reg=%u\n",y);
                }

                printf("\n");
            }

            printf("\n");
        }
    }

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

