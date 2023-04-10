#ifndef _INCLUDE_LIBRCONF_H_
#define _INCLUDE_LIBRCONF_H_

#define RCONF_ER_INIT		0x1000
#define RCONF_ER_SYNTAX		0x1001
#define RCONF_ER_KEY_SIZE	0x1002
#define RCONF_ER_VAL_SIZE	0x1003
#define RCONF_ER_CONF_FILE	0x1004
#define RCONF_ER_DOUBLE_LOSS	0x1005

extern int init_rconfig
	(const char *file);

extern void free_rconfig
	(void);

extern char *read_rconfig
	(const char *key);

#endif /* __RCONF_H */
