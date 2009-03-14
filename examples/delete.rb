#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
nodes = doc.root.find("/test/names/name")

puts "Original       : #{nodes.to_a.inspect}"

nodes.delete_if!{ |n| n.text == "egon"}
puts "#delete_if!    : #{nodes.to_a.inspect}"
puts "#length        : #{nodes.length}"
puts "#nitems        : #{nodes.nitems}"

nodes.delete_at!(2)
puts "#delete_at!(2) : #{nodes.to_a.inspect}"
puts "#length        : #{nodes.length}"
puts "#nitems        : #{nodes.nitems}"

nodes.delete_at!(-2)
puts "#delete_at!(-2): #{nodes.to_a.inspect}"
puts "#length        : #{nodes.length}"
puts "#nitems        : #{nodes.nitems}"

puts "\n"
nodes = doc.root.find("/test/names/name/@team")
puts "Original   : #{nodes.to_a.inspect}"
nodes.delete_if!{ |n| n.to_i % 2  != 0}
puts "#delete_if!: #{nodes.to_a.inspect}"

puts doc
