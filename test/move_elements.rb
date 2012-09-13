#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
node = doc.find("/test/names/name[@team='1']").first

# add all elements that have a bigger team number as previous siblings
# (Egon an Kathrin are now before Juergen in the document)
node.add_before(doc.find("/test/names/name[@team>=2]"))

puts doc
