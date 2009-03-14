#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
p doc.root.find("count(/test/names/name)")
p doc.root.find("string(/test/names/name/@team)")
