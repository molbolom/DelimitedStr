
# delimitedstr

    This is a library for generating a list of strings via a list of delimiters.


## Class


### DelimitedStr

    add(head, tail)            Add a delimiter with head and tail. flags default to KEEP_DELIMITERS  
    add(head, tail, flags)     Add a delimiter with flags.    
                                         add_delimiter will write to the first free position of delimiters.  

    set(x, head, tail)         Set the delimiter at x with head and tail.  
    set(x, head, tail, flags)  Set the delimiter at x with head, tail, and flags.  
                                         
                               if tail is "" or None, then the item will be from head to the end  
                               of the string.  

    remove(x)                  Remove the delimiter at x.  

    len()                      Get the length of the delimiters.
    get(x)                     Return a list of the head, tail, and flags of the delimiter at x.  
                                         Returns empty item if no delimiter exists at that position.  

    split(string)              Convert string into a list.  
                               Strings without delimiters are returned as single elements in the list.  


### Example Usage

```
from delimitedstr import DelimitedStr, KEEP_DELIMITERS, DISCARD_DELIMITERS, DISCARD_STRING, DELIMITER_WITH_STRING

delm = DelimitedStr()                      # Each instance will have its own delimiters.

delm.add("[", "]", DELIMITER_WITH_STRING)  # This must precede another delimiter
                                           # which is the same, but used differently
delm.add("[", "]", DISCARD_DELIMITER)      # The same delimiter, but after the DELIMITER_WITH_STRING

delm.add("\"", "\"")                       # flags default to 0.


# Input a string here.
s = "this is a test"

A = delm.split(s)
```

### How to install:
   
Requires a c compiler and Python development headers.

    pip install git+https://github.com/molbolom/DelimitedStr
