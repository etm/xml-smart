#!/usr/bin/ruby
require "#{File.dirname($0)}/../lib/smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
doc.register_namespace('x','aaaaa')
doc.unregister_namespace('soap')

# add an element hallo to root
nsnode = doc.root.add("hallo")
nsnode.add('aaa:test')

# add a namespace to the element
nsnode.namespaces.add("test","http://heise")
nsnode.namespaces.add("aaa","http://heise")
nsnode.namespaces.add(nil,"http://heise")
nsnode.namespaces[nil] = "http://heiser"

nsnode.namespaces.each do |n|
  puts "#{n.prefix}: #{n.href}"
end

puts "-" * 50

nsnode.namespaces.delete_at('aaa')
nsnode.namespaces.each do |n|
  puts "#{n.prefix}: #{n.href}"
end

puts "-" * 50
p doc.namespaces

nsnode.namespaces.delete_all!
p doc.namespaces
