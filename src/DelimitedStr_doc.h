


#define DELIMITEDSTR_MODULE_DOC \
	"Parse strings into lists by head/tail delimited pairs.\n\n" \
	"   Each instance owns its own delimiters. So a programmer can use\n" \
	"   different methods of parsing strings depending on different\n" \
	"   rules.\n" \
	"Constants:\n" \
	"                         Import with from delimterstr import KEEP_DELIMITERS, ...\n"\
	"   KEEP_DELIMITERS       The string keeps the delimiters with it.\n" \
	"                         Default is 0.\n" \
	"   DISCARD_DELIMITERS    The delimiters are not kept with the string.\n" \
	"   DISCARD_STRING        Everything between the delimiters and including\n" \
	"                         the delimiters is ignored.\n" \
	"   DELIMITER_WITH_STRING Will return a delimiter if it exists with a string.\n"

#define DELIMITEDSTR_CLASS_DOC \
	"The class stores and utilizes the delimiters for splitting strings into\n" \
	"a list."

#define DELIMITEDSTR_ADD_DELIMITER_DOC \
	"add_delimiter(head, tail, flags = 0)\n--\n\n" \
	"                                    adds a delimiter of head/tail to the list\n" \
	"                                    with flags.\n"

#define DELIMITEDSTR_SET_DELIMITER_DOC \
	"set_delimiter(index, head, tail, flags = 0)\n--\n\n" \
	"                                    Sets the delimiter of head/tail to the\n" \
	"                                    list at index within the delimiters.\n"

#define DELIMITEDSTR_REMOVE_DELIMITER_DOC \
	"remove_delimiter(index)\n--\n\n" \
	"                                    Removes the delimiter at index.\n"

#define DELIMITEDSTR_SPLIT_DOC \
	"split(string)\n--\n\n" \
	"                                    Converts the string into a list of strings\n" \
	"                                    separated by the delimiters.\n"

#define DELIMITEDSTR_GET_DOC \
	"get(index)\n--\n\n" \
	"   get the delimiters at index and write them to a list.\n"

#define DELIMITEDSTR_LEN_DOC \
	"len()\n--\n\n" \
	"   get the number of delimiters stored.\n"
