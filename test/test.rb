require 'rubygems'
require '../lib/xml/smart'
require 'pp'

doc = XML::Smart::open('EXAMPLE-NS.xml')
p doc.namespaces
nodes = doc.find('/xmlns:test/xmlns:names')

pp nodes


