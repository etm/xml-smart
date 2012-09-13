#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

doc = XML::Smart.open(File.dirname($0) + "/HELLO.xml")
doc.xinclude!
xsd = XML::Smart.open(File.dirname($0) + "/HELLO.xsd")

stime = Time.now.to_f
nums = 1000
# Watch the power
nums.times do
 p  doc.validate_against(xsd)
end
puts "#{nums} validations done: #{Time.now.to_f - stime} seconds"
