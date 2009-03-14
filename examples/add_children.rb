#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
node = doc.root.find("/test/names").first

# append children
node.add("test_node", {"attr" => 12}, "Muahaha")
node.add("test_node", "Muahaha", {"attr" => 13})
node.add("test_node", {"attr" => 14})
node.add("test_node", "Muahaha")
node.add("test_node")
puts doc
