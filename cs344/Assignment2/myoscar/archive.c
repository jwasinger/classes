#include "archive.h"


#define STATE_READ_ARCHIVE_HEADER 0
#define STATE_READ_FILE_HEADER 1
#define STATE_READ_FILE 2

#define OSCAR_HDR_SIZE 123

#define READ_BUFF_SIZE 128

void insert_spaces(char *str, int mem_size)
{
    int i = 0;
    int end_reached = 0;

    for(; i<mem_size; i++)
    {
        if(end_reached)
            str[i] = 32;
        else if (str[i] == '\0')
        {    
            end_reached = 1;
            str[i] = 32;
        }
    }
}

/* http://stackoverflow.com/questions/1114741/how-to-convert-int-to-char-c */
char *itoa(int n, int out_size)
{
    char *output = malloc(sizeof(char) * out_size);
    
    sprintf(output, "%d", n);
    insert_spaces(output, out_size);
    return output;
}

char *ltoa(long int n, int out_size)
{
    char *output = malloc(sizeof(char) * out_size);
    
    sprintf(output, "%d", n);
    insert_spaces(output, out_size);
    return output;
}

char *lltoa(long long int n, int out_size)
{
    char *output = malloc(sizeof(char) * out_size);
    
    sprintf(output, "%d", n);
    insert_spaces(output, out_size);
    return output;   
}

char *itoa_oct(int n, int out_size)
{
    char *output = malloc(sizeof(char) * out_size);

    sprintf(output, "%o", n);
    insert_spaces(output, out_size);
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
    char *cdate = NULL;
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

    file_size = lltoa(st.st_size, OSCAR_FILE_SIZE);
    name_len = itoa(strlen(file_name), 2);
    adate = ltoa(st.st_atime, OSCAR_DATE_SIZE);
    mdate = ltoa(st.st_mtime, OSCAR_DATE_SIZE);
    cdate = ltoa(st.st_ctime, OSCAR_DATE_SIZE);
    uid = itoa(st.st_uid, OSCAR_UGID_SIZE);
    gid = itoa(st.st_gid, OSCAR_UGID_SIZE);
    mode = itoa_oct(st.st_mode, OSCAR_MODE_SIZE);
    
    strcpy(hdr_out->oscar_name, file_name);
    strncpy(hdr_out->oscar_name_len, name_len, 2);
    strncpy(hdr_out->oscar_cdate, cdate, OSCAR_DATE_SIZE); //does st.st_birthtime exist?
    strncpy(hdr_out->oscar_adate, adate, OSCAR_DATE_SIZE); // THIS IS INCORRECT
    strncpy(hdr_out->oscar_mdate, mdate, OSCAR_DATE_SIZE);
    strncpy(hdr_out->oscar_uid, uid, OSCAR_UGID_SIZE); // st.st_uid user_id
    strncpy(hdr_out->oscar_gid, gid, OSCAR_UGID_SIZE); // st.st_gid group id
    strncpy(hdr_out->oscar_mode, mode, OSCAR_MODE_SIZE); // st.st_mode file mode
    strncpy(hdr_out->oscar_size, file_size, OSCAR_FILE_SIZE); 
    hdr_out->oscar_deleted = ' ';
    strcpy(hdr_out->oscar_sha1,"          ");
    strcpy(hdr_out->oscar_hdr_end, OSCAR_HDR_END);
    
    insert_spaces(hdr_out->oscar_name, OSCAR_MAX_FILE_NAME_LEN); 

    free(uid);
    free(gid);
    free(adate);
    free(mdate);
    free(cdate);
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

int __expand_array(void **array, int cur_size, int desired_size)
{
    if(desired_size < cur_size)
    {
        printf("desired size smaller than current size of array\n");
        return -1;
    }

    void *tmp = malloc(desired_size);
    memcpy(tmp, *array, cur_size);
    free(*array);
    *array = tmp;
    return 0;
}

/* expand the array of files in an archive structure if we reach the end of 'files' */ 
int __expand_archive(struct Archive *archive)
{
    /* copy the files array */
    //struct ArchiveFile *new_array = malloc(sizeof(struct ArchiveFile) * archive->size_files * 2);
    //if(!new_array)
    //    return -1;
    
    int res = __expand_array(&(archive->files), 
                             archive->size_files*sizeof(struct ArchiveFile), 
                             archive->size_files * 2 *sizeof(struct ArchiveFile));
    if(res == -1)
        return -1;

    //memcpy(new_array, archive->files, sizeof(struct ArchiveFile) * archive->num_files);
    archive->size_files *= 2;
    return 0;
}

void __free_archive_file(struct ArchiveFile *arc_file)
{
    free(arc_file->file_data);
    arc_file->file_data = NULL;
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
            else if( res == 0 && current_file->file_size != 0)
            {
               printf("\nunexpected EOF for %s\n", current_file->hdr.oscar_name);
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

int __write_oscar_hdr(int fd, const struct oscar_hdr *hdr, int *hdr_size_written)
{
    int res = -1;
    int error = 0;
    
    res = write(fd, hdr->oscar_name, OSCAR_MAX_FILE_NAME_LEN);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_name: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, hdr->oscar_name_len, 2);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_name_len: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;
    
    res = write(fd, hdr->oscar_cdate, OSCAR_DATE_SIZE);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_cdate: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, hdr->oscar_adate, OSCAR_DATE_SIZE);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_adate: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, hdr->oscar_mdate, OSCAR_DATE_SIZE);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_mdate: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, hdr->oscar_uid, OSCAR_UGID_SIZE);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_uid: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, hdr->oscar_gid, OSCAR_UGID_SIZE);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_gid: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, hdr->oscar_mode, OSCAR_MODE_SIZE);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_mode: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, hdr->oscar_size, OSCAR_FILE_SIZE);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_size: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, &(hdr->oscar_deleted), 1);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_deleted: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, hdr->oscar_sha1, OSCAR_SHA_DIGEST_LEN);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_sha1: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;

    res = write(fd, hdr->oscar_hdr_end, OSCAR_HDR_END_LEN);
    if(res == -1)
    {
        error = errno;    
        printf("error writing oscar_hdr_end: %s", strerror(error));
        return -1;
    }
    *hdr_size_written += res;
    
    return 0;
}

