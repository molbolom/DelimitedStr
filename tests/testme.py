
import delimitedstr

delm = delimitedstr.DelimitedStr()

delm.add_delimiter("{", "}", delimitedstr.DISCARD_DELIMITERS)
delm.add_delimiter("[", "]", delimitedstr.DELIMITER_WITH_STRING)
delm.add_delimiter("[", "]", delimitedstr.DISCARD_DELIMITERS)
delm.add_delimiter("\"", "\"")
delm.add_delimiter("'", "'" )
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



