require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestAdd < Test::Unit::TestCase
  def test_add
    nums  = 50000

    # Watch the power
    start_timing "Replace (#{nums} times)"
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    0.upto(nums) { |i|
      nodes = doc.find("/test/names/name[6]")
      nodes.first.parent.add("name", { "team" => "2" }, "Egon")
    }
    end_timing

    assert(doc.find("/test/names/name").length == nums + 7)
  end   
end  
