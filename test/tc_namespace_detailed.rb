require 'rubygems'
require 'minitest/autorun'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestNamespaceDetailes < MiniTest::Unit::TestCase
  def test_namespace_detailed
    # No closure, so changes are temporary
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")

    # add an element hallo to root
    nsnode = doc.root.add("hallo")

    assert(nsnode.dump == "<hallo/>")
    
    # add a namespace to the element
    ns = nsnode.namespaces.add("test","http://heise")
    
    # has the element a namespace? - No
    assert(!nsnode.namespace?)
    
    # set one
    nsnode.namespace = ns
    
    # has the element a namespace? - Yes
    assert(nsnode.namespace?)
    
    # inspect the namespace + print (to_s)
    assert(ns.prefix == 'test')
    assert(ns.href == 'http://heise')
    
    # add a node, a namespace, then add the namespace to the element (via a string)
    nsnode = doc.root.add("hallo")
    nsnode.namespaces["sulu"] = "http://slashdot"
    assert(nsnode.namespaces["soap"].nil?)
    assert(nsnode.namespaces["sulu"].prefix == 'sulu')

    nsnode.namespace = "sulu"
    assert(nsnode.namespaces["sulu"].prefix == 'sulu')

    nsnode.namespace = "http://slashdot"
    assert(nsnode.namespaces["sulu"].prefix == 'sulu')
  end  
end  
