#include "base.h"
#include "tar.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static char buf[8192];
static char hdr[512];
static u64 counter = 0;

u64 tar_octal(u8* octal, u64 size) {
    char copy[32];
    strncpy(copy, (char*)octal, size);
    copy[size] = '\0';
    return strtoull(copy, NULL, 8);
}

bool is_null_header(u8* hdr) {
    for (int i = 0; i < 512; i++) {
        if (hdr[i] != 0) {
            return false;
        }
    }
    return true;
}

void close_fd(int* fd) {
    if (*fd != 0) {
        close(*fd);
    }
}

int main(int argc, char** argv) {
    char* path = "";
    if (argc == 2) {
        path = argv[1];
    } else {
        printf("must require argument to tar file\n");
        return 255;
    }

    assert(sizeof(TarHeader) == 512);
    cleanup(close_fd, int) fd = open(path, O_RDONLY);

    TarHeader* header = (TarHeader*)hdr;

    while (1) {
        ssize_t r = read(fd, hdr, 512);
        if (r != 512) break;

        if (is_null_header((u8*)hdr)) {
            r = read(fd, hdr, 512);
            if (r != 512) break;
            if (is_null_header((u8*)hdr)) break;
        }

        printf("%s %s %s\n", header->filename, header->size, header->typeflag);

        u64 size = tar_octal(header->size, sizeof(header->size));
        u8 type = header->typeflag[0];

        if (type == '0' || type == 0) {
            cleanup(close_fd, int) new_fd = open(
                (char*)header->filename,
                O_CREAT | O_WRONLY | O_TRUNC | O_NOFOLLOW | O_CLOEXEC,
                tar_octal(header->mode, sizeof(header->mode))
            );

            counter = size;
            while (counter > 0) {
                u64 to_read = counter < 8192 ? counter : 8192;
                ssize_t r = read(fd, buf, to_read);
                if (r <= 0) {
                    perror("read");
                    break;
                }

                ssize_t w = write(new_fd, buf, r);
                if (w <= 0) {
                    perror("write");
                    break;
                }

                counter -= r;
            }

        } else if (type == '5') {
            mkdir((char*)header->filename, tar_octal(header->mode, sizeof(header->mode)));
        } else {
            printf("unknown file type\n");
            continue;
        }

        u64 padding = ((size + 511) / 512) * 512 - size;
        if (padding > 0) {
            lseek(fd, padding, SEEK_CUR);
        }
    }

    return 0;
}
