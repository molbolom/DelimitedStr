

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


s = "'abc' [def] xyc[alpha, beta, [0,1,2,3,4]] Yabba Dabba Do /*Brother bear*/" * 1000

t0 = time.perf_counter()
A = delm.split(s)
t1 = time.perf_counter()

print(f"split: {(t1-t0) * 1000:.3f} ms for {len(A)} items.")


#
#   First test: split: 0.848 ms for 6000 items.
#   
#   After tesing first characters of delimiters.
#
#   1. split: 0.548 ms for 6000 items.
#

