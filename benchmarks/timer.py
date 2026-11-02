

########################################################################################
#
# This file will run a long string and test how fast it runs and average it out over a
# number of runs.
#
# WARNING: This creates a string 1000*n long, and runs 500 times. Change the following
#          two values to other numbers if you want to check it on your system.
#
########################################################################################

string_expansion = 1000
run_length       =  500

import statistics as st
import time
from delimitedstr import DelimitedStr, DISCARD_DELIMITERS, DISCARD_STRING, DELIMITER_WITH_STRING

delm = DelimitedStr()

delimiters = [ ["{}", DISCARD_DELIMITERS],
               ["\"\"", 0 ],
               ["''",   0 ],
               ["<>", DISCARD_DELIMITERS],
               ["/**/", DISCARD_STRING],
               ["[]", DELIMITER_WITH_STRING],
               ["[]", DISCARD_DELIMITERS],
               ["##", DISCARD_DELIMITERS]]


for d in delimiters:
    c = len(d[0])//2
    delm.add_delimiter(d[0][0:c], d[0][c:], d[1])


s = "'abc' [def] xyc[alpha, beta, [0,1,2,3,4]] Yabba Dabba Do /*Brother bear*/" * string_expansion

delm.split(s)

cl = []
for x in range(run_length):
    t0 = time.perf_counter()
    A = delm.split(s)
    t1 = time.perf_counter()
    cl.append((t1-t0)*1000)

print(f"Averate of a string the length of {len(s)} splitting {run_length} times is {st.mean(cl)}")

#print(f"split: {(t1-t0) * 1000:.3f} ms for {len(A)} items.")

