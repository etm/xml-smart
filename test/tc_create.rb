#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

# When a string as second paramter is provided, then a empty
# xml file is created if it not exists. A block has to be supplied
#
# XML::Smart.modify(FILE,STRING) {}             ... create file if !exists?
# XML::Smart.modify(FILE) {}                    ... just open file change and write back (LOCKTIMEOUT defaults to 7)


XML::Smart.modify(File.dirname($0) + "/EXAMPLE.tmp.xml","<elements/>") { |doc|
  node = doc.root.add("element",Time.now)
}  

# Print the document
puts XML::Smart.open(File.dirname($0) + "/EXAMPLE.tmp.xml")
  
