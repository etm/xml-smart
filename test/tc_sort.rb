# coding: utf-8
require 'rubygems'
require 'minitest/autorun'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestSort < MiniTest::Unit::TestCase
  def test_sort
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    nodes = doc.find("//names/name[@team>0]").sort{|a,b| a.to_s <=> b.to_s}

    assert(nodes[0].text == 'Egon')
    assert(nodes[1].text == 'Jürgen')
    assert(nodes[2].text == 'Kathrin ')
    assert(nodes[3].text == 'Michel')
    assert(nodes[4].text == 'Raphi')
  end  
end  
