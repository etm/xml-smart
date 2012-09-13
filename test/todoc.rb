#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
stime = Time.now.to_f
nums = 10000
# Watch the power
nums.times do
  doc.find("/test/names").first.to_doc
end  
puts "#{nums} to_doc's done: #{Time.now.to_f - stime} seconds"
