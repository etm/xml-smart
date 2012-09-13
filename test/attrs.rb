#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
attrs = doc.root.attributes

attrs.each { |n| puts n.inspect }
puts attrs.include?("attr2")

doc.find("//@xml:lang").each do |attr|
  p attr.value
  attr.value = "en"
end

puts doc.to_s
