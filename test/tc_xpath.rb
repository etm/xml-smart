require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestXpath < MiniTest::Unit::TestCase
  def test_xpath
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE-NS.xml")
    doc.register_namespace 'des', 'http://example.org'
    doc.register_namespace 'ex', 'http://example1.org'

    assert(doc.find('//des:names').length == 1)
    nodesA = doc.root.find("des:names")
    assert(nodesA.length == 1)
  end
end
