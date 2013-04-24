require 'rubygems'
require 'minitest/autorun'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestNamespaceDefault < MiniTest::Unit::TestCase
  def test_namespace_default
    # No closure, so changes are temporary
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    assert(doc.register_namespace('x','http://schemas.xmlsoap.org/wsdl/soap/') == true)
    assert(doc.register_namespace('y','aaaa') == false)
    assert(doc.unregister_namespace('soap') == false)

    assert(doc.namespaces.length == 2)
    assert(doc.namespaces.key? 'soap')
    assert(doc.namespaces.key? 'x')
    
    # add an element hallo to root
    nsnode = doc.root.add("hallo")

    success = true
    nsnode.add('aaa:test') rescue success = false
    assert(!success)

    assert(nsnode.dump == "<hallo/>")
    assert(!nsnode.namespace?)

    success = true
    n = nsnode.add('x:test') rescue success = false
    assert(success)

    assert(nsnode.dump == "<hallo>\n  <soap:test/>\n</hallo>")

    assert(nsnode.qname.name                                    == "hallo")
    assert(nsnode.qname.to_s                                    == "hallo")
    assert(nsnode.qname.prefix                                  == nil)
    assert(nsnode.qname.href                                    == nil)
    assert(nsnode.namespace?                                    == false)
    assert(nsnode.namespace                                     == nil)

    assert(n.qname.name                                         == "test")
    assert(n.qname.to_s                                         == "soap:test")
    assert(n.qname.prefix                                       == "soap")
    assert(n.qname.href                                         == "http://schemas.xmlsoap.org/wsdl/soap/")
    assert(n.namespace?                                         == true)
    assert(n.namespace.instance_of?(XML::Smart::Dom::Namespace) == true)
    assert(n.namespace.prefix                                   == "soap")
    assert(n.namespace.href                                     == "http://schemas.xmlsoap.org/wsdl/soap/")

    # add a namespace to the element
    nsnode.namespaces.add("test","http://heise1")
    nsnode.namespaces.add("aaa","http://heise2")

    assert(nsnode.namespaces[nil] == nil)
    
    nsnode.namespaces.add(nil,"http://heise")
    nsnode.namespaces[nil] = "http://heiser"

    assert(nsnode.namespaces[nil] != nil)

    assert(nsnode.namespaces[nil].prefix == 'xmlns')
    assert(nsnode.namespaces[nil].href == "http://heiser")
    assert(nsnode.namespaces['xmlns'].href == "http://heiser")
    assert(nsnode.namespaces['aaa'].href == "http://heise2")
    assert(nsnode.namespaces['test'].href == "http://heise1")
    
    assert(doc.namespaces.length == 5)
    nsnode.namespaces.delete_at('aaa')
    assert(nsnode.namespaces['aaa'].nil?)
    
    assert(doc.namespaces.length == 4)
    nsnode.namespaces.delete_all!
    assert(doc.namespaces.length == 2)
  end  
end  
