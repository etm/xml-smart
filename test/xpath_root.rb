#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
node = doc.find("//*/text()")
node.each { |n| puts n.path }
