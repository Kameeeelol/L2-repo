#include <string.h>
#include <stdlib.h>

char *crlf_to_lf(char *line_with_crlf)
{
	if (line_with_crlf == NULL) return NULL;
	int length = strlen(line_with_crlf);
	if (length < 2 || line_with_crlf[length-2] != '\r' || line_with_crlf[length-1] != '\n') return NULL;
	line_with_crlf[length-2] = '\n';
	line_with_crlf[length-1] = '\0';
	return line_with_crlf;
}

char *lf_to_crlf(char *line_with_lf)
{
	if (line_with_lf == NULL) return NULL;
	size_t length = strlen(line_with_lf);
	if (length<2 || line_with_lf[length-1] != '\n') return NULL;
	char *tmp = malloc(length+2);
	if (tmp == NULL) return NULL;
	memcpy(tmp, line_with_lf, length);
	tmp[length-1] = '\r';
	tmp[length] = '\n';
	tmp[length+1] = '\0';
	return tmp;
}
