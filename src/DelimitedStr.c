
#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DelimitedStr.h"
#include "DelimitedStr_doc.h"

/*
 *	The c source global variables.
 *
 */

//				delms will be owned by the instance.
//				tmplist will be across all instances.

struct TEMP_LIST tmplist = { 0, 0, NULL };

typedef struct {
	PyObject_HEAD
	struct STRING_DELIMITERS delms;
} DelimitedStrObject;

/*	resize_delimiters(delimiter list, additional length)
 *
 *	This function resizes the delimiter list so new delimiters can be added to it.
 *
 *	The reason for this is since some delimiters need to precede other delimiters
 *	a person would be able to insert a delimiter or move a delimiter to another location.
 *	Thus holes would be preserved.
 *
 */

static int resize_delimiters(struct STRING_DELIMITERS *delms, int l)
{
	if (l <= 0)
		return(REDELIMLIST_ERROR_LENGTH);

	int i = delms->len;
		l = delms->len + l;

	struct DELIMITERS *newdelms = NULL;

	newdelms = realloc(delms->item, sizeof(struct DELIMITERS)*l);

	if (newdelms == NULL)
		return(REDELIMLIST_ERROR_ALLOC);

	for (int x=i;x<l;x++) {
		newdelms[x].head = NULL;
		newdelms[x].headlen = 0;
		newdelms[x].tail = NULL;
		newdelms[x].taillen = 0;
		newdelms[x].flags   = 0;
	}

	delms->len = l;
	delms->item = newdelms;
	
	return(REDELIMLIST_OK);
}

/****************************************************
 *
 *	remove_delimiter(x) will remove the delimiter at x.
 *
 ****************************************************
 */

static PyObject *DelimitedStr_remove_delimiter(PyObject *self, PyObject *pos)
{

	struct STRING_DELIMITERS *delms = &((DelimitedStrObject *)self)->delms;
	if (!PyLong_Check(pos) || PyBool_Check(pos)) {
		PyErr_SetString(PyExc_TypeError, "remove_delimiter() requires an integer.");
		return(NULL);
	}
	long x = PyLong_AsLong(pos);
	if ( (x == -1) && (PyErr_Occurred()) )
		return(NULL);

	if ( (x < 0) || (x >=delms->len) ) {
		PyErr_SetString(PyExc_OverflowError, "Index out of range.");
		return(NULL);
	}

	free(delms->item[x].head);
	delms->item[x].head = NULL;
	delms->item[x].tail = NULL;

	Py_RETURN_NONE;
}

/*
 *	write_delimiter(index, head, tail, flags)
 *
 *	Sets the delimiter, head/tail, at index with flags.
 *
 *	Not accessable through python.
 *
 */
static int write_delimiter(struct STRING_DELIMITERS *delms, int x, const char *head, const char *tail, int flags)
{

	if ((x > delms->len) || (x< 0)) 
		return(REDELIMLIST_ERROR_LENGTH);

	if (x == delms->len)
		if ( resize_delimiters(delms, 8) != REDELIMLIST_OK) 
			return(REDELIMLIST_ERROR_ALLOC);

	int headlen = strlen(head);
	int taillen = strlen(tail);

	char *newstr = malloc(sizeof(char)*(headlen+taillen+2));

	if (newstr == NULL) 
		return(REDELIMLIST_ERROR_ALLOC);

	if (delms->item[x].head != NULL)
		free(delms->item[x].head);

	memcpy(newstr, head, headlen+1);
	memcpy(newstr+headlen+1, tail, taillen+1);

	delms->item[x].head = newstr;
	delms->item[x].headlen = headlen;
	delms->item[x].tail = delms->item[x].head+delms->item[x].headlen+1;
	delms->item[x].taillen = taillen;
	
	if (strncmp(delms->item[x].head, delms->item[x].tail, delms->item[x].headlen) == 0)
		delms->item[x].flags = DELIMITER_NO_NEST | flags;
	else
		delms->item[x].flags = flags;

	return(REDELIMLIST_OK);
}

/*
 *	DelimitedStr_len()  Return the length of the delimiter array.
 *
 *
 */

