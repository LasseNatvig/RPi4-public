// utils/setFreq_kHz.c    used in TDT4258 2026 code example RPi4-5  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_FREQS 64

// Reads /sys/.../scaling_available_frequencies into freqs[], returns count (0 on failure).
static int get_available_frequencies(int freqs[MAX_FREQS]) {
    FILE *file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies", "r");
    if (file == NULL) {
        perror("Failed to open scaling_available_frequencies");
        return 0;
    }
    int count = 0;
    while (count < MAX_FREQS && fscanf(file, "%d", &freqs[count]) == 1) {
        count++; // loops stops if freqs array is full or fscanf fails to read more frequencies
    }
    fclose(file);
    return count;  // return number of frequencies found
}

static int is_valid_frequency(int freq, const int freqs[], int count) {
    for (int i = 0; i < count; i++) {
        if (freqs[i] == freq) return 1;
    }
    return 0;
}

void set_cpu_frequency(const char *freq) { // sets the cpu frequency of core 0
    // by writing the freq value to a special system file exposed by the Linux kernel's cpufreq subsystem.

    // Check if the program is running as root
    if (getuid() != 0) {
        fprintf(stderr, "Error: This program must be run as root (sudo).\n");
        exit(EXIT_FAILURE);
    }
    FILE *file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed", "w");
    if (file == NULL) {
        perror("Failed to open frequency file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", freq);  // sets the frequency by writing to that special file
    fclose(file); // Ensures that the write operation is completed.

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <frequency_in_kHz>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *endptr;
    long freq = strtol(argv[1], &endptr, 10); // converts char string to long. 10 is base (decimal)
    if (*endptr != '\0' || endptr == argv[1]) {  // endptr is the first chraracter that could not be converted
        fprintf(stderr, "Error: '%s' is not a valid integer frequency\n", argv[1]);
        return EXIT_FAILURE;
    }
    int freqs[MAX_FREQS];
    int count = get_available_frequencies(freqs);
    if (count == 0) {
        fprintf(stderr, "Error: could not determine available frequencies\n");
        return EXIT_FAILURE;
    }
    if (!is_valid_frequency((int)freq, freqs, count)) {
        fprintf(stderr, "Error: %ld is not a valid frequency. Valid options (kHz): ", freq);
        for (int i = 0; i < count; i++) {
            fprintf(stderr, "%d%s", freqs[i], i < count - 1 ? ", " : "\n");
        }
        return EXIT_FAILURE;
    }
    set_cpu_frequency(argv[1]);
    return EXIT_SUCCESS;
}
