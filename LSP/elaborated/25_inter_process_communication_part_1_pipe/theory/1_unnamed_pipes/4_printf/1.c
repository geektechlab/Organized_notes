#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    char *name = "Linux";
    float version = 4.5;
    int filedes = open("hello.txt", O_WRONLY|O_CREAT, S_IWUSR | S_IRUSR);

    FILE *f = fdopen(filedes, "w");
    fprintf(f, "Name:%s version:%f\n", name, version);
    fclose(f);
    return 0;
}