static PyObject *DelimitedStr_len(PyObject *self, PyObject *a)
{
	struct STRING_DELIMITERS *delms = &((DelimitedStrObject *)self)->delms;
	return(PyLong_FromLong(delms->len));
}

/*
 *	get(index)  will return a list of the delimiter with the flags
 *				at index.
 *
 *
 */

static PyObject *DelimitedStr_get(PyObject *self, PyObject *a)
{
	struct STRING_DELIMITERS *delms = &((DelimitedStrObject *)self)->delms;
	if (!PyLong_Check(a) || PyBool_Check(a)) {
		PyErr_SetString(PyExc_OverflowError, "Requires an integer.");
		return(NULL);
	}

	long x = PyLong_AsLong(a);
	if ((x == -1) && PyErr_Occurred())
		return(NULL);
	if ((x<0) || (x >= delms->len)) {
		PyErr_SetString(PyExc_OverflowError, "Index out of range.");
		return(NULL);
	}

	if (delms->item[x].head == NULL)
		Py_RETURN_NONE;

	return(Py_BuildValue("[ssi]", delms->item[x].head, delms->item[x].tail,
						delms->item[x].flags & 0xff));

}

/*
 *	set_delimiter(index head, tail, flags)
 *
 *  Sets the delimiter with flags at index.
 *
 *  Since some delimiters need to precede other delimiters, this can be 
 *  used to ensure those delimiters are in their correct position.
 *
 */

static PyObject *DelimitedStr_set_delimiter(PyObject *self, PyObject *args) {

	int flags = 0;
	int x;
	const char *head;
	const char *tail;

	struct STRING_DELIMITERS *delms = &((DelimitedStrObject *)self)->delms;
	if (!PyArg_ParseTuple(args, "iss|i", &x, &head, &tail, &flags)) {
		return(NULL);
	}

	x = write_delimiter(delms, x, head, tail, flags);

	if (x & REDELIMLIST_ERROR_LENGTH) {
		PyErr_SetString(PyExc_OverflowError, "Index out of range.");
		return(NULL);
	} else if (x & REDELIMLIST_ERROR_ALLOC) {
		return(PyErr_NoMemory());
	}

	Py_RETURN_NONE;
}

/*	add_delimiter(head, tail, flags)
 *
 *	Searches for the first empty spot and adds the delimiter to it.
 *
 */

static PyObject *DelimitedStr_add_delimiter(PyObject *self, PyObject *args) {

	int flags = 0;
	int x;
	const char *head;
	const char *tail;

	if (!PyArg_ParseTuple(args, "ss|i", &head, &tail, &flags)) {
		return(NULL);
	}

	struct STRING_DELIMITERS *delms = &((DelimitedStrObject *)self)->delms;
	for(x=0;x<delms->len;x++)
		if (delms->item[x].head == NULL) break;

	
	x = write_delimiter(delms, x, head, tail, flags);

	if (x & REDELIMLIST_ERROR_LENGTH) {
		PyErr_SetString(PyExc_OverflowError, "Index out of range.");
		return(NULL);
	} else if (x & REDELIMLIST_ERROR_ALLOC) {
		return(PyErr_NoMemory());
	}

	Py_RETURN_NONE;
}

/*
 *	resize_tmplist(additional length)  This is not used outside of the 
 *			c library. All this routine does is lengthens the temporary
 *			list for storing strings before writing to the list.
 */

static int resize_tmplist(int l)
{

	if (l <= 0) 
		return(RETEMPLIST_ERROR_LENGTH);

	int i = tmplist.len;			// Start index for initializing data.
	l     = tmplist.len + l;

	struct STRING_LIST *newlist = NULL;

	newlist = realloc(tmplist.item, sizeof(struct STRING_LIST)*l);

	if (newlist == NULL)
		return(RETEMPLIST_ERROR_ALLOC);

	for (int x=i;x<l;x++) {
		newlist[x].head = NULL;
		newlist[x].tail = NULL;
	}

	tmplist.len = l;
	tmplist.item = newlist;
	return(RETEMPLIST_OK);
}

