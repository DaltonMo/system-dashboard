#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CPU_USER      0
#define CPU_NICE      1
#define CPU_SYSTEM    2
#define CPU_IDLE      3
#define CPU_IOWAIT    4
#define CPU_IRQ       5
#define CPU_SOFTIRQ   6
#define CPU_STEAL     7
#define CPU_GUEST     8
#define CPU_GUEST_NICE 9
#define CPU_FIELD_COUNT 10

typedef struct {
  double usage;
} CPUInfo;

typedef struct {
  long total;
  long available;
} RAMInfo;

RAMInfo read_memory() {
  RAMInfo ram = {0, 0};
  FILE *fp = fopen("/proc/meminfo", "r"); // Create ptr to file, read-only
  if (fp == NULL) { // Could not open file
    perror("Failed to open /proc/meminfo");
    exit(1);
  }

  char line[256]; // Create buffer for fgets
  while (fgets(line, sizeof(line), fp)) { // Reads a one line at a time and stores in `line` buffer
    if (sscanf(line, "MemTotal: %ld kB", &ram.total) == 1) {
      ram.total /= 1024; // Convert total from kB -> MB
    }
    if (sscanf(line, "MemAvailable: %ld kB", &ram.available) == 1) {
      ram.available /= 1024; // Convert total from kB -> MB
    }
    if (ram.total > 0 && ram.available > 0) {
      break;
    }
  }

  fclose(fp);

  return ram;
}

void read_cpu_times(long *total, long *idle) {
  FILE *fp = fopen("/proc/stat", "r");
  if (fp == NULL) {
    perror("Failed to open /proc/stats");
    exit(1);
  }

  char label[5]; // "cpu" char[] buffer
  long fields[CPU_FIELD_COUNT];
  fscanf(fp, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
        label,
        &fields[CPU_USER], &fields[CPU_NICE], &fields[CPU_SYSTEM], &fields[CPU_IDLE], &fields[CPU_IOWAIT],
        &fields[CPU_IRQ], &fields[CPU_SOFTIRQ], &fields[CPU_STEAL], &fields[CPU_GUEST], &fields[CPU_GUEST_NICE]);

  *idle = fields[3];
  *total = 0;
  for (int i = 0; i < CPU_FIELD_COUNT; i++) {
    *total += fields[i];
  }

  fclose(fp);
}

void sleep_ms(int milliseconds) {
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000;

  nanosleep(&ts, NULL);
}

CPUInfo read_cpu_usage() {
  CPUInfo cpu = {0};

  long total1, idle1, total2, idle2;
  read_cpu_times(&total1, &idle1);
  sleep_ms(250);
  read_cpu_times(&total2, &idle2);

  long delta_total = total2 - total1;
  long delta_idle = idle2 - idle1;

  if (delta_total > 0) {
    cpu.usage = 100.0 * ((double)(delta_total - delta_idle) / delta_total);
  }

  return cpu;
}

void write_json(CPUInfo cpu, RAMInfo ram) {
  FILE *fp = fopen("../data/stats.json", "w");
  if (fp == NULL) {
    perror("Failed to open stats.json for writing");
    exit(1);
  }

  fprintf(fp,
      "{\n"
      "\t\"cpu\": {\n"
      "\t\t\"usage\": %.2f\n"
      "\t},\n"
      "\t\"ram\": {\n"
      "\t\t\"total\": %ld,\n"
      "\t\t\"available\": %ld\n"
      "\t}\n"
      "}\n",
      cpu.usage, ram.total, ram.available
  );

  fclose(fp);
}

int main() {
  while (1) {
    RAMInfo ram = read_memory();
    CPUInfo cpu = read_cpu_usage();

    write_json(cpu, ram);

    printf("Wrote RAM stats to data.json!\n");

    sleep_ms(2000);
  }
}