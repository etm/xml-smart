#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")

# add an element hallo to root
nsnode = doc.root.add("hallo")

# add a namespace to the element
ns = nsnode.namespaces.add("test","http://heise")

# has the element a namespace? - No
p nsnode.namespace?

# set one
nsnode.namespace = ns

# has the element a namespace? - Yes
p nsnode.namespace?

# inspect the namespace + print (to_s)
p ns

# add a node, a namespace, then add the namespace to the element (via a string)
nsnode = doc.root.add("hallo")
nsnode.namespaces["sulu"] = "http://slashdot"
nsnode.namespace = "sulu" # nsnode.namespace = "http://slashdot" would have worked also
nsnode.namespaces["sulu"] = "http://slashdot/test"

# find namspaces
p nsnode.namespaces.find("soap")
p nsnode.namespaces.find("http://slashdot/test")

# print document
puts doc