/*
 *	DelimitedStr_split(self, string)   The python usage of str2list.
 *
 *	This calls str2list and copies the list stored in tmplist to a
 *	python list. 
 *
 */

static PyObject *DelimitedStr_split(PyObject *self, PyObject *str)
{

	if (!PyUnicode_Check(str)) {
		PyErr_SetString(PyExc_TypeError, "split() needs a string.");
		return(NULL);
	}

	struct STRING_DELIMITERS *delms = &((DelimitedStrObject *)self)->delms;
	const char *s = PyUnicode_AsUTF8(str);
	if (s == NULL)
		return(NULL);

	str2list(delms, s);

	PyObject *newlist = PyList_New(tmplist.c);

	int x;

	for(x=0;x<tmplist.c;x++) {
		PyObject *obj = PyUnicode_FromStringAndSize(tmplist.item[x].head, tmplist.item[x].tail - tmplist.item[x].head);

		PyList_SET_ITEM(newlist, x, obj);
		
	}

	return(newlist);

}

/*
 * The main split function separate from the python code.
 *
 *
 *	str2list(delimited list, string)
 */

static void str2list(struct STRING_DELIMITERS *delms, const char *str)
{

	if (*str == 0) {		// Was an empty string passed.
		tmplist.c = 0;
		return;
	}

	if (tmplist.len == 0)	// If tmplist has not been initialized.
		return;

	tmplist.c = 0;			// Initialize necessary variables.

	const char *head = NULL;
	const char *tail = NULL;

	int x;

	while (*str != 0)
	{
		if (head == NULL)			// Trim white spaces if we're not reading a string.
		{
			if (*str == 0x20)
			{
				str++;
				continue;
			}
			head = str;				// Start reading the string.
		}

		for (x=0;x<delms->len;x++)
		{
			if (delms->item[x].head == NULL)				// if this is a hole, skip.
				continue;
			if (*str != delms->item[x].head[0])				// If the first characters match, continue.
				continue;
			if (strncmp(str, delms->item[x].head, delms->item[x].headlen) == 0)		//head is found.
			{
				tail = strstr(str+delms->item[x].headlen, delms->item[x].tail);
				if (tail != NULL)
				{
					if (CAN_NEST(delms->item[x].flags)) {			//If delimiters can nest.
						const char *nesthead = str;
						const char *nesttail = tail;
						while (1) {
							nesthead = strstr(nesthead+delms->item[x].headlen, delms->item[x].head);
							if (nesthead == NULL) {
								tail = nesttail;
								break;
							} else if ( nesthead > nesttail ) {
								tail = nesttail;
								break;	
							} else {
								nesttail = strstr(nesttail+delms->item[x].taillen, delms->item[x].tail);
								if (nesttail == NULL) {
									if (nesthead != nesttail) {
										tail = nesthead;
									}
									break;
								}
							}
								


						}
					}

					if (head + delms->item[x].headlen == tail) {	// If empty string in delimiter.
						str = tail + delms->item[x].taillen;
						tail = head;
						break;
					}
					if (HAS_DELIMITER_WITH_STRING(delms->item[x].flags)) {	// Delimiter can be read
																			// with string.
						if (head < str) {
							tail = tail + delms->item[x].taillen;
							str = tail;
						} else {
							tail = NULL;
							continue;
						}
						break;
					}

					if (head < str) {										   // Reading a normal string.
						tail =str;
					} else if (HAS_DISCARD_DELIMITERS(delms->item[x].flags)) { // Reading a discarded delimited string.
						head = head + delms->item[x].headlen;
						str = tail + delms->item[x].taillen;
					} else if (HAS_DISCARD_STRING(delms->item[x].flags)) {     // Reading a comment block.
						str = tail + delms->item[x].taillen;
						head = NULL;
						tail = NULL;
					} else if (HAS_KEEP_DELIMITERS(delms->item[x].flags)) {	   // Keep delims. 
						tail = tail + delms->item[x].taillen;
						str = tail;
					}

					break;

				}
			}
		}

		if (tail == head) {						//if tail == head, ignore last read.
			head = NULL;
			tail = NULL;
			continue;
		}

		if (tail == NULL) {						// If a normal string is being read.
			str++;
			if ((*str == 0x20) || (*str == 0))
				tail = str;
		}

		

		if ((tail != NULL) && (tail != head))   	// If tail was found, write to string.
		{

			if (tmplist.c >= tmplist.len)
			{
				int resize = resize_tmplist(8);
				if (resize != 0) {
					tmplist.c = 0;
					return;
				}
			}

			tmplist.item[tmplist.c].head = head;
			tmplist.item[tmplist.c].tail = tail;
			tmplist.c++;
			head = NULL;
			tail = NULL;

		} 
	}

}

