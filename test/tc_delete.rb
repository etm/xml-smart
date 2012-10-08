#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"
require 'pp'

doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
nodes = doc.root.find("/test/names/name")

#puts "Original       : #{nodes.to_a.inspect}"
puts "#length        : #{nodes.length}"

nodes.delete_if{ |n| n.text == "Egon"; }
#puts "#delete_if    : #{nodes.to_a.inspect}"
puts "#length        : #{nodes.length}"

nodes.delete_at(2)
#puts "#delete_at(2) : #{nodes.to_a.inspect}"
puts "#length        : #{nodes.length}"

nodes.delete_at(-2)
#puts "#delete_at(-2): #{nodes.to_a.inspect}"
puts "#length        : #{nodes.length}"

puts doc.to_s
puts "\n"

nodes = doc.root.find("/test/names/name/@team")
puts "Before #delete_if:"
pp nodes.to_a
puts "-" * 40

nodes.delete_if{ |n| n.to_i % 2  != 0 }
puts "After #delete_if:"
pp nodes.to_a
puts "-" * 40

puts doc

nodes.delete_all!
puts "After #delete_all!:"
pp nodes.to_a
puts "-" * 40

puts doc
