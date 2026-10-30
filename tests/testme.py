
import delimitedstr

delm = delimitedstr.DelimitedStr()

delm.add_delimiter("{", "}", 1)
delm.add_delimiter("[", "]", 4)
delm.add_delimiter("[", "]", 1)
delm.add_delimiter("\"", "\"", 0)
delm.add_delimiter("'", "'", 0)

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



