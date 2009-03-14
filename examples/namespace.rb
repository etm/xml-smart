#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# No closure, so changes are temporary
# add namespaces through a Hash
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
node = doc.root.find("/test/s:hallo", {"s" => "http://schemas.xmlsoap.org/wsdl/soap/"} ).first

# various aspects of the node
p node.name
puts node.name
puts node.name.prefix
puts node.name.namespace

