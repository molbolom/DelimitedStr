


#define DELIMITEDSTR_MODULE_DOC \
	"Parse strings into lists by head/tail delimited pairs.\n\n" \
	"Constants:\n" \
	"   KEEP_DELIMITERS       The string keeps the delimiters with it.\n" \
	"                         Default is 0.\n" \
	"   DISCARD_DELIMITERS    The delimiters are not kept with the string.\n" \
	"   DISCARD_STRING        Everything between the delimiters and including\n" \
	"                         the delimiters is ignored.\n" \
	"   DELIMITER_WITH_STRING Will return a delimiter if it exists with a string.\n"


#define DELIMITEDSTR_ADD_DELIMITER_DOC \
	"add_delimiter(head, tail flags = 0)\n--\n\n"

#define DELIMITEDSTR_SET_DELIMITER_DOC \
	"set_delimiter(index, head, tail, flags = 0)\n--\n\n"

#define DELIMITEDSTR_REMOVE_DELIMITER_DOC \
	"remove_delimiter(index)\n--\n\n"

#define DELIMITEDSTR_SPLIT_DOC \
	"split(string)\n--\n\n"

