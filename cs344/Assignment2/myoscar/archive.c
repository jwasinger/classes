#include "archive.h"

#define STATE_READ_ARCHIVE_HEADER 0
#define STATE_READ_FILE_HEADER 1
#define STATE_READ_FILE 2

#define OSCAR_HDR_SIZE 123

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

int read_line(char **line_ptr, int *n, int fd)
{
    //read out in increments of READ_BUFF_SIZE
    //if a '\n' is read, copy the contents of the buffer into line_ptr
    //then lseek back to the character right after the \n
    int res = 0;
    int write_pos = 0; //write position within line_ptr
    char tmp_buff[READ_BUFF_SIZE]; //temporary buffer to read portions of the line into
    int i = 0;
    int read_pos = 0;
    char *error_str = NULL;
    char *out_buff = malloc(READ_BUFF_SIZE);
    int out_buff_cap = READ_BUFF_SIZE;
    
    //while(haven't read to the end of the file || haven't read to a \n)
    while((res = read(fd, tmp_buff, READ_BUFF_SIZE)) != 0)
    {
        if(res == -1)
        {
            printf("file read error: %s\n", strerror(errno));
            return -1;
        }
            
        for(; i < res; i++)
        {
            if(tmp_buff[i] == '\n')
            {        
                memcpy(out_buff+write_pos, tmp_buff, i+1);// copy the buffer to the index where the \n is
                *n = i + 1; // is this correct?
                (*line_ptr) = out_buff; 
                write_pos += i + 1;
                
                off_t cur_offset = lseek(fd, 0, SEEK_CUR);
                lseek(fd, cur_offset - (res - (i+1)), SEEK_SET);// lseek to the char right after the \n ( (size of buffer-1) - index of \n)
 
                return 0;
            }
        }
        
        if(res < READ_BUFF_SIZE)
        {
            //end of file has been reached, copy the contents of the buffer to line_ptr
            memcpy(out_buff+write_pos, tmp_buff, res);
            *n = write_pos + i + 1; // is this correct?
            return 1;
        }

        //text has been read but still on same line.  Copy all characters from the tmp_buff and advance write_pos
        memcpy(out_buff+write_pos, tmp_buff, i+1);

        write_pos += i + 1;
      
        if(write_pos == out_buff_cap)
        {
            out_buff_cap *= 2;
            char *tmp = malloc(out_buff_cap);
            memcpy(tmp, out_buff, sizeof(out_buff));
            free(out_buff);
            out_buff = tmp;
        }

        i = 0;
    }
    
    if(res  < READ_BUFF_SIZE)
        return 1;    

    return 0;
}

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
    
    (*archive) = malloc(sizeof(struct Archive));
    (*archive)->num_files = 0;
    (*archive)->size_files = 10;
    (*archive)->files = malloc(sizeof(struct ArchiveFile) * 10);
    
    strcpy((*archive)->archive_name,"test");

    //while((res = read_line(&line, &len, fd)) != 1)
    while(1)
    {
        //seek forward in the file when bytes are read 
        //lseek(fd, len, SEEK_CUR);

        if(state == STATE_READ_ARCHIVE_HEADER)
        {
            res = read_line(&line, &len, fd);
            if(res == 1)
            {
                printf("unexpected EOF encountered\n");
                return -1;
            }
            else if (res == -1)
            {
                printf("file read failure\n");
                return -1;
            }

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
            res = read_line(&line, &len, fd);
            if(res == 1)
            {
                break;
            }
            else if (res == -1)
            {
                printf("\nfile read failure\n");
                return -1;
            }
 
            //try and read out file header and fail if it's not there
            if(len != OSCAR_HDR_SIZE)
            {
                printf("\nInvalid file header size: %d\n", len);
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

            res = read(fd, current_file->file_data, current_file->file_size);
            //read out file data until the end of the 'file' is reached
            if(res == -1)
            {
                printf("\nreading file data failed for %s\n", current_file->hdr.oscar_name);
                return -1;
            }
            else if( res == 0)
            {
               printf("\nunexpected EOF for %s\n", current_file->hdr.oscar_name);
               return -1;
            }
            if(strlen(current_file->file_data) != current_file->file_size)
            {
                printf("\ninvalid file size\n");
                return -1;
            }

            //fseek(fp, current_file->file_size, SEEK_CUR);
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
    
    struct Archive *archive = NULL;
    if(read_archive(fd, &archive) == -1)
    {
        return -1;
    }

	return 0;
}

