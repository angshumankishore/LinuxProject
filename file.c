#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

void list_needed_libraries(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        close(fd);
        return;
    }

    void *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)map;
    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "%s is not an ELF file.\n", filename);
        munmap(map, st.st_size);
        close(fd);
        return;
    }

    Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)map + ehdr->e_phoff);
    Elf64_Dyn *dynamic = NULL;
    size_t dyn_size = 0;

    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_DYNAMIC) {
            dynamic = (Elf64_Dyn *)((char *)map + phdr[i].p_offset);
            dyn_size = phdr[i].p_filesz;
            break;
        }
    }

    if (!dynamic) {
        fprintf(stderr, "No dynamic section found.\n");
        munmap(map, st.st_size);
        close(fd);
        return;
    }

    const char *strtab = NULL;
    for (Elf64_Dyn *d = dynamic; d->d_tag != DT_NULL; ++d) {
        if (d->d_tag == DT_STRTAB)
            strtab = (const char *)((char *)map + (uintptr_t)d->d_un.d_ptr);
    }

    if (!strtab) {
        fprintf(stderr, "No string table found.\n");
        munmap(map, st.st_size);
        close(fd);
        return;
    }

    printf("Dependencies for %s:\\n", filename);
    for (Elf64_Dyn *d = dynamic; d->d_tag != DT_NULL; ++d) {
        if (d->d_tag == DT_NEEDED)
            printf("  %s\\n", strtab + d->d_un.d_val);
    }

    munmap(map, st.st_size);
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <binary>\\n", argv[0]);
        return 1;
    }

    list_needed_libraries(argv[1]);
    return 0;
}
