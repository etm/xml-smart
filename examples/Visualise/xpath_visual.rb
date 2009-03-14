#!/usr/bin/ruby
require 'term-ansicolor-0.0.4/lib/term/ansicolor.rb'
require "#{File.dirname($0)}/../../smart"
include Term::ANSIColor

def rememberPath(path,output)
  @remember.include?(path) ? red + output + clear : output
end

def prnAttrs(node)
  str = node.attributes.collect{ |a| rememberPath(a.path,a.name + '="' + a.to_s + '"') }.join(" ")
  str == "" ? "" :  " " + str
end

def prnTree(node,depth,mixed)
  print " " * depth unless mixed
  print "<" + rememberPath(node.path,node.name.to_s) + prnAttrs(node) + ">"
  print "\n" if !node.mixed? && !mixed && node.children?
  node.children.each { |n|
    case n
      when XML::Smart::Dom::Element: prnTree(n,depth+2,node.mixed? | mixed)
      when XML::Smart::Dom::Text: print rememberPath(n.path,n.text)
    end  
  }
  print " " * depth if !node.mixed? && !mixed && node.children?
  print "</" + rememberPath(node.path,node.name.to_s) + ">" 
  print "\n" unless mixed
end

doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")

# xpath expression that should be visualised
xpath = ARGV[0] || "/"

# remember pathes, that an xpath expression selects
@remember = []
begin
  @remember = doc.find(xpath).collect { |n| n.path }
rescue
  puts "Invalid XPath!"
end

# pretty print tree
puts bold + "XPath:" + clear + " " + xpath + "\n"
prnTree(doc.root,0,false)
