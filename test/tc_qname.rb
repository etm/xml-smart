require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestQname < Test::Unit::TestCase
  def test_qname
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    doc.register_namespace("s","http://schemas.xmlsoap.org/wsdl/soap/")

    node = doc.root.find("/test/s:hallo").first
    assert(!node.nil?)
    assert(node.qname == "hallo")
    assert(node.qname.name == 'hallo')
    assert(node.qname.prefix == 'soap')
    assert(node.namespace.instance_of? XML::Smart::Dom::Namespace)
    assert(node.qname.to_s == 'soap:hallo')
    assert(node.qname.instance_of? XML::Smart::QName)

    node = doc.root.find("/test").first
    assert(node.qname.to_s == 'test')

    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE-NS.xml")

    node = doc.root.find("/xmlns0:test").first
    assert(!node.nil?)
    assert(node.qname == "test")
    assert(node.qname.to_s == "test")
  end  
end  
