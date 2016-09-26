#include <stdio.h>
#include <stdlib.h>
#ifndef __MINGW32__
#include <alloca.h>
#endif
#include <unistd.h>
#include <string.h>
#ifndef _countof
#define _countof(x) (sizeof(x)/sizeof((x)[0]))
#endif
int main(int argc, char* argv[])
{
    int idx = 0;
    char emacsclient[] = "emacsclient";
    char *opts[] = {"-a", "", "-c", NULL};
    const char apath[] = "PATH=/bin";
    const char *opath = getenv("PATH");
    const size_t len1 = strlen(opath);
    const size_t len2 = len1 + 1 + 2 + sizeof(apath);
    char *npath = alloca(len2);
    char **nargv;

    /* add the "/bin" to PATH */
    snprintf(npath, len2, "%s:%s", apath, opath);
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
    for (idx = 0; nargv[idx]; idx++)
        printf("%s\n", nargv[idx]);
    execvp(emacsclient, nargv);
    return 0;
}
