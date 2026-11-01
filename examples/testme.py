

from delimitedstr import DelimitedStr
from delimitedstr import DISCARD_DELIMITERS, DISCARD_STRING, DELIMITER_WITH_STRING

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
print("   .add_delimiter(head, tail, flags)")
print("   .set_delimiter(index, head, tail, flags)  Sets a delimiter to index within the delimiter list.") 
print("   .remove_delimiter(index)                  Removes the delimiter at index.")
print("   .len()                                    Length of the delimter list.")
print("   .get(index)                               Return a list object representing the delimiter at")
print("                                             index.")
print("   .split(string)                            Returns a list of strings where each string is")
print("                                             separated by a space or a delimiter.")


print("""
delm = DelimitedStr()

delm.add_delimiter("{", "}", DISCARD_DELIMITERS)
delm.add_delimiter("[", "]", DELIMITER_WITH_STRING)
delm.add_delimiter("[", "]", DISCARD_DELIMITERS)
delm.add_delimiter("\"", "\"")         # Defaults to KEEP_DELIMITERS
delm.add_delimiter("'", "'")
delm.add_delimiter("<", ">")
delm.add_delimiter("(", ")")
""")

delm = DelimitedStr()

delm.add_delimiter("{", "}", DISCARD_DELIMITERS)
delm.add_delimiter("[", "]", DELIMITER_WITH_STRING)
delm.add_delimiter("[", "]", DISCARD_DELIMITERS)
delm.add_delimiter("\"", "\"")
delm.add_delimiter("'", "'")
delm.add_delimiter("<", ">")
delm.add_delimiter("(", ")")

A = ["This is a string",
     "This is[a delimited] string.",
     "Testing 'if this reads' correctly [a,b,c,d]"]

B = []


B.append(delm.split(A[0]))
B.append(delm.split(A[1]))
B.append(delm.split(A[2]))


print("----------------------------------------------------------------------------------------")
print(B[0])
print(B[1])
print(B[0])
print("----------------------------------------------------------------------------------------")



