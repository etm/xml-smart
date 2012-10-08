require 'test/unit'
require 'pp'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestXinclude < Test::Unit::TestCase
  def test_xinclude
    nums = 10000

    # Watch the power
    start_timing "#xinclude! (#{nums} times)"
    nums.times do
      doc = XML::Smart.open(::File.dirname(__FILE__) + "/HELLO.xml")
      doc.xinclude!
    end 
    end_timing

    doc = XML::Smart.open(::File.dirname(__FILE__) + "/HELLO.xml")
    doc.xinclude!

    stuff = doc.find('/hellos/*')
    assert(stuff[4].text == "hello me")
    assert(stuff[5].text == "hello curucamp")
  end  
end  