/*	The python instance deallocation routine. 
 *
 * 	This is where the instance only values are deconstructed. 
 *
 */

static void DelimitedStr_dealloc(DelimitedStrObject *self)
{
	int x;
	struct STRING_DELIMITERS *delms = &self->delms;
	for (x=0;x<delms->len;x++)
		free(delms->item[x].head);

	free(delms->item);
	
	Py_TYPE(self)->tp_free((PyObject *)self);
}

static void clean_str2list(void)
{

	free(tmplist.item);
	tmplist.item = NULL;
	tmplist.len = 0;
	tmplist.c = 0;
}

static PyMethodDef DelimitedStr_methods[] = {
	{"add_delimiter", DelimitedStr_add_delimiter, METH_VARARGS, DELIMITEDSTR_ADD_DELIMITER_DOC},
	{"set_delimiter", DelimitedStr_set_delimiter, METH_VARARGS, DELIMITEDSTR_SET_DELIMITER_DOC},
	{"get",           DelimitedStr_get, METH_O, DELIMITEDSTR_GET_DOC},
	{"len",           DelimitedStr_len, METH_NOARGS, DELIMITEDSTR_LEN_DOC},
	{"remove_delimiter", DelimitedStr_remove_delimiter, METH_O, DELIMITEDSTR_REMOVE_DELIMITER_DOC},
	{"split", DelimitedStr_split, METH_O, DELIMITEDSTR_SPLIT_DOC},
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject DelimitedStrType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name        = "delimitedstr.DelimitedStr",
	.tp_basicsize   = sizeof(DelimitedStrObject),
	.tp_flags       = Py_TPFLAGS_DEFAULT,
	.tp_doc         = DELIMITEDSTR_CLASS_DOC,
	.tp_new         = PyType_GenericNew,
	.tp_methods     = DelimitedStr_methods,
	.tp_dealloc     = (destructor)DelimitedStr_dealloc,
};


static struct PyModuleDef delimitedstr_module = {
	PyModuleDef_HEAD_INIT,
	"delimitedstr",
	DELIMITEDSTR_MODULE_DOC,
	-1,
	NULL
};

PyMODINIT_FUNC PyInit_delimitedstr(void)
{
	// Initialize tmplist and delms here.

	if (PyType_Ready(&DelimitedStrType) < 0)
		return(NULL);
	
	PyObject *m = PyModule_Create(&delimitedstr_module);
	if (m == NULL)
		return(NULL);
	PyModule_AddIntConstant(m, "KEEP_DELIMITERS", KEEP_DELIMITERS);
	PyModule_AddIntConstant(m, "DISCARD_DELIMITERS", DISCARD_DELIMITERS);
	PyModule_AddIntConstant(m, "DISCARD_STRING", DISCARD_STRING);
	PyModule_AddIntConstant(m, "DELIMITER_WITH_STRING", DELIMITER_WITH_STRING);

	Py_INCREF(&DelimitedStrType);
	if (PyModule_AddObject(m, "DelimitedStr", (PyObject *)&DelimitedStrType) < 0) {
		Py_DECREF(&DelimitedStrType);
		Py_DECREF(m);
		return(NULL);
	}
	resize_tmplist(15);

/*	tmplist is freed at exit because it is used across instances
 *  and freeing it at any other time would break the program.
 *
 *
 */
	if (Py_AtExit(clean_str2list) < 0 )		
		clean_str2list();
	
	return(m);
}
