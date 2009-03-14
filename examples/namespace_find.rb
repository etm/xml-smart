#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")

# add a node with namespace
nsnode = doc.root.add("hallo")
ns = nsnode.namespaces.add("test","http://heise")
nsnode.add("test:entry")

# find node
puts "Find 1: #{doc.find("test:node").empty?}" rescue puts "Find 1: undefined namespace prefix"
puts "Find 2: #{doc.find("test:node",{"test" => "http://heise"}).empty?}"

doc.namespaces = {"t" => "http://heise"}
puts "Find 3: #{doc.find("t:node").empty?}"

# print document
puts doc
