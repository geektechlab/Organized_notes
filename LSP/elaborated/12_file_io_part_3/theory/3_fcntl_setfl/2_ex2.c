#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("/tmp/fd_share.txt", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644);

    // append to empty file
    write(fd, "aaaaaaaaaa", 10);

    off_t cur = lseek(fd, 1, SEEK_SET);
    printf("offset after being set to 1: %ld\n", (long)cur);

    // append
    write(fd, "bbbbbbbb", 8);

    cur = lseek(fd, 0, SEEK_CUR);
    printf("offset after appending bbbbbbbb: %ld\n", (long)cur);

    cur = lseek(fd, 2, SEEK_SET);
    printf("offset after being set to 2: %ld\n", (long)cur);

    // now toggle "append mode" to FALSE
    int open_flag = fcntl(fd, F_GETFL);
    if (fcntl(fd, F_SETFL, open_flag & ~O_APPEND) == -1) {
        printf("failed to set flag\n");
        return 0;
    }

    cur = lseek(fd, 0, SEEK_CUR);
    printf("offset after unsetting O_APPEND: %ld\n", (long)cur);

    cur = lseek(fd, 3, SEEK_SET);
    printf("offset after being set to 3: %ld\n", (long)cur);

    // write without appending
    write(fd, "cccc", 4);

    cur = lseek(fd, 0, SEEK_CUR);
    printf("offset after writing cccc: %ld\n", (long)cur);

    // now toggle "append mode" to TRUE
    open_flag = fcntl(fd, F_GETFL);
    if (fcntl(fd, F_SETFL, open_flag | O_APPEND) == -1) {
        printf("failed to set flag\n");
        return 0;
    }

    cur = lseek(fd, 0, SEEK_CUR);
    printf("offset after unsetting O_APPEND: %ld\n", (long)cur);

    // append
    write(fd, "dd", 2);

    cur = lseek(fd, 0, SEEK_CUR);
    printf("offset after appending dd: %ld\n", (long)cur);

    close(fd);
}
