require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestXpathAttrs < Minitest::Test
  def test_xpath_attrs
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    node = doc.root.find("/test/names/name[2]/@team").first
    
    assert(node.qname == "team")

    # print attribute name and value
    assert(node.qname + "=" + node.value == 'team=1')
    assert(node.qname + "=" + node.to_s == 'team=1')
    
    # set the value of the attribute
    node.value = "Value"
    assert(node.value == 'Value')
    
    doc.root.find("/test/names/name[2]/@team").first.value = "Hi all"
    assert(doc.root.find("/test/names/name[2]/@team").first.value == 'Hi all')
  end  
end  
