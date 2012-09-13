#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
doc.register_namespace("s","http://schemas.xmlsoap.org/wsdl/soap/")
node = doc.root.find("/test/s:hallo").first

# check
if node.qname == "hallo"
  p node.qname.name
  p node.qname.prefix
  p node.namespace
  p node.qname.to_s
  p node.qname.inspect
  p node.qname
end  
