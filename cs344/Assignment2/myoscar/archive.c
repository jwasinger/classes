#include "archive.h"

#define STATE_READ_ARCHIVE_HEADER 0
#define STATE_READ_FILE_HEADER 1
#define STATE_READ_FILE 2

#define OSCAR_HDR_SIZE 240

#define READ_BUFF_SIZE 128

struct ArchiveFile
{
    struct oscar_hdr hdr;
    char *file_data;
    int file_size;
};

struct Archive
{
    struct ArchiveFile *files;
    int size_files; /* capacity of 'files' array */
    int num_files; /* number of ArchiveFiles in array */
    char archive_name[32];
};

/* expand the array of files in an archive structure if we reach the end of 'files' */ 
int __expand_archive(struct Archive *archive)
{
    /* copy the files array */
    struct ArchiveFile *new_array = malloc(sizeof(struct ArchiveFile) * archive->size_files * 2);
    if(!new_array)
        return -1;
    
    memcpy(new_array, archive->files, sizeof(struct ArchiveFile) * archive->num_files);
    archive->size_files *= 2;
    
    /* delete the old array */
    free(archive->files);
    archive->files = new_array;
    return 0;
}

int read_line(int fd, char **line_ptr, size_t *n)
{
    int read_buff_size = 128;
    char *buff = malloc(read_buff_size);
    char *tmp = NULL;
    int res = 0;
    int write_pos = 0;
    int start = 0;

    while((res = read(fd, buff, read_buff_size)) == 0)
    {   
        bytes_read = strlen(buff);
        start = write_pos;
        for (; write_pos < bytes_read; write_pos++)
        {
            if(buff[write_pos] == '\n')
            {
                *n = write_pos - start;
                strncpy(*line_ptr, buff, *n);
                return 0;
            }
            else if(buff[write_pos] == '\0')
            {
                *n = write_pos - start;
                strncpy(*line_ptr, buff, *n);
                return 1;  
            }


        }

        
        if(strlen(buff) < read_buff_size)
        {
            *n = 
            break;
        }

        /* expand the size of 'buff' and copy the current contents over to the new larger buffer */
        /* this covers the case where a line in the file could be ridiculously long */
        read_buff_size *= 2;
        tmp = malloc(read_buff_size);
        memcpy(tmp, buff, read_buff_size/2);
        buff = tmp;
        tmp = NULL;
    }

    if(res == -1)
    {
        printf("read() failed \n");
        free(buff);
        return -1;
    }

    *line_ptr = buff;
    return 0;
}

int write_archive(char *file_name, struct Archive *archive)
{
    
}

int read_archive(int fd, struct Archive **archive)
{
    if(!fd || !archive)
        return -1;

    int state = STATE_READ_ARCHIVE_HEADER;
    
    int res = -1;
    char *line = NULL;
    size_t len = 0;
    FILE *fp = NULL;
    struct ArchiveFile *current_file = NULL;

    fp = fdopen(fd, "r");
    if(fp == NULL)
    {
        printf("\nfdopen() failed\n");
        return -1;
    }
    
    (*archive) = malloc(sizeof(struct Archive));
    (*archive)->num_files = 0;
    (*archive)->size_files = 10;
    (*archive)->files = malloc(sizeof(struct ArchiveFile) * 10);
    
    strcpy((*archive)->archive_name,"test");

    while((res = getline(&line, &len, fp)) != 0)
    {
        if(state == STATE_READ_ARCHIVE_HEADER)
        {
            //try and read out archive header and fail if it's not there
            if(strcmp(line, OSCAR_ID) != 0)
            {
                printf("\ninvalid archive header\n");
                return -1;
            }
            state = STATE_READ_FILE_HEADER;
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

 
            current_file = &((*archive)->files[(*archive)->num_files]);
            memcpy(&(current_file->hdr), line, OSCAR_HDR_SIZE);
    
            (*archive)->num_files++;
            
            int i_size = atoi(current_file->hdr.oscar_size);
            current_file->file_data = malloc(i_size);
            current_file->file_size = i_size;

            state = STATE_READ_FILE;
        }
        else if(state == STATE_READ_FILE)
        {
            //read out file data until the end of the 'file' is reached
            if(read(fd, current_file->file_data, current_file->file_size) != 0)
            {
                printf("\nreading file data failed for %s\n", current_file->hdr.oscar_name);
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

int open_archive(char *file_name, struct Archive *out_archive)
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
    char *line;
    int len;
    int res;
    while((res = read_line(fd, &line, &len)) != -1)
    {
        if(res == 1)
            break;

        printf(line);
        printf("\n");
        free(line);
    }

    struct Archive *archive = NULL;
    if(read_archive(fd, &archive) != -1)
    {
        return -1;
    }

	/* if it doesn't exist create the file */

	return 0;
}

