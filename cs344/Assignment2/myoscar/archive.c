#include "archive.h"


#define STATE_READ_ARCHIVE_HEADER 0
#define STATE_READ_FILE_HEADER 1
#define STATE_READ_FILE 2

#define OSCAR_HDR_SIZE 123

#define READ_BUFF_SIZE 128

/* http://stackoverflow.com/questions/1114741/how-to-convert-int-to-char-c */
char *itoa(int n, int out_size)
{
    char *output = malloc(sizeof(char) * out_size);
    sprintf(output, "%d", n);
    return output;
}

/* generate oscar file header data from a file */
int __create_oscar_hdr(int fd, char *file_name, struct oscar_hdr *hdr_out)
{
    struct stat st; 
    char *file_size = NULL;
    char *name_len = NULL;
    char *adate = NULL;
    char *mdate = NULL;
    char *uid = NULL;
    char *gid = NULL;
    char *mode = NULL;
    char deleted;
    char sha1[OSCAR_SHA_DIGEST_LEN];
    int res = 0;
    int error = 0;

    res = fstat(fd, &st);
    if(res == -1)
    {
        error = errno;
        printf("Error fstat'ing file descriptor  %d:%s", fd, strerror(errno));
        return -1;
    }

    file_size = itoa(st.st_size, OSCAR_FILE_SIZE+1);
    name_len = itoa(strlen(file_name), 2+1);
    //adate = itoa(st.st_birthtime, OSCAR_DATE_SIZE);
    mdate = itoa(st.st_mtime, OSCAR_DATE_SIZE);
    uid = itoa(st.st_uid, OSCAR_UGID_SIZE);
    gid = itoa(st.st_gid, OSCAR_UGID_SIZE);
    mode = itoa(st.st_mode, OSCAR_MODE_SIZE);


    strcpy(hdr_out->oscar_name, file_name);
    strncpy(hdr_out->oscar_name_len, name_len, 2);
    //strcpy(hdr->oscar_cdate, st.st_birthtime); //does st.st_birthtime exist?
    //strcpy(hdr->oscar_adate, st.);
    strcpy(hdr_out->oscar_mdate, mdate);
    strcpy(hdr_out->oscar_uid, uid); // st.st_uid user_id
    strcpy(hdr_out->oscar_gid, gid); // st.st_gid group id
    strcpy(hdr_out->oscar_mode, mode); // st.st_mode file mode
    strcpy(hdr_out->oscar_size, file_size); 
    hdr_out->oscar_deleted = ' ';
    strcpy(hdr_out->oscar_hdr_end, OSCAR_HDR_END);

    free(uid);
    free(gid);
    free(adate);
    free(mdate);
    free(mode);
    free(file_size);
    free(name_len);

    return 0;
}

int __read_line(char **line_ptr, int *n, int fd)
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

void free_archive(struct Archive **archive)
{
    int i = 0;
    if(!(*archive))
        return;

    free((*archive)->files);
    (*archive) = NULL;
}

