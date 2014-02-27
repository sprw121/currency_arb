import json
import urllib2

# Takes the results from results.txt and makes a request to yahoo
# for the includes exchange rates. Verifies the profit calculated
# in main. Deals with the issues defined in the comments of 
# get_exchange_rates_async.py in the same way.

cycles = []

with open("results.txt") as f:
	cycle = []
	for x in f.read().splitlines():
		if(x == "------"):
			cycle.reverse()
			cycles.append(cycle)
		else:
			cycle.append(x)

base_url = "http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.xchange%20where%20pair%20in%20("
end_url = ")&format=json&diagnostics=true&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys&callback="

for cycle in cycles:
	print("\nArbitrage cycle:")
	print(cycle)
	
	formatted_pairs = []
	formatted_reverse = []
	
	for x in cycle:
		formatted_pairs.append("%%22%s%%22" % (x))
	
	query = '%2C'.join(formatted_pairs)
	
	url = base_url + query + end_url
	
	res = urllib2.urlopen(url)
	parsed_data = json.load(res)

	exchange_rates = dict()

	for x in parsed_data["query"]["results"]["rate"]:	
		if(float(x["Bid"]) > .001):
			exchange_rates[x["id"]] = float(x["Bid"])
		else:
			formatted_reverse.append("%%22%s%%22" % (x["id"][3:6]+x["id"][0:3]))


	query = '%2C'.join(formatted_reverse)

	url = base_url + query + end_url
	res = urllib2.urlopen(url)
	parsed_data = json.load(res)
	
	data = parsed_data["query"]["results"]["rate"]

	if(data.has_key("Name")):
		exchange_rates[data["id"][3:6]+data["id"][0:3]] = 1/float(data["Ask"]) 
	else:
		for x in data["query"]["results"]["rate"]:
			exchange_rates[x["id"][3:6]+x["id"][0:3]] = 1/float(x["Ask"])

	profit = 1

	rates_used = []

	for x in cycle:
			rates_used.append(exchange_rates[x])
			profit *= exchange_rates[x]

	print("With exchange rates: ")	
	print(rates_used)
	print("\nGenerated a return of %f%%\n" % ((profit-1)*100))
