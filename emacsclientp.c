#include <stdio.h>
#include <stdlib.h>
#ifdef __MINGW32__
# include <windows.h>
# define PATH_SEP ";"
#else
# include <alloca.h>
# define PATH_SEP ":"
#endif
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#ifndef _countof
#define _countof(x) (sizeof(x)/sizeof((x)[0]))
#endif
int main(int argc, char* argv[])
{
    int idx = 0;
#ifdef __MINGW32__
    char emacsclient[] = "emacsclientw";
#else
    char emacsclient[] = "emacsclient";
#endif
    char *opts[] = {"-a", "", "-c", NULL};
    char *apath = alloca(1024);
    const char *opath = getenv("PATH");
    size_t len1 = 0;
    size_t len2 = 0;
    char *npath = NULL;
    char **nargv;
#ifndef __MINGW32__             /* readlink not implemented on MinGW */
    readlink("/proc/self/exe", apath, 1024 - 1);
#else
    GetModuleFileName(NULL, apath, 1024 - 1);
#endif
    apath = dirname(apath);
    len1 = strlen(opath);
    len2 = sizeof("PATH=") + 1 + len1 + strlen(apath);
    npath = alloca(len2);

    /* add the "/bin" to PATH */
    snprintf(npath, len2, "PATH=%s%s%s", apath, PATH_SEP, opath);
    /* setenv() is not implemented on MinGW */
    putenv(npath);

    /* copy the original */
    nargv = alloca((argc + 1 + _countof(opts)) * sizeof(nargv[0])); /* argv end with NULL, argv[argc] == NULL */
    memcpy(nargv, argv, (argc + 1) * sizeof(argv[0]));
    nargv[0] = emacsclient;
    /* detect options */
    for (idx = 0; idx < argc; idx++) {
        if (argv[idx][0] == '-')
            break;
    }
    if (idx >= argc) {          /* not option in argv, add opts as default */
        for (size_t i = 0; i < _countof(opts); i++) {
            nargv[argc + i] = opts[i];
        }
    }
    execvp(nargv[0], nargv);
    return 0;
}
