#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

stime = Time.now.to_f
nums = 10000
# Watch the power
doc = nil
nums.times do
  doc = XML::Smart.open(File.dirname($0) + "/HELLO.xml")
  doc.xinclude!

  # doc.find("//hellos").each do |e|
  #   e.xinclude!
  # end
end 
puts "#{nums} loads and xincludes done: #{Time.now.to_f - stime} seconds"
puts doc.to_s
