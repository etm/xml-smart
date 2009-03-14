#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"
require 'profile'

$stdout.sync = true
stime = Time.now.to_f
nums  = 1000
# Watch the power
1.upto(nums) { |i|
  # create a XML document and copy the elements for each node to a file
  soc = XML::Smart.string("<?xml version='1.0'?><root><node id='1'><text>I am a text</text></node><node id='2'/></root>")
  soc.root.find("/root/node").each { |ele|
    XML::Smart.modify(File.dirname($0) + "/#{ele.attributes["id"]}.xml","<services/>") { |cdoc|
      cdoc.root.replace_by(ele)
    }  
  }

  # create a XML document and copy the elements for each node to a file
  soc = XML::Smart.string("<?xml version='1.0'?><root><node id='1'><text>I am a text</text></node><node id='2'/></root>")
  soc.root.find("/root/node").each { |ele|
    XML::Smart.modify(File.dirname($0) + "/#{ele.attributes["id"]}.xml","<services/>") { |cdoc|
      cdoc.root = ele
    }  
  }

  # create a file and copy a newly created XML document to it
  XML::Smart.modify(File.dirname($0) + "/1.xml","<services/>") { |cdoc|
    cdoc.root = XML::Smart.string("<root><node id='1'><text>I am a text</text></node><node id='2'/></root>").root
  }
  print "#{i}/#{nums} runs#{13.chr}" if i%100 == 0
}
puts "#{nums} parses from a file and some work done: #{Time.now.to_f - stime} seconds"
