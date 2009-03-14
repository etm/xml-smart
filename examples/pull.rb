#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# with next
 puller = XML::Smart.pull(File.open(File.dirname($0) + "/EXAMPLE.xml"))
 while puller.next do 
   puts puller.to_s
   p puller.node_type
 end  

# or with closure
XML::Smart.pull(File.open(File.dirname($0) + "/EXAMPLE.xml")) { |puller| 
  unless puller.attributes.empty?
    puller.attributes.each { |qname,value|
      puts "#{qname}=#{value}"
    }
  end
}
