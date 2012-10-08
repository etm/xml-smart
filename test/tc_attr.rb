require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestAttr < Test::Unit::TestCase
  def test_attr
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    attrs = doc.root.attributes
    assert(attrs.length == 1)

    attrs.each do |n| 
      assert(n.qname.name == 'lang')
      assert(n.qname.prefix == 'xml')
      assert(n.qname.href == 'http://www.w3.org/XML/1998/namespace')
      assert(n.value == "de")
    end
    assert(attrs.include?("attr2") == false)

    doc.find("//@xml:lang").each do |attr|
      attr.value = "en"
    end
    attrs.each do |n| 
      assert(n.qname.name == 'lang')
      assert(n.qname.prefix == 'xml')
      assert(n.qname.href == 'http://www.w3.org/XML/1998/namespace')
      assert(n.value == "en")
    end
  end  
end  
