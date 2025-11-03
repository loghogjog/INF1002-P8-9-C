#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "headers.h"

void create_snapshot(const char *snapshot_name);

void restore_snapshot(const char *snapshot_name);

#endif // SNAPSHOT_H
