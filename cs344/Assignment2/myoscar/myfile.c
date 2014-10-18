#include "myfile.h"


int __get_num_lines(int fd, int *num_lines_out)
{
	int numread = 0;
	char buff[256];
	
	while((numread = read(fd, (void *)buff, 256)) > 0)
	{
		for(int i = 0; i < numread; i++)
		{
			if (buff[i] == '\n')
			{
				(*num_lines_out)++;
			}
		}
	}

	if(numread == -1)
	{
		return -1;
	}

	return 0;
}

/* retrieve all lines in a file specified by 'fd'. The return value is a 
   pointer to dynamically allocated 2D array of the lines */
int get_file_lines(int fd, char ***lines_out)
{
	int num_lines = 0;
	if(!lines_out)
	{
		printf("invalid arg lines_out");
		return -1;
	}
	
	num_lines = __get_num_lines(fd, &num_lines);
	if(num_lines == -1)
	{
		return -1;
	}
	
	char **lines = (char **)malloc(sizeof(char *) * num_lines);
	if(!lines)
	{
		return -1;
	}

	char curr_line[MAX_LINE_LENGTH];
	char buff[MAX_LINE_LENGTH];
	short write_pos = 0;
	
	for( int i = 0; i < num_lines, i++ )
	{
		int num_read = 0;
		int num_to_read = 0;
		while(true)
		{
			num_read = read(fd, buff, MAX_LINE_LENGTH - num_to_read);
			if(num_read == -1)
			{	
				return -1;
			}
			
			for(int j = 0; j < num_read; j++)
			{
				if
			}

			num_to_read = MAX_LINE_LENGTH - num_read;
			if(num_to_read == 0)
			{
				
				break;
			}
		}
	}
}

/* retrieve one line from a file */
int get_file_line(int fd, int line_num, char **line)
{
	
}

int remove_file_line(int fd, int line_num)
{
	
}
