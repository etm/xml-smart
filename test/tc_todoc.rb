require 'rubygems'
require 'minitest/autorun'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestToDoc < MiniTest::Unit::TestCase
  def test_todoc
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")

    nums = 10000

    # Watch the power
    start_timing "#to_doc (#{nums} times)"
    nums.times do
      doc.find("/test/names/name[2]").first.to_doc
    end  
    end_timing
    
    assert(doc.find("/test/names/name[2]").first.to_doc.root.dump == '<name team="1">J&#xFC;rgen</name>')
  end
end  
