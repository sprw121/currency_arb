# This script takes the list of currencies in currencies.txt, and gets exchange rates
# on all pairs via yahoo api (query.yahooapis.com). The pairs are formatted into
# valid requests in batches based on the source currency, then these requests
# are submitted asynchronously through grequests.

# Yahoo's api can be unreliable at times, and may be missing data occasionally.
# User will not be warned in this script, but will be upon running main
# May take several attempts to successfully get the data.

# Yahoo's api is only accurate to 4 decimal places, and rounds everything below
# up to .0001. In the case of small values, this create discreprancies.
# This is dealt with by using the multiplicative inverse of the pair in the
# opposite direction in the case of pairs being less than .01.
# Not doing this results in some pairs being significantly off mark.

import json
import grequests
	 	
with open("currencies.txt") as c:
	currency_list = c.read().splitlines()

urls = []
exchange_rates = dict()

for x in currency_list:
	formatted_pairs = []
	
	for y in currency_list:
		formatted_pairs.append("%%22%s%%22" % (x+y))
		exchange_rates[x+y] = "default"
	
	base_url = "http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.xchange%20where%20pair%20in%20("
	end_url = ")&format=json&diagnostics=true&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys&callback="

	query = '%2C'.join(formatted_pairs)
	url = base_url + query + end_url	
	urls.append(url)

reqs = (grequests.get(u) for u in urls)


print("Waiting for responses....")
for res in grequests.map(reqs):
	parsed_data = json.JSONDecoder().decode(res.content)
	for x in parsed_data["query"]["results"]["rate"]:
		exchange_rates[x["id"]] = (x["Bid"],x["Ask"])

print("Got responses") 	
		
f = open("exchange_rates.txt",'w')
for key in sorted(exchange_rates.keys()):
	if(exchange_rates[key][0] == "default" or exchange_rates[key][0] == "N/A" or exchange_rates[key][0] == "d"):
		f.writelines("%s %s default\n" % (key[0:3],key[3:6]))
	elif (float(exchange_rates[key][0])  > .01):
		f.writelines("%s %s %s\n" % (key[0:3],key[3:6],exchange_rates[key][0]))
	else:
		opp = key[3:6] + key[0:3]
		f.writelines("%s %s %s\n" % (key[0:3],key[3:6], repr(1/float(exchange_rates[opp][1]))))
f.close()
