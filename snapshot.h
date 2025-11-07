#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "headers.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>


bool file_in_dir(const char *path, const char *snapshot_name);

bool create_snapshot(const char *snapshot_name, char *cwd);

bool restore_snapshot(const char *snapshot_name, char *cwd);

bool delete_snapshot(const char *snapshot_name, char *cwd);

#endif // SNAPSHOT_H
