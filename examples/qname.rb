#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
node = doc.root.find("/test/s:hallo", {"s" => "http://schemas.xmlsoap.org/wsdl/soap/"} ).first

# check
if node.name == "hallo"
  p node.name.name
  p node.name.prefix
  p node.name.namespace
  p node.name.to_s
  p node.name.inspect
  p node.name
end  
