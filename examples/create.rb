#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# When a string as second or third paramter is provided, then a empty
# xml file is created if it not exists. A block has to be supplied
#
# XML::Smart.modify(FILE,LOCKTIMEOUT,STRING) {} ... set timeout for lock file and create file if !exists?
# XML::Smart.modify(FILE,LOCKTIMEOUT) {}        ... set timeout for lock
# XML::Smart.modify(FILE,STRING) {}             ... create file if !exists?
# XML::Smart.modify(FILE) {}                    ... just open file change and write back (LOCKTIMEOUT defaults to 7)

XML::Smart.modify(File.dirname($0) + "/EXAMPLE.tmp.xml","<elements/>") { |doc|
  node = doc.root.add("element",Time.now)
}  

# Print the document
puts XML::Smart.open(File.dirname($0) + "/EXAMPLE.tmp.xml")
  
