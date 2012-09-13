#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/XSL_BASE.xml")
style = XML::Smart.open(File.dirname($0) + "/XSL_DOCUMENT.xml")

stime = Time.now.to_f
nums = 1000
# Watch the power
nums.times do
  doc.transform_with(style)
end  
puts "#{nums} transformations done: #{Time.now.to_f - stime} seconds"