int __write_file(int fd, const struct ArchiveFile *file, int *out_size_written)
{
    int res = -1;
    int hdr_size_written = 0;

    res = __write_oscar_hdr(fd, &file->hdr, &hdr_size_written); 
    if(res == -1)
    {
        return -1;
    }

    res = write(fd, (void *)file->file_data, file->file_size);
    if(res != file->file_size)
    {
        printf("less bytes written than expected while writing file data\n");
        return -1;
    }
    
    *out_size_written = hdr_size_written + res;

    return 0;
}

int archive_contains_file(char *file_name, const struct Archive *archive, int **out_indices, int *out_num_indices)
{
    int i = 0;
    int res = 0;
    struct oscar_hdr hdr;
    char *file_data;
    int error = 0;
    int size_indices = 0;

    *out_indices = malloc(sizeof(int) * 10);
    *out_num_indices = 0;
    
    for(; i < archive->num_files; i++)
    {
        if(strstr(archive->files[i].hdr.oscar_name, file_name) != NULL)
        {    
            if(*out_num_indices == size_indices)
            {
                __expand_array(out_indices, size_indices, size_indices * 2);
                size_indices *= 2;
            }
            
            (*out_indices)[*out_num_indices] = i;
            (*out_num_indices)++;
        }
    }
    
    if(*out_num_indices > 0)
        return 1;
    return 0;
}

