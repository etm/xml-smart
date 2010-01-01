#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")

# add an element hallo to root
nsnode = doc.root.add("hallo")

# add a namespace to the element
nsnode.namespaces.add("test","http://heise")
nsnode.namespaces.add(nil,"http://heise")
nsnode.namespaces[nil] = "http://heiser"
p nsnode.namespaces[nil]

nsnode.namespaces.each do |n|
  p n.prefix
  p n.href
end

# print document
puts doc
