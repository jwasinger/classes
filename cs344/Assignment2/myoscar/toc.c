#include "toc.h"
/*tlpi page 281 */

char *__get_mode_str(mode_t mode)
{
    char *result = malloc(sizeof(char) * 11);
    int i = 0; 
    
    memset(result, 0, sizeof(char) * 10);

    //populate first bit with file type
    if(S_ISREG(mode))
    {
        result[0] = '-';
    }
    else if(S_ISDIR(mode))
    {
        result[0] = 'd';
    }
    else if(S_ISCHR(mode))
    {
        result[0] = 'c';
    }
    else if(S_ISBLK(mode))
    {
        result[0] = 'b';
    }
    else if(S_ISFIFO(mode))
    {
        result[0] = 'p';
    }
    else if(S_ISLNK(mode))
    {
        result[0] = 'l';
    }
    else if(S_ISSOCK(mode))
    {
        result[0] = 's';
    }

    //determine which permission bits are set on user/group/other fields 

    if(mode & S_IRUSR)
    {
        result[1] = 'r';
    }
    if(mode & S_IWUSR)
    {
        result[2] = 'w';
    }
    if(mode & S_IXUSR)
    {
        result[3] = 'x';
    }
    if(mode & S_IRGRP)
    {
        result[4] = 'r';
    }
    if(mode & S_IWGRP)
    {
        result[5] = 'w';
    }
    if(mode & S_IXGRP)
    {
        result[6] = 'x';
    }
    if(mode & S_IROTH)
    {
        result[7] = 'r';
    }
    if(mode & S_IWOTH)
    {
        result[8] = 'w';
    }
    if(mode & S_IXOTH)
    {
        result[9] = 'x';
    }
    
    for(i=0; i < 10; i++)
    {
        if(result[i] == 0)
        {
            result[i] = '-';
        }
    }

    result[10] = '\0';
    return result;
}

char *__get_time(time_t *t)
{

    struct tm now;
    localtime_r(t, &now);
    
    char *output = malloc(sizeof(char) * 32);
    strftime(output, 32, "%a %b %d %k:%M:%S %Y", &now);
    return output;
}

int get_gr_name(int gid, char **output)
{
    struct group grp;
    struct group *grp_ptr = &grp;
    struct group *tmp_grp_ptr = NULL;
    char buffer[256];
    size_t buf_size = 256;
    int error = 0;

    if((getgrgid_r(gid, grp_ptr, buffer, buf_size, &tmp_grp_ptr)) != NULL)
    {
        error = errno;
        printf("Errror getting group with gid(%d): %s\n", gid, error);
        return -1;
    }
    
    (*output) = malloc(sizeof(char) * 64);
    strcpy((*output), grp.gr_name);
    return 0;
}

int get_usr_name(int uid, char **output)
{
    struct passwd pwd;
    struct passwd *pwd_ptr = &pwd;
    struct passwd *result = NULL;
    char buf[256];
    size_t bufsize = 256;
    int s;

    s = getpwuid_r(uid, pwd_ptr, buf, bufsize, &result);
    if(result == NULL)
        return s;
    
    (*output) = malloc(sizeof(char) * 64);
    strcpy((*output), result->pw_name);
    return 0;
}

void disp_archive_toc(struct Archive *archive)
{
    int i = 0;
    char file_name[OSCAR_MAX_FILE_NAME_LEN+1]; 
    printf("Short table of contents for oscar archive file: %s\n", archive->archive_name);

    for(i = 0; i < archive->num_files; i++)
    {
        strncpy(file_name, archive->files[i].hdr.oscar_name, OSCAR_MAX_FILE_NAME_LEN);
        file_name[OSCAR_MAX_FILE_NAME_LEN] = '\0';
        printf("  %s\n", file_name);
    }
}

void disp_archive_long_toc(struct Archive *archive)
{
    int i = 0;
    int j = 0;
    int res = 0;
    int mode = 0;
    char *perms = NULL;
    char perms_num[5];
    char *owner_name = NULL;
    char *grp_name = NULL;
    char *create_date = NULL;
    char *access_date = NULL;
    char *modify_date = NULL;
    char *deleted = NULL;
    char *p_end = NULL;
    char oscar_size[OSCAR_FILE_SIZE];

    int gid = 0;
    int uid = 0;
    long int adate = 0;
    long int mdate = 0;
    long int cdate = 0;

    printf("Long table of contents for oscar archive file: %s\n", archive->archive_name);
    
    for(i = 0; i < archive->num_files; i++)
    {
        //perms = __get_mode_str(archive->files[i].hdr.oscar_mode);
        mode = strtol(archive->files[i].hdr.oscar_mode, &p_end, 8);
        perms = __get_mode_str(mode);

        memcpy(&perms_num[1], &archive->files[i].hdr.oscar_mode[3], 3);
        perms_num[0] = '0';
        perms_num[4] = '\0';
        
        uid = atoi(archive->files[i].hdr.oscar_uid);
        gid = atoi(archive->files[i].hdr.oscar_gid);
        
        res = get_gr_name(gid, &grp_name);
        if(grp_name == NULL)
        {
            printf("ERROR(%d): %s\n", res, strerror(errno));
            return;
        }
        res = get_usr_name(uid, &owner_name);
        if(owner_name == NULL)
        {
            printf("ERROR(%d): %s\n", res, strerror(errno));
            return;
        }
        
        //owner_name = getpwuid(uid)->pw_name;
        
        adate = atol(archive->files[i].hdr.oscar_adate);
        mdate = atol(archive->files[i].hdr.oscar_mdate);
        cdate = atol(archive->files[i].hdr.oscar_cdate);

        access_date = __get_time(&adate);
        modify_date = __get_time(&mdate);
        create_date = __get_time(&cdate);
        
        deleted = (archive->files[i].hdr.oscar_deleted == 'y')? "yes" : "no"; 
        
        strncpy(oscar_size, archive->files[i].hdr.oscar_size, OSCAR_FILE_SIZE);
        for(j = 0; j < OSCAR_FILE_SIZE; j++)
        {
            if(oscar_size[j] == 32)
            {
                oscar_size[j] = '\0';
                break;
            }
        }
        
        printf("  File name: %.*s\n", OSCAR_MAX_FILE_NAME_LEN, archive->files[i].hdr.oscar_name);
        printf("\t File size:   %s bytes\n", oscar_size);
        printf("\t Permissions: %s (%s)\n", perms, perms_num);
        printf("\t File owner:  %s (uid: %d)\n", owner_name, uid);
        printf("\t File group:  %s (gid: %d)\n", grp_name, gid);
        printf("\t Create date: %s\n", create_date);
        printf("\t Access date: %s\n", access_date);
        printf("\t Modify date: %s\n", modify_date);
        printf("\t Marked deleted: %s\n", deleted);

        free(perms);
        free(access_date);
        free(modify_date);
        free(create_date);
        free(grp_name);
        free(owner_name);
    }
}
