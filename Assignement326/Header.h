#pragma once

#include <iostream>
#include <stdio.h>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <sstream>

#define MEMORY_SIZE 124

// Memory Block Table structure
typedef struct s_mbt {
	int size;
	bool content[124];
} mbt;

// Page table structure
typedef struct s_page {
	int size;
	bool *content;
} page;

// PCB structure
typedef struct s_pcb {
	int pid;
	page table;
} pcb;
