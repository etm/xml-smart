require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestSort < Test::Unit::TestCase
  def test_sort
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    nodes = doc.find("//names/name").sort{|a,b| a.to_s <=> b.to_s}

    assert(nodes[0].text == 'Egon')
    assert(nodes[1].text == 'Jürgen')
    assert(nodes[2].text == 'Kathrin ')
    assert(nodes[3].text == 'Michel')
    assert(nodes[5].text == 'Raphi')
  end  
end  
