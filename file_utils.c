#include "file_utils.h"

/**
* Read the whole stream and heap allocate a large enough memory space
* into a string.
*/
int read_whole_file(int fd, char **output_str)
{
	int max_len = 64;
	int len = 0;
	char *str = malloc((max_len + 1) * sizeof(char));
	
	int r;
	while ((r = read(fd, str + len, max_len - len)) != 0)
	{
		if (r == -1)
			err(EXIT_FAILURE, "read file");
		len += r;
		if (len == max_len)
		{
			max_len *= 2;
			str = realloc(str, (max_len + 1) * sizeof(char));
		}
	}
	str[len] = 0;
	*output_str = str;
	return len;
}


