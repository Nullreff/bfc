// Shared macros n stuff
#pragma once

#define CHECK_OOM(pointer) if (!pointer) { fprintf(stderr, "Out of memory!\n"); exit(EXIT_FAILURE); }

