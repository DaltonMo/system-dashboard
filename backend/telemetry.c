#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_memory(long *total, long *available) {
  FILE *fp = fopen("/proc/meminfo", "r"); // Create ptr to file, read-only
  if (fp == NULL) { // Could not open file
    perror("Failed to open /proc/meminfo");
    exit(1);
  }

  char line[256]; // Create buffer for fgets
  while (fgets(line, sizeof(line), fp)) { // Reads a one line at a time and stores in `line` buffer
    if (sscanf(line, "MemTotal: %ld kB", total) == 1) {
      *total /= 1024; // Convert total from kB -> MB
    }
    if (sscanf(line, "MemAvailable: %ld kB", available) == 1) {
      *available /= 1024; // Convert total from kB -> MB
    }
    if (*total > 0 && *available > 0) {
      break;
    }
  }
  fclose(fp);
}

int main() {
  long memTotal = 0, memAvailable = 0;
  read_memory(&memTotal, &memAvailable);

  printf("Total RAM: %ld MB\n", memTotal);
  printf("Available RAM: %ld MB\n", memAvailable);

  return 0;
}