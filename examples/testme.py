

from delimitedstr import DelimitedStr
from delimitedstr import DISCARD_DELIMITERS, DISCARD_STRING, DELIMITER_WITH_STRING,DISCARD_TAIL

print("DelimitedStr is a library for parsing strings into lists based on")
print("the delimiters given to the program.")
print("Each instance owns its own delimiter string list.")
print("\n   Flags:")
print("           KEEP_DELIMITERS                   String keeps the delimiters with it.")
print("                                             Without a flag passed, this is the default.")
print("           DISCARD_DELIMITERS                String is returned without the delimiters.")
print("           DISCARD_STRING                    String and delimiters are not kept.")
print("           DELIMITER_WITH_STRING             Returns a string with a delimiter if")
print("                                             it is part of the word.")
print("                                             If a delimiter is added after this one,")
print("                                             then that one takes its flag.")
print("Methods:")
print("   .add(head, tail, flags)")
print("   .set(index, head, tail, flags)  Sets a delimiter to index within the delimiter list.") 
print("   .remove(index)                  Removes the delimiter at index.")
print("   .len()                                    Length of the delimter list.")
print("   .get(index)                               Return a list object representing the delimiter at")
print("                                             index.")
print("   .split(string)                            Returns a list of strings where each string is")
print("                                             separated by a space or a delimiter.")


print("""
delm = DelimitedStr()

delm.add("{", "}", DISCARD_DELIMITERS)
delm.add("[", "]", DELIMITER_WITH_STRING | DISCARD_DELIMITERS)
delm.add("\"", "\"")         # Defaults to KEEP_DELIMITERS
delm.add("/*", "*/", DISCARD_STRING)
delm.add("'", "'")
delm.add("<", ">")
delm.add("(", ")")
delm.add("#", "")
""")

delm = DelimitedStr()

delm.add("{", "}", DISCARD_DELIMITERS)
delm.add("[", "]", DELIMITER_WITH_STRING)
delm.add("[", "]", DISCARD_DELIMITERS)
delm.add("\"", "\"")
delm.add("/*", "*/", DISCARD_STRING)
delm.add("s.", ";", DISCARD_TAIL)
delm.add("'", "'")
delm.add("<", ">")
delm.add("(", ")")
delm.add("#", "")

A = ["This is a string without delimiters.",
     "This is[here is a delimiter with a string] a test with two similar [delimited strings].",
     "This string will show /*what happens when you use a comment block */ within a string",
     "Another{string with} different \"types of\" delimiters. abc[def] [geh]",
     "And now a s.string to; show # what happens if the tail is null."]


for s in A:
    c = delm.split(s)

    print(s)
    for item in c:
        print(f"{item} | ", end = "")
    print("\n")



