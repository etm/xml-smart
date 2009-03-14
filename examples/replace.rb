#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

$stdout.sync = true
stime = Time.now.to_f
nums  = 100000
# Watch the power
0.upto(nums) { |i|
  doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
  nodes = doc.root.find("/test/names/name[6]")
  nodes.first.parent.add("name", { "team" => "2" }, "Egon")
  print "#{i}/#{nums} runs#{13.chr}" if i % 100 == 0
}
puts "#{nums} parses from a file and some work done: #{Time.now.to_f - stime} seconds"