int __read_archive(int fd, char *file_name, struct Archive **archive)
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
    
    strcpy((*archive)->archive_name, file_name);

    //while((res = __read_line(&line, &len, fd)) != 1)
    while(1)
    {
        //seek forward in the file when bytes are read 
        //lseek(fd, len, SEEK_CUR);

        if(state == STATE_READ_ARCHIVE_HEADER)
        {
            res = __read_line(&line, &len, fd);
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
            res = __read_line(&line, &len, fd);
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

int __write_file(int fd, const struct ArchiveFile *file)
{
    int res = -1;

    res = write(fd, (void *)&(file->hdr), OSCAR_HDR_SIZE);
    if(res != OSCAR_HDR_SIZE)
    {
        printf("less bytes written than expected while writing file header\n");
        return -1;
    }
    
    res = write(fd, (void *)file->file_data, file->file_size);
    if(res != file->file_size)
    {
        printf("less bytes written than expected while writing file data\n");
        return -1;
    }
    
    return 0;
}

int archive_contains_file(char *file_name, const struct Archive *archive)
{
    int fd = 0;
    int i = 0;
    int res = 0;
    struct oscar_hdr hdr;
    char *file_data;
    int error = 0;

    fd = open(file_name, O_RDONLY, 0);
    if(fd == -1)
    {
        error = errno;
        printf("attempt to open file '%s' failed: %s\n", file_name, strerror(error)); 
        return -1;    
    }
    
    res = __create_oscar_hdr(fd, file_name, &hdr);
    if(res == -1)
    {
        return -1;    
    }
    
    for(; i < archive->num_files; i++)
    {
        if(strcmp(hdr.oscar_name, file_name) == 0)
            return 1;    
    }

    return -1;
}



int write_archive(char *file_name, const struct Archive *archive)
{
    int fd = -1;
    int res = -1;
    char *oscar_hdr_str = OSCAR_ID;
    int i = 0;

    res = open(file_name, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);

    if(res == -1)
    {
        printf("Error opening file: %s\n", strerror(errno));
        return -1;
    }
    else
    {
        fd = res;
    }

    /* write the archive header */
    res = write(fd, (void *)oscar_hdr_str, OSCAR_ID_LEN); 
    if(res < OSCAR_ID_LEN)
    {
        printf("Less bytes written than attempted\n");
        printf("Error(?): %s\n", strerror(errno));
        return -1;
    }

    /* write each of the archives files */
    for(; i < archive->num_files; i++)
    {
        if(__write_file(fd, &(archive->files[i])) == -1)
            return -1;
    }

    return 0;
}

struct Archive *__create_archive(char *archive_name)
{
    struct Archive *arc = malloc(sizeof(struct Archive));
    arc->size_files = 10;
    arc->num_files = 0;
    strcpy(arc->archive_name, archive_name);
    arc->files = malloc(sizeof(struct ArchiveFile) * 10);
    return arc;
}

int open_archive(char *file_name, struct Archive **out_archive, int create)
{
	int fd = -1;
	
	/* try and open the file 'file_name' */
	fd = open(file_name, O_RDWR, 0);
	if(fd == -1)
	{
		if(errno == ENOENT)
		{
			if(create == 1)
            {
                /* file doesn't exist, create it */
                printf("creating file '%s'\n", file_name);
                
                fd = open(file_name, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);
                if(fd == -1)
                {
                    error_exit();	
                    return -1;
                }
                
                //fill out an empty archive struct and exit
                (*out_archive) = __create_archive(file_name);
                return 0;

            }
            else
            {
                printf("file %s doesn't exist and create != 1\n", file_name);
                return 1; // special return value 
            }
		}
	}
	
	/* if it exists see if it is a valid archive and return -1 if not */ 
    if(__read_archive(fd, file_name, out_archive) == -1)
    {
        return -1;
    }

    close(fd);
	return 0;
}

int __archive_add_file(struct Archive *archive, struct ArchiveFile *archive_file)
{
    int placement_index = archive->num_files;

    if(archive->num_files == archive->size_files)
    {
        if(__expand_archive(archive) == -1)
            return -1;       
    }

    archive->files[placement_index] = *archive_file;
}

/* create and populate the fields of an ArchiveFile struct */
/* return -1 if error, or the value of the file descriptor for the created file if success*/
int __populate_arc_file_struct(char *file_name, struct ArchiveFile** out_archive_file)
{
    struct oscar_hdr hdr; 
    int res = 0;
    int fd = 0;
    int error = 0;
    
    *out_archive_file = malloc(sizeof(struct ArchiveFile));
    fd = open(file_name, O_RDONLY, 0);
    if(fd == -1)
    {
        error = errno;
        printf("Error opening file %s:%s", file_name, strerror(errno));
        return -1;
    }
    
    res = __create_oscar_hdr(fd, file_name, &hdr);
    if (res == -1)
    {
        return -1;
    }

    //write the header to the disk
    res = write(fd, (void *)&hdr, sizeof(struct oscar_hdr));
    if(res == -1)
    {
        error = errno;
        printf("Error writing oscar header to %s: %s\n", file_name, strerror(error));
        return -1;
    }

    return fd;
}

int archive_add_files(struct Archive *archive, char **file_names, int num_files)
{
    int i = 0;
    int cmp = 0;
    int res = 0;
    struct ArchiveFile *arc_file = NULL;

    for(; i < num_files; i++)
    {
        cmp = archive_contains_file(file_names[i], archive); 
        if(cmp == -1)
            return -1;    
        else if(cmp == 0)
        {
            res = __populate_arc_file_struct(file_names[i], &arc_file);
            if(res == -1)
                return -1;

            res = __archive_add_file(archive, arc_file);
            if(res == -1)
                return -1;
        }
        //else if(cmp == 1) print something in verbose mode here...
    }
}

int archive_del_files(struct Archive *archive, char *file_names, int num_files)
{
    
}

int archive_add_reg_files(struct Archive *archive)
{
    
}
