require 'rubygems'
require 'minitest/autorun'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestNamespaceFind < MiniTest::Unit::TestCase
  def test_namespace_find
    # No closure, so changes are temporary
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")

    # add a node with namespace
    nsnode = doc.root.add("hallo")
    nsnode.namespaces.add("test","http://heise")
    nsnode.add("test:entry")

    # find node
    assert(doc.find("//test:entry").empty? == false)
    
    doc.register_namespace("t","http://heise")
    assert(nsnode.find("t:entry").empty? == false)
  end  
end  
