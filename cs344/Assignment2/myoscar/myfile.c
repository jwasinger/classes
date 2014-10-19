
/* http://stackoverflow.com/questions/3501338/c-read-file-line-by-line */

#include "myfile.h"
#include <stdio.h>

int __get_num_lines(FILE *fp, int *num_lines_out)
{
	if(!fp)
	{
		return -1;
	}

	char *line = NULL;
	size_t len = 0;
	int read = -1;
	while((read = getline(&line, &len, fp)) != -1)
	{
		(*num_lines_out)++;
	}
	
	/* return file seek position to origin so it can be read again */
	if(fseek(fp, 0, SEEK_SET) == -1)
	{
		return -1;
	}
	return 0;
}

/* retrieve all lines in a file specified by 'fd'. The return value is a 
   pointer to dynamically allocated 2D array of the lines */
int get_file_lines(int fd, char ***lines_out, int *num_lines)
{
	FILE *fp = NULL;
	char *line = NULL;
	size_t len = 0;
	int i;

	fp = fdopen(fd, "r");
	if(fp == NULL)
	{
		return -1;
	}
	
	if(__get_num_lines(fp, num_lines) == -1)
	{
		return -1;
	}

	(*lines_out) = malloc(sizeof(char *) * (*num_lines));
	int *x = malloc(sizeof(int) * 5);
	for(i = 0; i < (*num_lines); i++)
	{
		int read = getline(&line, &len, fp);
		if(read == -1)
			break;

		/* copy string from the local variable line, to its appropriate place in line_lengths_out*/
		(*lines_out)[i] = malloc(sizeof(char) * (len+1));
		memcpy((*lines_out)[i], line, sizeof(char) * len);
		(*lines_out)[i][len] = '\0';
	}

	return 0;
}

int get_file_lines(int fd, char ***lines_out, int first_line, int last_line)
{
	
}

/* retrieve one line from a file */
int get_file_line(int fd, int line_num, char **line)
{
	return -1;	
}

int remove_file_line(int fd, int line_num)
{
	return -1;	
}
