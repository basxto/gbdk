/* Unix */

#include <stdio.h>

#ifndef GBDKLIBDIR
#define GBDKLIBDIR "/home/michaelh/projects/gbdk-lib"
#endif

#include <string.h>

#define DEFAULT_ASM	"gbz80"

#define CPP_NAME	"/bin/sdcpp"
#define COM_NAME	"/bin/sdcc"
#define AS_NAME		"/bin/as"
#define LD_NAME		"/bin/link"

char *suffixes[] = { ".c", ".i", ".asm;.s", ".o;.obj", ".ihx.gb", 0 };
char inputs[256] = "";

char *cpp[] = { GBDKLIBDIR CPP_NAME,
		"-Wall", "-lang-c++", "-DSDCC=1", "-DSDCC_PORT=" DEFAULT_ASM,
		"-DSDCC_PLAT=gb", "-DGB", "-DGAMEBOY", "-DINT_16_BITS",
		"$1", "$2", "$3", 0 };
char *include[] = { "-I" GBDKLIBDIR "/include", 0 };

char *com[] = { GBDKLIBDIR COM_NAME,
		"-mgbz80", "--c1mode",
		"$1", "$2", "$3", 0 };

char *as[] = { GBDKLIBDIR AS_NAME "-" DEFAULT_ASM, "-pog", "$1", "$3", "$2", 0 };
char *ld[] = { GBDKLIBDIR LD_NAME "-" DEFAULT_ASM, "-n", "--",
	       "-z",
	       "$1", 
	       "-k" GBDKLIBDIR "lib/" DEFAULT_ASM" /", "-l" DEFAULT_ASM ".lib",
	       "-k" GBDKLIBDIR "lib/gb/", "-lgb.lib",
	       "$3", GBDKLIBDIR "lib/gb/crt0.o", "$2", 0 };

extern char *concat(const char *, const char *);

const char *starts_with(const char *s1, const char *s2)
{
    if (!strncmp(s1, s2, strlen(s2))) {
	return s1+strlen(s2);
    }
    return NULL;
}

static struct {
    const char *port;
    const char *sasm;
    const char *bin;
    const char *include;
    const char *lib;
} _G = {
    "gb", DEFAULT_ASM, GBDKLIBDIR "/bin", GBDKLIBDIR "/include",
    GBDKLIBDIR "/lib"
};

static void _set(void)
{
    cpp[0] = concat(_G.bin, CPP_NAME);
    cpp[4] = concat("-DSDCC_PORT=", _G.sasm);
    cpp[5] = concat("-DSDCC_PLAT=", _G.port);

    com[0] = concat(_G.bin, COM_NAME);
    as[0] = concat(_G.bin, concat(AS_NAME, concat("-", _G.sasm)));
    ld[0] = concat(_G.bin, concat(LD_NAME, concat("-", _G.sasm)));
	
    ld[5] = concat("-k", concat(_G.lib, concat("/", concat(_G.sasm, "/"))));
    ld[6] = concat("-l", concat(_G.sasm, ".lib"));
    ld[7] = concat("-k", concat(_G.lib, concat("/", concat(_G.port, "/"))));
    ld[8] = concat("-l", concat(_G.port, ".lib"));
    ld[10] = concat(_G.lib, concat("/", concat(_G.port, "/crt0.o")));
    com[1] = concat("-m", _G.sasm);

    include[0] = concat("-I", _G.include);
}

int option(char *arg) {
    const char *tail;
    if ((tail = starts_with(arg, "--gbdkdir="))) {
	/* Go through and set all of the paths */
	_G.bin = strdup(tail);
	_G.lib = concat(tail, "/lib");
	_G.include = concat(tail, "/include");
	_set();
	return 1;
    }
    else if ((tail = starts_with(arg, "--gbdklibdir="))) {
	_G.lib = strdup(tail);
	_set();
	return 1;
    }
    else if ((tail = starts_with(arg, "--gbdkincludedir="))) {
	_G.include = strdup(tail);
	_set();
	return 1;
    }
    else if ((tail = starts_with(arg, "--ihx"))) {
	ld[3] = strdup("-i");
	return 1;
    }
    else if ((tail = starts_with(arg, "-m"))) {
	/* Split it up into a asm/port pair */
	char *slash = strchr(tail, '/');
	if (slash) {
	    *slash++ = '\0';
	    _G.port = strdup(slash);
	}
	_G.sasm = strdup(tail);
	_set();
	return 1;
    }
    return 0;
}
