// utils/setGovernor.c   used in TDT4258 2026 code example RPi4-5  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *valid_governors[] = {
    "performance", "powersave", "userspace", "ondemand", "conservative", "schedutil"
};
static const int num_valid_governors = sizeof(valid_governors) / sizeof(valid_governors[0]);

static int is_valid_governor(const char *governor) {
    for (int i = 0; i < num_valid_governors; i++) {
        if (strcmp(governor, valid_governors[i]) == 0) return 1;
    }
    return 0;
}

void set_cpu_governor(const char *governor) {
    // Check if the program is running as root
    if (getuid() != 0) {
        fprintf(stderr, "Error: This program must be run as root (sudo).\n");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", "w");
    if (file == NULL) {  // special system file used to set CPU governor
        perror("Failed to open governor file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", governor);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <governor>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (!is_valid_governor(argv[1])) {
        fprintf(stderr, "Error: '%s' is not a valid governor. Valid options: ", argv[1]);
        for (int i = 0; i < num_valid_governors; i++) { // compact way of prrinting list with commas ended by :
            fprintf(stderr, "%s%s", valid_governors[i], i < num_valid_governors - 1 ? ", " : "\n");
        }
        return EXIT_FAILURE;
    }
    set_cpu_governor(argv[1]);
    return EXIT_SUCCESS;
}
