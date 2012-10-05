require 'test/unit'
require 'time'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestConcurrent < Test::Unit::TestCase
  def test_unformated
    # Watch the power
    nums = 100

    id      = 16
    e_matnr = "9906264"
    e_name  = "mangler"
    e_secid = 1
    exam    = true

    result = ""
    p = []
    start_timing 'Concurrent - unformatted'
    0.upto(nums) do |i|
      p[i] = Thread.new do
        XML::Smart.modify(::File.dirname(__FILE__) + "/concurrent.xml","<solutions/>") do |xml|
          xml.save_unformated = true
          res = xml.find("/solutions/solution[@matnr=\"#{e_matnr}\" or @secid=\"#{e_secid}\"]").delete_all! if exam
          node = xml.root.add("solution",{:matnr => e_matnr, :name => e_name, :secid => e_secid, :assessment => id})
          1.upto(3) do |qbi|
            qid = qbi*2
            ques = node.add("question", :block => qbi, :question => qid)
          end

          result = xml.to_s 
        end  
      end  
    end

    p.each do |t|
      t.join
    end
    end_timing
  end

  def test_formated
    # Watch the power
    nums = 100

    id      = 16
    e_matnr = "9906264"
    e_name  = "mangler"
    e_secid = 1
    exam    = true

    p = []
    start_timing 'Concurrent - formatted'
    0.upto(nums) do |i|
      p[i] = Thread.new do
        XML::Smart.modify(::File.dirname(__FILE__) + "/concurrent.xml","<solutions/>") do |xml|
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
    end_timing
  end
end