int write_archive(char *file_name, struct Archive *archive)
{
    int fd = -1;
    int res = -1;
    char *oscar_hdr_str = OSCAR_ID;
    int i = 0;
    char cur_oscar_hdr_str[OSCAR_ID_LEN];
    int size_written = 0;
    int file_size_written = 0;

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
    
    size_written += OSCAR_ID_LEN;

    /* write each of the archives files */
    for(; i < archive->num_files; i++)
    {
        if(__write_file(fd, &(archive->files[i]), &file_size_written) == -1)
            return -1;

       size_written += file_size_written; 
    }
    
    /* if there is still text left in the archive file, clear it */
    
    res = ftruncate(fd, size_written);
    if(res == -1)
    {
        printf("failed to truncate file %s.  Error: %s\n", archive->archive_name, strerror(errno));
        return -1;
    }
    
    close(fd);
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
    archive->num_files++;
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
     
    (*out_archive_file)->hdr = hdr;
    (*out_archive_file)->file_size = atoi(hdr.oscar_size);
    (*out_archive_file)->file_data = malloc(sizeof(char) * (*out_archive_file)->file_size);
    res = read(fd, (*out_archive_file)->file_data, (*out_archive_file)->file_size);
    if(res == -1)
    {
        error = errno;
        printf("failed to read file '%s': %s\n", file_name, strerror(errno));
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int archive_add_files(struct Archive *archive, char **files, int num_files)
{
    int i = 0;
    int cmp = 0;
    int res = 0;
    struct ArchiveFile *arc_file = NULL;
    int out_index = 0; 
    int size_written = 0;

    for(; i < num_files; i++)
    {
        res = __populate_arc_file_struct(files[i], &arc_file);
        if(res == -1)
            return -1;

        res = __archive_add_file(archive, arc_file);
        if(res == -1)
            return -1;
    }

    return 0;
}

int archive_cleanse(struct Archive *archive)
{
    int i = 0;
    int new_num_files = archive->num_files;

    for(i = 0; i < archive->num_files; i++)
    {
        if(archive->files[i].hdr.oscar_deleted == 'y')
        {
            __archive_delete_index(i, archive);
            new_num_files--;
        }
    }
    
    archive->num_files = new_num_files;
    return 0;
}

int archive_extract_member(char *file_name, const struct Archive *archive, int overwrite)
{
    int res = 0;
    int error = 0;
    int fd = 0;
    int i = 0;
    
    int *out_index;
    int num_indices = 0;

    if(!archive_contains_file(file_name, archive, &out_index, &num_indices))
    {
        printf("archive %s doesn't contain file %s\n", archive->archive_name, file_name);
        return -1;
    }
    
    if(overwrite)
    {
        res = remove(file_name);
        if(res == -1)
        {
            if(errno != ENOENT) 
            {
                error = errno;
                printf("File deletion error on file '%s': %s\n", file_name, strerror(error));
                return -1;
            }
        }
    }

    res = open(file_name, O_RDONLY, 0);
    if(res == -1)
    {
        if(errno != ENOENT)
        {
            error = errno;
            printf("File extraction error: %s\n", strerror(error));
            return -1;
        }
    }
    else if(!overwrite)
    {   
        return 1; //quit out and don't overwrite the file that currently exists
    }

    res = open(file_name, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);
    if(res == -1)
    {
        error = errno;
        printf("File extraction error: %s\n", strerror(error));
        return -1;
    }
    
    fd = res;
    
    for(i = 0; i < archive->num_files; i++)
    {
        if(strstr(archive->files[i].hdr.oscar_name, file_name) != NULL)
        {
            res = write(fd, archive->files[i].file_data, archive->files[i].file_size); 
            if(res < archive->files[i].file_size)
            {
                error = errno;
                printf("Error writing file extraction output to file %s: %s", file_name, strerror(error));
                return -1;
            }
            return 0;
        }
    }
    
    printf("this line should never have been reached\n");
    return -1;
    close(fd);
}

int archive_extract_member_cur_time(char *file_name, const struct Archive *archive, int overwrite)
{
    int res = 0;
    int file_index = 0;
    
    int *out_indices = NULL;
    int num_indices = 0;

    res = archive_contains_file(file_name, archive, &out_indices, &num_indices);
    if(res <= 0)
    {
        printf("file is not a part of archive.");    
        return -1;
    }

    if(archive_extract_member(file_name, archive, overwrite) == -1)
        return -1;
    
    struct utimbuf times;
    times.actime = atol(archive->files[file_index].hdr.oscar_adate);
    times.modtime = atol(archive->files[file_index].hdr.oscar_mdate);
    res = utime(file_name, &times);
    if(res == -1)
    {
        printf("failed to modify file dates\n");
        return -1;
    }
    return 0;
}

int __archive_delete_index(int index, struct Archive *archive)
{
    int i = 0;

    __free_archive_file(&(archive->files[index]));
    memmove(archive->files + index, archive->files + index + 1, sizeof(struct ArchiveFile) * (archive->num_files - (index + 1)));

    return 0;
}

int __archive_file_is_marked(struct ArchiveFile *arc_file)
{
    if(arc_file->hdr.oscar_deleted == 'y')
        return 1;
    else
        return 0;
}

int archive_delete_members(char **files, int num_files, struct Archive *archive)
{
    int res = 0;
    int size_written = 0;
    int fd = 0;
    int i = 0;
    int j = 0;

    int *file_indices = NULL; //memory leak
    int num_indices = 0;

    for(i = 0; i < num_files; i++)
    {
        res = archive_contains_file(files[i], archive, &file_indices, &num_indices);
        if(res == -1)
        {
            return -1;
        }
        else if (res == 0)
        {
            printf("file '%s' is not a member of archive '%s'\n", files[i], archive->archive_name);
            return -1;
        }
        
        if(!__archive_file_is_marked(archive->files + file_indices[0]))
        {    
            __archive_delete_index(file_indices[0], archive);
            archive->num_files--;
        }
    }

    res = remove(archive->archive_name);
    if(res == -1)
    {
        printf("error trying to remove file %s: %s\n", archive->archive_name, strerror(errno));
        return -1;
    }

    return 0;
}

int archive_mark_members(char **file_names, int num_files, struct Archive *archive)
{
    int i = 0;
    int file_index = 0;
    int res = 0;
    int j = 0;
    
    int num_indices = 0;
    int *indices = NULL;

    for(i = 0; i < num_files; i++)
    {
        res = archive_contains_file(file_names[i], archive, &indices, &num_indices);
        if(res == -1)
        {
            return -1;
        }
        if(res == 0)
        {
            printf("file '%s' is not a part of the archive...\n", file_names[i]);
        }
        if(res == 1)
        {
            for(j=0; j<num_indices; j++)
            {
                if(archive->files[indices[j]].hdr.oscar_deleted == 'y')
                {
                    printf("file %s is already marked...\n", archive->files[file_index].hdr.oscar_name);
                    continue;
                }
                
                archive->files[indices[j]].hdr.oscar_deleted = 'y';
            }
        }
    }

    return 0;
}


int archive_unmark_members(char **file_names, int num_files, struct Archive *archive)
{
    int i = 0;
    int res = 0;
    int j = 0;

    int num_indices = 0;
    int *file_indices = NULL;

    for(i = 0; i < num_files; i++)
    {
        res = archive_contains_file(file_names[i], archive, &file_indices, &num_indices);
        if(res == -1)
        {
            return -1;
        }
        if(res == 0)
        {
            printf("file '%s' is not a part of the archive...\n", file_names[i]);
        }
        if(res == 1)
        {
            for(j = 0; j < num_indices; j++)
            {
                if(archive->files[file_indices[j]].hdr.oscar_deleted == ' ')
                {
                    printf("file %s is not marked...\n", archive->files[file_indices[j]].hdr.oscar_name);
                    continue;
                }
                
                archive->files[file_indices[j]].hdr.oscar_deleted = ' ';
            }
        }
    }

    return 0;
}
