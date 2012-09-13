#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

doc = XML::Smart.open(File.dirname($0) + "/HELLO.xml")
doc.xinclude!
rng = XML::Smart.open(File.dirname($0) + "/HELLO.rng")

stime = Time.now.to_f
nums = 10000
# Watch the power
nums.times do
  doc.validate_against(rng)
end
puts "#{nums} validations done: #{Time.now.to_f - stime} seconds"
