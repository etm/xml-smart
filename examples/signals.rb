#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"

# No closure, so changes are temporary
doc = XML::Smart.open(File.dirname($0) + "/EXAMPLE.xml")
p = nil
1.upto(2) do
  doc.on_change do |type,node|
    p node
  end
  p doc.change_handlers
  p =  Proc.new do |type,node|
     puts "1"
  end
  doc.change_handlers << p
end  
p doc.change_handlers
doc.change_handlers.delete(p)
p doc.change_handlers

$stdout.sync = true
stime = Time.now.to_f
nums  = 1
# Watch the power
0.upto(nums) { |i|
  node = doc.find("/test/names/name").first
  node.namespaces["a"] = "uri:b"
}  
