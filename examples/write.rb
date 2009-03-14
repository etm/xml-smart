#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# When a closure is used, then the changes are written back to the disk
XML::Smart.modify(File.dirname($0) + "/EXAMPLE.xml") { |doc|
  node = doc.root.find("/test/names/name")[0]
  node.text = Time.now
}  

# Print the document
puts XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
