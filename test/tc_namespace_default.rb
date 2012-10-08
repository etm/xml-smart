require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestNamespaceDefault < Test::Unit::TestCase
  def test_namespace_default
    
    # No closure, so changes are temporary
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    doc.register_namespace('x','aaaaa')
    doc.unregister_namespace('soap')

    assert(doc.namespaces.length == 2)
    assert(doc.namespaces.key? 'soap')
    assert(doc.namespaces.key? 'x')
    
    # add an element hallo to root
    nsnode = doc.root.add("hallo")

    success = true
    nsnode.add('aaa:test') rescue success = false
    assert(!success)

    p nsnode.dump
    
    # add a namespace to the element
    nsnode.namespaces.add("test","http://heise")
    nsnode.namespaces.add("aaa","http://heise")
    nsnode.namespaces.add(nil,"http://heise")
    nsnode.namespaces[nil] = "http://heiser"
    
    nsnode.namespaces.each do |n|
      puts "#{n.prefix}: #{n.href}"
    end
    
    puts "-" * 50
    
    nsnode.namespaces.delete_at('aaa')
    nsnode.namespaces.each do |n|
      puts "#{n.prefix}: #{n.href}"
    end
    
    puts "-" * 50
    p doc.namespaces
    
    nsnode.namespaces.delete_all!
    p doc.namespaces
  end  
end  
