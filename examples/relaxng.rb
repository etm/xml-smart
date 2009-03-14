#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

stime = Time.now.to_f
nums = 1000
# Watch the power
nums.times do
  doc = XML::Smart.open(File.dirname($0) + "/HELLO.xml")
  rng = XML::Smart.open(File.dirname($0) + "/HELLO.rng")
  doc.validate_against(rng)
end
puts "#{nums} validations done: #{Time.now.to_f - stime} seconds"
