#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

# No closure, so changes are temporary
# add namespaces through a Hash
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
doc.register_namespace('s', "http://schemas.xmlsoap.org/wsdl/soap/")
node = doc.root.find("/test/s:hallo").first

# various aspects of the node
p node.qname
puts node.qname
puts node.qname.prefix
puts node.qname.href
p node.namespace

