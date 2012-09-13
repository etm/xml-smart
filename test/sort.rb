#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")

doc.find("//names/name").sort{|a,b| a.to_s <=> b.to_s}.each do |e|
  puts e.to_s
end


