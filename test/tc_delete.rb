# coding: utf-8
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestDelete < Minitest::Test
  def test_delete
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")

    nodes = doc.root.find("/test/names/name")
    assert(nodes.length == 6)
    
    nodes.delete_if{ |n| n.text == "Egon"; }
    assert(nodes.length == 5)
    
    nodes.delete_at(2)
    assert(nodes.length == 4)
    
    nodes.delete_at(-2)
    assert(nodes.length == 3)
    
    nodes = doc.root.find("/test/names/*")
    assert(nodes[0].qname.name == 'name')
    assert(nodes[0].attributes['team'] == '0')
    assert(nodes[0].attributes['a'] == '3')

    assert(nodes[1].qname.name == 'name')
    assert(nodes[1].attributes['team'] == '1')
    assert(nodes[1].text == "Jürgen")

    assert(nodes[2].qname.name == 'name')
    assert(nodes[2].attributes['team'] == '2')
    assert(nodes[2].text == 'Kathrin ')

    nodes = doc.root.find("/test/names/name/@team")
    nodes.delete_if{ |n| n.to_i % 2  != 0 }
    assert(nodes[0].value == '0')
    assert(nodes[1].value == '2')
    assert(nodes[2].nil?)
    
    nodes = doc.root.find("/test/names/name/@team")
    assert(nodes[0].value == '0')
    assert(nodes[1].value == '2')
    
    nodes.delete_all!
    assert(nodes.empty?)
    
    nodes = doc.root.find("/test/names/name/@team")
    assert(nodes.empty?)
  end
end
