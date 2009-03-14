#!/usr/bin/ruby
require "#{File.dirname($0)}/../smart"
require 'time'

nums = 100000
# Watch the power

id      = 16
e_matnr = "9906264"
e_name  = "mangler"
e_secid = 1
exam    = true

p = []
0.upto(nums) do |i|
  p[i] = Thread.new do
    XML::Smart.modify("concurrent.xml","<solutions/>") do |xml|
      res = xml.find("/solutions/solution[@matnr=\"#{e_matnr}\" or @secid=\"#{e_secid}\"]").delete_all! if exam
      node = xml.root.add("solution",{:matnr => e_matnr, :name => e_name, :secid => e_secid, :when => Time.now.xmlschema, :assessment => id})
      1.upto(3) do |qbi|
        qid = qbi*2
        ques = node.add("question", :block => qbi, :question => qid)
      end  
    end  
  end  
end

p.each do |t|
  t.join
end  
