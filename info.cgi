#!/usr/local/bin/ruby

require 'cgi'
require 'open-uri'

def getMelbourneForecast
    url = 'http://bom.gov.au'
    data = open(url).read

    result = data.match(/Link to Melbourne forecast(.*?)<\/tr>/m)[1]
    result = result.match(/>(.*?)&deg/)[1]
end

def getLavertonObservation
    url = 'http://www.bom.gov.au/products/IDV60901/IDV60901.94865.shtml'
    data = open(url).read

    result = data.match(/t1-temp">(.*?)<\/td>/)[1]
end

def getMelbourneTime
    time = Time.now.utc + (3600 * 11)
end

def getLineOne
    data1 = getMelbourneForecast()
    data2 = getLavertonObservation()

    return 'Now ' + data2 + '  F ' + data1
end

def getLineTwo
    data = getMelbourneTime()
    data = data.strftime('%a %d/%m %H:%M')

    return data
end

cgi = CGI.new

cgi.out('text/plain') {
	'#'+getLineOne() + '#' + getLineTwo() + '#'
}
