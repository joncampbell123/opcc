
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <string>

enum tokentype_t {
    TOK_NONE=0,
    TOK_UINT,
    TOK_INT,
    TOK_FLOAT
};

struct tokenstate_t {
    enum tokentype_t    type;
    std::string         string;
    union {
        uint64_t        u;
        int64_t         i;
    } intval;
    long double         floatval;
} tokenstate;

FILE*           srcfp = NULL;
std::string     srcfile;

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

    fclose(srcfp);
    return 0;
}

