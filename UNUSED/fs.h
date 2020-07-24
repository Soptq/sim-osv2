//
// Created by Soptq on 2020/7/24.
//

#ifndef SIM_OS_FS_H
#define SIM_OS_FS_H

#include "../cpu/type.h"

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 /* Is the file an active mountpoint?
                             * Here setting it to 8 so that it can be bitwise-or in
                             * with FS_DIRECTORY */

typedef uint32_t (*read_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent * (*readdir_type_t)(struct fs_node*, u32int);
typedef struct fs_node * (*finddir_type_t)(struct fs_node*, char *name);

struct dirent {
    char name[128];
    uint32_t ino;
};

typedef struct fs_node {
    char name[128];
    uint32_t mask;  /* permission mask */
    uint32_t uid;   /* owning user */
    uint32_t gid;   /* owning group */
    uint32_t flags;
    uint32_t inode; /* device-specific number, used for identifying */
    uint32_t length;    /* size (bytes) */
    uint32_t impl;      /* implementation defined number */
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir; /* return the nth child of a directory */
    finddir_type_t finddir; /* find a child in a directory by name */
    struct fs_node *ptr;    /* symlink */
} fs_node_t;

extern fs_node_t *fs_root; /* The root of the filesystem. */

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_fs(fs_node_t *node, uint8_t read, uint8_t write);
void close_fs(fs_node_t *node);
struct dirent *readdir_fs(fs_node_t *node, uint32_t index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);

#endif //SIM_OS_FS_H
