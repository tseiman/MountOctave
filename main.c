
#include <fuse.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include "fuse-octave.h"
#include "http-connector.h"

int main(int argc, char *argv[]) {
//    initFuse(argc, argv);
    char *httpdata = NULL;
    getLocalAction("l5e336e4950f068603b5e6e2f.json", &httpdata);
//    printf("My HTTP data:\n%s\n",httpdata);
    free(httpdata);
    return 0;
}
