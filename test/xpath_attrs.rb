#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
node = doc.root.find("/test/names/name[2]/@team").first

p node.qname == "tea"
# print attribute name and value
puts node.qname + "=" + node.value
puts node.qname + "=" + node.to_s
puts node.inspect

# set the value of the attribute
node.value = "Value"
p node

node = doc.root.find("/test/names/name[2]/@team").first.value = "Hi all"
p node
