#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"
# require 'profile'

$stdout.sync = true
stime = Time.now.to_f
nums  = 1000
soc = XML::Smart.string("<?xml version='1.0'?><root><node id='1'><text>I am a text</text></node><node id='2'/></root>")
# Watch the power
1.upto(nums) { |i|
  # create a XML document and copy the elements for each node to a file
  soc.root.find("/root/node").each do |ele|
    ts = ele.find('text')
    if ts.any?
      a = ts.first.to_doc
    end  
  end
  #ts = soc.find("/root/node").first
  #ts.add('text','Haller')

  print "#{i}/#{nums} runs#{13.chr}" if i%100 == 0
}
puts "#{nums} parses from a file and some work done: #{Time.now.to_f - stime} seconds"
