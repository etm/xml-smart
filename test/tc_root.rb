require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestBasic < MiniTest::Unit::TestCase
  def test_root
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")

    node = doc.find("test")
    assert(node.length == 1)

    node = doc.root.find("test")
    assert(node.length == 0)

    node = doc.root.find("/test")
    assert(node.length == 1)

    node = node.first.find("names")
    assert(node.length == 1)
  end
end
