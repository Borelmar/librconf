#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utilfuncs.h"
#include "rconf.h"

struct _config_st {
	int count;
	char **keys;
	char **values;
};

struct _config_st config_st;

static int append2config(const char* key, const char* val)
{
	config_st.count += 1;
	int key_len = strlen(key);
	int val_len = strlen(val);
	int new_i = config_st.count-1;

	/* allocing */
	config_st.keys = realloc(config_st.keys, config_st.count * 8);
	config_st.values = realloc(config_st.values, config_st.count * 8);

	config_st.keys[new_i] = (char*)malloc(key_len+1);
	config_st.values[new_i] = (char*)malloc(val_len+1);

	/* coping */
	strcpy( config_st.keys[new_i], key );
	strcpy( config_st.values[new_i], val );
	return 0;
}



static int _pstrconf
	(const char *str, char *key, size_t key_size, char *value, size_t value_size)
{
    const char *start = str;
    const char *end = start + strlen(str);
    const char *ptr = start;
    while (ptr < end)
    {
        while (_isuchr(*ptr) != 0)
	{
		ptr++;
	}

        start = ptr; /* store the start of key */

        while (_isuchr(*ptr) == 0)
	{
		ptr++; /* find the key end */
	}

        const size_t key_len = ptr - start; /* store the key size */

        if (key_len >= key_size)
	{
            return RCONF_ER_KEY_SIZE; // if buffser is small
        }

        memcpy(key, start, key_len);
        key[key_len] = '\0';
	if (_isustr(key) != 0)
	{
		return RCONF_ER_SYNTAX;
	}

        /* skiping the equal sign, tabes and spaces */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '=' || *ptr == '\n')
	{
            ptr++;
        }
        start = ptr; /* store the start of value */

        /* find the end of value */
        while (*ptr != '\0' && *ptr != ';' && *ptr != '\n') {
            ptr++;
        }
        const size_t value_len = ptr - start;

        if (value_len >= value_size)
	{
            return -2; /* buffer size small for value */
        }

        memcpy(value, start, value_len);
        value[value_len] = '\0'; /* add 0x0a to value end */
	if (_isustr(value) != 0)
	{
		return RCONF_ER_SYNTAX;
	}

        if (key_len > 0 && value_len > 0) {
            return 0; /* Success complete */
        }
    }
    return RCONF_ER_SYNTAX; /* key or/and value not found */
}

void free_rconfig(void)
{
	for(int i = config_st.count-1; i >= 0; i--)
	{
		free(config_st.keys[i]);
		free(config_st.values[i]);
	}
	free(config_st.keys);
	free(config_st.values);
	config_st.keys = NULL;
	config_st.values = NULL;
	config_st.count = 0;
}

int init_rconfig
	(const char *file)
{
	char *curr_conf_line;
	FILE *rconf_fd;
	int curr_chr;

	if(config_st.count != 0)
	{
		return RCONF_ER_INIT;
	}

	/* open config file */
	rconf_fd = fopen(file, "r");
	if (rconf_fd == NULL)
	{
		return RCONF_ER_CONF_FILE;
	}

	curr_conf_line = (char*)malloc(5);
	int i = 0;
	int skiping = 0;
	int pre_skiping = 0;
	int size_curr_conf_line = 5;

	while(1)
	{
		curr_chr = fgetc(rconf_fd);
		if (curr_chr == EOF)
		{
			break;
		}
		if (skiping == 0)
		{
			if (curr_chr == '/')
			{
				pre_skiping = 1;
				continue;
			}
			else if (curr_chr == '*' && pre_skiping == 1)
			{
				skiping = 1;
				continue;
			}
			else
			{
				skiping = 0;
				pre_skiping = 0;
			}

			/* end config loss */
			if (curr_chr == ';')
			{
				/* new config loss
				 * for a creating the one config loss
				 * we need a minimum 4 chars,
				 * ex: a=b;
				 * but then do if(strlen(curr_conf_line) == 0)
				 * this code curr_conf_line have in min 3
				 * (without ';')
				 */
				if(strlen(curr_conf_line) < 3)
				{
					/* free after crash */
					free_rconfig();
					free(curr_conf_line);
					return RCONF_ER_SYNTAX;
					fclose(rconf_fd);
				}
				else
				{
					/* else parsing */
					curr_conf_line[strlen(curr_conf_line)] = ';';
					char _key[RCONF_KEY_MAX_SIZE];
					char _val[RCONF_VAL_MAX_SIZE];
					int _rret = _pstrconf(
							curr_conf_line,
							_key,
							RCONF_KEY_MAX_SIZE,
							_val,
							RCONF_VAL_MAX_SIZE
							);

					/* if parsing error */
					if(_rret != 0)
					{
						if(config_st.count != 0)
						{
							free_rconfig();
						}
						free(curr_conf_line);
						fclose(rconf_fd);
						return _rret;
					}

					/* append to struct */
					append2config(_key, _val);

					/* clean cursors */
					free(curr_conf_line);
					curr_conf_line = (char*)malloc(5);
					size_curr_conf_line = 0;
					i = 0;
				}
			}
			else /* if curr chr not ';' */
			{
				size_curr_conf_line += 1;
				curr_conf_line = realloc(curr_conf_line, size_curr_conf_line);
				curr_conf_line[i] = curr_chr;
				i += 1;
			}
		}
		else /* find the end comment */
		{
			if(curr_chr == '*')
			{
				pre_skiping = 1;
				continue;
			}
			else if(curr_chr == '/' && pre_skiping == 1)
			{
				skiping = 0;
				pre_skiping = 0;
				continue;
			}
		}
	}
	fclose(rconf_fd);
	return 0;
}

char *read_rconfig(const char *key)
{
	char *_val = NULL;
	for(int i = 0; i < config_st.count; i++)
	{
		if(strcmp(key, config_st.keys[i]) == 0)
		{
			_val = (char*)malloc(
				strlen(config_st.values[i])
				);
			strcpy(_val, config_st.values[i]);
			return _val;
		}
	}
	return NULL;
}
