#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
node = doc.root.find("/test/names/name[@team='2']").first

# add siblings
node.add_before("test_node",{"attr" => "12"}, "Muahaha")
node.add_after("test_node", "Muahaha", {"attr" => "13"})

# print doc
puts doc
