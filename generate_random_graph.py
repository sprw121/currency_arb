# Generates random exchange rates in the range of (.1,1)
# for all currency pairs in currencies.txt.
# This ensures that no arbitrage cycles are possible.
# Results to be used as a random test case for main.

import random

with open("currencies.txt") as f:
	currency_list = f.read().splitlines()

f = open("test_random.txt",'w')

for x in currency_list:
	for y in currency_list:
		f.writelines("%s %s %s\n" % (x,y, random.uniform(.1,1)))

f.close()
