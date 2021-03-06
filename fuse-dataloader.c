/* ***************************************************************************
 *
 * Thomas Schmidt, 2020
 *
 * This file is part of the MountOctave Demonstrator Project
 *
 * The file contains a wrapper and helper (intermidieate layer) between local 
 * data layer/storage and the fuse Filesystem. This implementation 
 * accesses the data layer and transalte it to fuse filesystem structures
 * if a mapping needs to be done
 *
 * License: Not Defined Yet
 *
 * Project URL: https://github.com/tseiman/MountOctave 
 *
 ************************************************************************** */


#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "data-container.h"
#include "fuse-dataloader.h"
#include "global.h"

#ifdef IS_MOC
#include "MOCFILE.h"

MOC_DEMO_FILES;

#endif

int oct_dirLoaderCallback(struct Oct_DirLoaderRef_s *ref) {

    ref->filler(ref->buf, ref->filename, NULL, 0);

    return 0;
}

int oct_dirLoader(struct Oct_DirLoaderRef_s *ref) {


    return oct_walkFolders(oct_dirLoaderCallback, ref);

}


int oct_FileLoader(const char *path, char *buf, size_t size, off_t offset) {
    DEBUG_LOG("READ data chunk with size %d at offet %ld file: %s", (int)size, offset, path);


    if (strcmp(path, filepath) == 0) {
	size_t len = strlen(filecontent);
	if (offset >= len) {
    	    return 0;
	}

	if (offset + size > len) {
    	    memcpy(buf, filecontent + offset, len - offset);
    	    return len - offset;
	}

	memcpy(buf, filecontent + offset, size);
	return size;
    }



    if (strcmp(path, filepath1) == 0) {
	size_t len = strlen(filecontent1);
	if (offset >= len) {
    	    return 0;
	}

	if (offset + size > len) {
    	    memcpy(buf, filecontent1 + offset, len - offset);
    	    return len - offset;
	}

	memcpy(buf, filecontent1 + offset, size);
	return size;
    }


    return -ENOENT;

}



int oct_statForPath(const char *path, struct stat *stbuf) {
    struct Oct_ObjectStat_s file_info;


    if(oct_getInfo(path, &file_info)) {
        WARN_LOG("oct_getInfo() returned an error condition: %s",path);
	return -1;
    }
    switch(file_info.type) {
	case OCT_STAT_TYPE_FOLDER:
	    DEBUG_LOG("FOLDER with name addded: %s",path);
	    stbuf->st_mode = S_IFDIR | 0755;
	    stbuf->st_nlink = 1;
	    stbuf->st_size = 4096;
	    stbuf->st_atime = stbuf->st_mtime = stbuf->st_ctime = file_info.atime;
	    break;
	case OCT_STAT_TYPE_FILE:
	    DEBUG_LOG("FILE with name addded: %s",path);
	    stbuf->st_mode = S_IFREG | 0666;
	    stbuf->st_nlink = 1;
	    stbuf->st_size = file_info.filesize;
	    stbuf->st_atime = stbuf->st_mtime = stbuf->st_ctime = file_info.atime;
	    break;
	case OCT_STAT_TYPE_REFERENCE:
	    DEBUG_LOG("LINK with name addded: %s",path);
	    stbuf->st_mode = S_IFLNK | 0777;
	    stbuf->st_nlink = 1;
	    stbuf->st_size = 4096;
	    stbuf->st_atime = stbuf->st_mtime = stbuf->st_ctime = file_info.atime;
	    break;
	default: return -1;
    }

    return 0;
}

int oct_LinkInfo(const char *path, char * linkDstPath, size_t size) {
    return oct_getLinkInfo(path,linkDstPath, size);
}

