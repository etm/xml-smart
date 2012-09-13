#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
soc = XML::Smart.string("<?xml version='1.0'?><root><node id='1'><text>I am a text</text></node><node id='2'/></root>")

# copy a node
nodeA = doc.root.find("/test/names/name[5]").first
nodeB = soc.root.find("/root/node[@id='1']").first
nodeS = soc.root.find("/root/*")

doc.root.add(nodeA) # move the node (default, XML::Smart::MOVE)
doc.root.add(nodeB) # copy the foreign node (default, XML::Smart::COPY)

# copy the node 3 times
3.times { doc.root.add(nodeA,XML::Smart::COPY) }

# copy all elements in nodeset
doc.root.add(nodeS)

puts soc
puts "-" * 79
puts doc
