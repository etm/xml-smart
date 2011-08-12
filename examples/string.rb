#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# create a string
xmldoc  = "<?xml version='1.0' encoding='ISO-8859-1'?>\n"
xmldoc << "<contributors>
  <guy id='1' nick='eTM'>Jürgen</guy>
  <guy id='2' nick='chris2'>Christian</guy>
  <guy id='3'>Emanuel</guy>
  <guy id='4'>41</guy>\n"
xmldoc << "</contributors>"

# parse it
doc = XML::Smart.string(xmldoc)

# output all guys with a nickname (the ü is a UTF-8 character in the output)
# cause (always) everything that is returned is UTF-8
doc.root.find("/contributors/guy[@nick]").each { |ele|
  puts "#{ele} (#{ele.attributes.include?("ick")})"
  puts "#{ele} (#{ele.attributes["nick"]})"
}

# query arthur dent
puts doc.root.find("/contributors/guy[@id='4']").first.to_i + 1

# write to a file
File.open(File.dirname($0) + "/EXAMPLE.str.xml",File::WRONLY|File::CREAT|File::TRUNC) { |f| doc.save_as f }
