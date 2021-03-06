#include <stdio.h>

struct sty {
    int f;
};

struct sty s;

int foo(struct sty *p) {
    p->f ++;
    return 0;
}

int bar(struct sty *u) {
    return 0;
}

int see(struct sty *x) {
    foo(x);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) return 0;
    s.f = 0;
    FILE *f = fopen(argv[1], "r");
    struct sty *p[2];
    p[0] = &s;
    p[1] = &s;
    if (f == NULL) return 0;
    int x;
    fscanf(f, "%d", &x);
    fclose(f);
    for (int i = 0; i < x; i++)
        see(p[0]);
    for (int i = 0; i < x; i++)
        bar(p[1]);
    printf("%d\n", s.f);
    return 0;
}
