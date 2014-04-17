require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestXpathRoot < Minitest::Test
  def test_xpath_root
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    nodes = doc.find("//*/text()")
    assert(nodes[0].path == "/test/names/name[1]/text()")
    assert(nodes[1].path == "/test/names/name[2]/text()")
    assert(nodes[2].path == "/test/names/name[3]/text()")
    assert(nodes[3].path == "/test/names/name[4]/text()")
    assert(nodes[4].path == "/test/names/name[5]/text()")
    assert(nodes[5].path == "/test/names/name[5]/b/text()")
    assert(nodes[6].path == "/test/names/name[6]/text()")
    assert(nodes[7].path == "/test/names/test_node/text()[1]")
    assert(nodes[8].path == "/test/names/test_node/text()[2]")
    assert(nodes[9].path == "/test/names/test_node/text()[3]")
    assert(nodes[10].path == "/test/colors/blue/text()")
    assert(nodes[11].nil?)
  end
end
