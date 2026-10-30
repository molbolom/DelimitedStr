
#define RETEMPLIST_ERROR_LENGTH    1
#define RETEMPLIST_ERROR_ALLOC     2
#define RETEMPLIST_OK              0

#define REDELIMLIST_ERROR_LENGTH   1
#define REDELIMLIST_ERROR_ALLOC    2
#define REDELIMLIST_ERROR_ITEM     4
#define REDELIMLIST_OK             0

// 										User defined flags. 

#define KEEP_DELIMITERS            0
#define DISCARD_DELIMITERS         1
#define DISCARD_STRING             2
#define DELIMITER_WITH_STRING      4

// 										Initialization flags.

#define DELIMITER_NO_NEST	   (1<<8)

#define HAS_DISCARD_DELIMITERS(x)    ((x) & DISCARD_DELIMITERS)
#define HAS_DISCARD_STRING(x)        ((x) & DISCARD_STRING)
#define HAS_DELIMITER_WITH_STRING(x) ((x) & DELIMITER_WITH_STRING)
#define HAS_KEEP_DELIMITERS(x)        (!((x) & 0xff))

#define CAN_NEST(x)            (!((x) & DELIMITER_NO_NEST))

struct DELIMITERS {
	char *head;
	int   headlen;
	char *tail;
	int   taillen;
	int   flags;
};

struct STRING_DELIMITERS {
	int len;
	struct DELIMITERS *item;
};

struct STRING_LIST {
	const char *head;
	const char *tail;
};

struct TEMP_LIST {
	int c;
	int len;

	struct STRING_LIST *item;
};


void str2list(const char *);

void clean_str2list(void);
