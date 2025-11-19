#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "headers.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

bool file_in_dir(const char *path, const char *snapshot_name);

bool is_valid_snapshot_name(const char *snapshot_name);

bool construct_snapshot_path(const char *cwd, const char *snapshot_name, char *buffer, size_t buffer_size);

bool create_snapshot(const char *snapshot_name, char *file_path);

bool restore_snapshot(const char *snapshot_name, char *file_path);

bool delete_snapshot(const char *snapshot_name, char *file_path);

#endif // SNAPSHOT_H
