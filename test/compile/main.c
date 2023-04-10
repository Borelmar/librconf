#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <librconf.h>

/* 
 * int _pstrconf
 * 	(const char *str, char *key, size_t key_size, char *value, size_t value_size)
 */
/*
struct _config_st {
	int count;
	char **strings;
	char **values;
};
*/

int main(void)
{
	init_rconfig("config.txt");
	char *t1 = read_rconfig("opt1");
	char *t2 = read_rconfig("opt2");
	char *t3 = read_rconfig("opt3");
	printf("t1 = %s\n", t1);
	printf("t2 %s\n", t2);
	printf("t3 %s\n", t3);
	return 0;
}
