#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
node = doc.find("/test/names").first

$stdout.sync = true
nums  = 1000000
# Watch the power
0.upto(nums) { |i|
  test = node.children[0].to_s
  test = node.children[0].to_s
  print "#{i}/#{nums} runs#{13.chr}" if i%1000 == 0
}
