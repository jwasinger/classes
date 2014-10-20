#include "archive.h"

#define STATE_READ_ARCHIVE_HEADER 0
#define STATE_READ_FILE_HEADER 1
#define STATE_READ_FILE 2

#define OSCAR_HDR_SIZE 123
typedef struct
{
    ArchiveFile *files;
    int size_files; /* capacity of 'files' array */
    int num_files; /* number of ArchiveFiles in array */
    char archive_name[32];
} Archive;

typedef struct 
{
    struct oscar_hdr hdr;
    char *file_data;
    int file_size;
} ArchiveFile;

/* expand the array of files in an archive structure if we reach the end of 'files' */ 
int __expand_archive(Archive *archive)
{
    /* copy the files array */
    ArchiveFile *new_array = malloc(sizeof(ArchiveFile) * archive->size_files * 2);
    if(!new_array)
        return -1;
    
    memcpy(new_array, archive->files, sizeof(ArchiveFile) * archive->num_files);
    archive->size_files *= 2;
    
    /* delete the old array */
    free(archive->files);
    archive->files = new_array;
    return 0;
}

int __read_file_header(char *line, struct oscar_hdr hdr)
{
    
}

int read_archive(int fd, Archive **archive)
{
    if(!fd || !archive)
        return -1;

    int state = STATE_READ_ARCHIVE_HEADER;
    int last_state = STATE_INITIAL; /*state of my program on previous line of file being read */
    
    int res = -1;
    char *line = NULL;
    size_t len = 0;
    int fp = NULL;
    ArchiveFile *curr_file = NULL;

    fp = fdopen(fd, "r");
    if(fp == NULL)
    {
        printf("\nfdopen() failed\n");
        return -1;
    }
    
    (*archive) = malloc(sizeof(Archive) * 10);
    (*archive)->num_files = 0;
    (*archive)->archive_name = "test";

    while((res = getline(&line, &len, fp)) != 0)
    {
        if(state == STATE_READ_ARCHIVE_HEADER)
        {
            //try and read out archive header and fail if it's not there
            if(!strcmp(line, OSCAR_ID) != 0)
            {
                printf("\ninvalid archive header\n");
                return -1;
            }
            state = STATE_READ_ARCHIVE;
        }
        else if(state == STATE_READ_FILE_HEADER)
        {
            //try and read out file header and fail if it's not there
            if(len != OSCAR_HDR_SIZE)
            {
                printf("\nInvalid file header size\n");
                return -1;
            }

            if ((*archive)->num_files == (*archive)->size_files) 
            {
                __expand_archive(*archive);
            }
            
            /*copy the line data into the header struct */
            (*archive)->files[num_files] = malloc(sizeof(ArchiveFile));  
            (*archive)->num_files++;
 
            current_file = (*archive)->files[num_files];
            memcpy(current_file->hdr, line, OSCAR_HDR_SIZE);
            
            int i_size = atoi(current_file->hdr->oscar_size);
            current_file->file_data = malloc(i_size);
            current_file->file_size = i_size;

            state = STATE_READ_FILE;
        }
        else if(state == STATE_READ_FILE)
        {
            //read out file data until the end of the 'file' is reached
            if(read(fd, current_file->file_data, current_file->file_size) != 0)
            {
                printf("\nreading file data failed for %s\n", current_file->hdr->oscar_name);
                return -1;
            }
            state = STATE_READ_FILE_HEADER;
        }
    }

    if(state != STATE_READ_FILE_HEADER)
    {
        printf("\nunexpected termination of archive file\n");
        return -1;
    }

    return 0;
}

int open_archive(char *file_name, int *fd_out)
{
	int fd = -1;
	
	/* try and open the file 'file_name' */
	fd = open(file_name, O_RDWR, 0);
	if(fd == -1)
	{
		if(errno == ENOENT)
		{
			/* file doesn't exist, create it */
			printf("creating file '%s'\n", file_name);
			
			fd = open(file_name, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);
			if(fd == -1)
			{
				error_exit();	
			}

		}
	}
	
	/* if it exists see if it is a valid archive and return -1 if not */
    char **lines = NULL;
	int num_lines;

	if(get_file_lines(fd, &lines, &num_lines) == -1)
	{
		return -1;
	}
	
    int i = 0;
	for(; i < num_lines; i++)
	{
		printf(lines[i]);
	}

	/* if it doesn't exist create the file */

	return 0;
}

