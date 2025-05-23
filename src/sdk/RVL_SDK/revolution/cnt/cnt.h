#ifndef RVL_SDK_CNT_H
#define RVL_SDK_CNT_H

#include "revolution/ARC.h"
#include "revolution/DVD.h"
#include "revolution/mem/mem_allocator.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum CNTDirMode {
    ARC = 1,
    DVD = 2
};

typedef struct _CNTDir {
    union {
        ARCDir arc; // 0x0
        struct {
            u32 pad; // ???
            DVDDir dvd;
        };
    };
    u8 type; // 0x10
} CNTDir;

typedef struct _CNTDirEntry {
    union {
        ARCEntry arc;
        struct {
            u32 pad; // ???
            DVDDirEntry dvd;
        };
    };
    u8 type;
} CNTDirEntry;

typedef struct _CNTHandleNAND {
    ARCHandle ArcHandle;
    long FileDescriptor;
    void *allocator;
} CNTHandleNAND;

typedef struct _CNTHandleDVD {
    unsigned long index;
    long rootDir;
    long currDir;
    u8 reserved[24];
} CNTHandleDVD;

typedef struct _CNTHandle {
    union {
        CNTHandleNAND nand;
        CNTHandleDVD dvd;
    };
    u8 type;
} CNTHandle;

typedef struct _CNTFileInfoNAND {
    CNTHandleNAND *CntHandle;
    unsigned int startOffset;
    unsigned int length;
    long readOffset;
} CNTFileInfoNAND;

typedef struct _CNTFileInfoDVD {
    DVDFileInfo fileInfo;
    long readOffset;
} CNTFileInfoDVD;

typedef struct _CNTFileInfo {
    union {
        CNTFileInfoNAND nand;
        CNTFileInfoDVD dvd;
    };
    u8 type;
} CNTFileInfo;

void CNTInit(void);
int CNTShutdown(void);

int CNTReleaseHandle(CNTHandle *);
CNTHandle *
contentInitHandleTitleNAND(u64 title_id, u32 content_id, void *r6, MEMAllocator *);
u32 contentConvertPathToEntrynumDVD(const char *path);

BOOL CNTOpenDir(CNTHandle *handle, const char *path, CNTDir *dir);
BOOL CNTReadDir(CNTDir *, CNTDirEntry *);
BOOL CNTCloseDir(CNTDir *);

int CNTOpen(CNTHandle *handle, const char *path, CNTFileInfo *fileInfo);
int CNTGetLength(CNTFileInfo *fileInfo);
int CNTClose(CNTFileInfo *fileInfo);
int CNTRead(CNTFileInfo *fileInfo, void *buffer, int length);
int CNTSeek(CNTFileInfo *fileInfo, int position, int seekFrom);

#ifdef __cplusplus
}
#endif
#endif
