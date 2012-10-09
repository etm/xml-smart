require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestMoveElements < Test::Unit::TestCase
  def test_move_elements
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    node = doc.find("/test/names/name[@team='1']").first
    
    # add all elements that have a bigger team number as previous siblings
    # (Egon an Kathrin are now before Juergen in the document)
    node.add_before(doc.find("/test/names/name[@team>=2]"))
    
    nodes = doc.find("/test/names/name[@team>0]")
    assert(nodes[0].text == "Kathrin ")
    assert(nodes[1].text == "Egon")
    assert(nodes[2].text == "J\303\274rgen")
    assert(nodes[3].text == "Michel")
    assert(nodes[4].text == "Raphi")
    assert(nodes[5].nil?)
  end  
end  