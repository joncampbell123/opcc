
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
    "ud"                        // 15
};

struct tokenstate_t {
    enum tokentype_t    type;
    std::string         string;
    union {
        uint64_t        u;
        int64_t         i;
    } intval;
    long double         floatval;

    inline const char *type_str(void) const {
        return tokentype_str[type];
    }
} tokenstate;

FILE*           srcfp = NULL;
std::string     srcfile;
int             untoke = -1;

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
    if (chr < 0) return false;

    switch ((unsigned char)chr) {
        case '-': tok.type = TOK_MINUS;         return true;
        case ';': tok.type = TOK_SEMICOLON;     return true;
        case '(': tok.type = TOK_OPEN_PARENS;   return true;
        case ')': tok.type = TOK_CLOSE_PARENS;  return true;
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
            if (!isalpha(chr)) {
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

int main(int argc,char **argv) {
    if (parse_argv(argc,argv))
        return 1;

    if ((srcfp=fopen(srcfile.c_str(),"r")) == NULL) {
        fprintf(stderr,"Unable to open file '%s', %s\n",srcfile.c_str(),strerror(errno));
        return 1;
    }

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

    fclose(srcfp);
    return 0;
}

