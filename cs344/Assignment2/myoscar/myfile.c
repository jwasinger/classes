#include "myfile.h"
/* http://www.thegeekstuff.com/2012/06/c-directory/ */


int __load_file(char *file_name, char **file_data, int *size)
{
    struct stat st;
    int res = -1;
    int fd = -1;
    //fix the size shit
    res = open(file_name, O_RDONLY);
    if(res == -1)
    {
        printf("Error opening file %s\n", file_name);
        return -1;
    }
    fd = res;

    res = fstat(fd, &st);
    if(res == -1)
    {
        printf("fstat error on file %s: %s\n", file_name, strerror(errno));
        return -1;
    }

    size = st.st_size;
    (*file_data) = malloc(size);

    res = read(fd, (void *)(*file_data), size);
    if(res <= 0)
    {
        printf("error reading file: %s\n", file_name);
        return -1;
    }

    return 0;
}

int __get_pwd_files(char ***file_names, int *num_files)
{
    int file_names_cap = 2;
    int cur_num_files = 0;
    int res = -1;
    DIR *dp = NULL;
    char *cwd = NULL;
    struct dirent *entry;
    int error = 0;
    int i = 0;
    (*file_names) = malloc(sizeof(char *) * file_names_cap);

    /* open cwd dir */
    cwd = get_current_dir_name();
    dp = opendir(cwd);
    if(dp == NULL)
    {
        printf("Error opening directory %s: %s", cwd, strerror(errno));
        return -1;
    }
    
    while(1)
    {
        entry = readdir(dp);
        if(entry == NULL)
        {
            if(errno != 0)
            {
                error = errno;
                printf("Error reading directory %s: %s\n", cwd, strerror(error));
                
                for(i = 0; i < cur_num_files; i++)
                {
                    free((*file_names)[i]);
                }

                free(*file_names);
                return -1;
            }
            else
            {
                *num_files = cur_num_files;
                return 0;
            }
        }
        
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        //add the file name to file_names and enlarge the array if necessary
        if(cur_num_files == file_names_cap)
        {
            file_names_cap *= 2;
            char **tmp = malloc(sizeof(char *) * file_names_cap);
            for(i = 0; i < file_names_cap/2; i++)
            {
                tmp[i] = malloc(sizeof(char) * strlen((*file_names)[i]));
                strcpy(tmp[i], (*file_names)[i]);
            }
            
            for(i = 0; i < file_names_cap/2; i++)
                free((*file_names)[i]);
            
            free(*file_names);
            (*file_names) = tmp;
        }
        
        (*file_names)[cur_num_files] = malloc(sizeof(char) * strlen(entry->d_name));
        
        strcpy((*file_names)[cur_num_files], entry->d_name);
        cur_num_files++;
    }
}

int __get_pwd_reg_files(char ***file_names, int *num_files)
{
    int file_names_cap = 2;
    int cur_num_files = 0;
    int res = -1;
    DIR *dp = NULL;
    char *cwd = NULL;
    struct dirent *entry;
    int error = 0;
    int i = 0;
    (*file_names) = malloc(sizeof(char *) * file_names_cap);

    /* open cwd dir */
    cwd = get_current_dir_name();
    dp = opendir(cwd);
    if(dp == NULL)
    {
        printf("Error opening directory %s: %s", cwd, strerror(errno));
        return -1;
    }
    
    while(1)
    {
        entry = readdir(dp);
        if(entry == NULL)
        {
            if(errno != 0)
            {
                error = errno;
                printf("Error reading directory %s: %s\n", cwd, strerror(error));
                
                for(i = 0; i < cur_num_files; i++)
                {
                    free((*file_names)[i]);
                }

                free(*file_names);
                return -1;
            }
            else
            {
                *num_files = cur_num_files;
                return 0;
            }
        }
        
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || !(entry->d_type & DT_REG))
            continue;
        
        //add the file name to file_names and enlarge the array if necessary
        if(cur_num_files == file_names_cap)
        {
            file_names_cap *= 2;
            char **tmp = malloc(sizeof(char *) * file_names_cap);
            for(i = 0; i < file_names_cap/2; i++)
            {
                tmp[i] = malloc(sizeof(char) * strlen((*file_names)[i]));
                strcpy(tmp[i], (*file_names)[i]);
            }
            
            for(i = 0; i < file_names_cap/2; i++)
                free((*file_names)[i]);
            
            free(*file_names);
            (*file_names) = tmp;
        }
        
        (*file_names)[cur_num_files] = malloc(sizeof(char) * strlen(entry->d_name));
        
        strcpy((*file_names)[cur_num_files], entry->d_name);
        cur_num_files++;
    }
}

void __free_arrays(char **file_data, char **file_names, int *sizes, int num)
{
    int i = 0;
    
    for(; i < num; i++)
    {
        free(file_data[i]);
        free(file_names[i]);
    }
    free(sizes);
}

int load_files(char **file_names, int num_files, char ***files_data, int **sizes)
{
    int i = 0;
    int res = -1;

    (*files_data) = malloc(sizeof(char *) * num_files);
    (*sizes) = malloc(sizeof(int) * num_files);

    for(; i < num_files; i++)
    {
        res = __load_file(file_names[i], (*files_data)[i], sizes[i]);        
        if(res == -1)
        {
            free(*files_data);
            free(*sizes);
            return -1;
        }
    }

    return 0;
}

int load_pwd_files(char ***files_data_out, int **sizes_out, char ***file_names_out, int *num_files_out)
{
    int num_files = 0;    
    int res = 0;
    int i = 0;
    
    res = __get_pwd_files(file_names_out, &num_files);
    if(res == -1)
        return -1;
    
    (*files_data_out) = malloc(sizeof(char *) * num_files);
    (*sizes_out) = malloc(sizeof(int) * num_files);

    for(; i < num_files; i++)
    {
        res = __load_file(&file_names_out[i], &((*files_data_out)[i]), (*sizes_out)[i]);
        if(res == -1)
        {
            __free_arrays(*(files_data_out), *(file_names_out), *sizes_out, i-1); 
            return -1;
        }
    }
    
    *num_files_out = num_files;
    return 0;
}

int load_pwd_reg_files(char ***files_data_out, int **sizes_out, char ***file_names_out, int *num_files_out)
{
    int num_files = 0;    
    int res = 0;
    int i = 0;
    
    res = __get_pwd_reg_files(file_names_out, &num_files);
    if(res == -1)
        return -1;
    
    (*files_data_out) = malloc(sizeof(char *) * num_files);
    (*sizes_out) = malloc(sizeof(int) * num_files);

    for(; i < num_files; i++)
    {
        res = __load_file(&file_names_out[i], &((*files_data_out)[i]), (*sizes_out)[i]);
        if(res == -1)
        {
            __free_arrays(*(files_data_out), *(file_names_out), *sizes_out, i-1); 
            return -1;
        }
    }

    *num_files_out = num_files;
    return 0;
}
