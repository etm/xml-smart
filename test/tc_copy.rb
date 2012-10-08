require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestCopy < Test::Unit::TestCase
  def test_copy
    # No closure, so changes are temporary
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    soc = XML::Smart.string("<?xml version='1.0'?><root><node id='1'><text>I am a text</text></node><node id='2'/></root>")

    # copy a node
    nodeA = doc.root.find("/test/names/name[5]").first
    nodeB = soc.root.find("/root/node[@id='1']").first
    nodeS = soc.root.find("/root/*")

    doc.root.add(nodeA) # move the node (default, XML::Smart::MOVE)
    doc.root.add(nodeB) # copy the foreign node (default, XML::Smart::COPY)

    # copy the node 3 times
    3.times { doc.root.add(nodeA,XML::Smart::COPY) }

    # copy all elements in nodeset
    doc.root.add(nodeS)

    assert(doc.find("/test/names/name[2]").first.dump == "<name team=\"1\">J&#xFC;rgen</name>")
    assert(doc.find("/test/names/name[3]").first.dump == "<name team=\"1\">Michel</name>")
    assert(doc.find("/test/names/name[4]").first.dump == "<name team=\"1\">Raphi</name>")
    assert(doc.find("/test/names/name[5]").first.dump == "<name team=\"2\">Egon</name>")

    assert(doc.find("/test/*[4]").first.dump          == "<name team=\"2\">Kathrin <b>Fiedler</b></name>")
    assert(doc.find("/test/*[5]").first.dump          == "<node id=\"1\">\n  <text>I am a text</text>\n</node>")
    assert(doc.find("/test/*[6]").first.dump          == "<name team=\"2\">Kathrin <b>Fiedler</b></name>")
    assert(doc.find("/test/*[7]").first.dump          == "<name team=\"2\">Kathrin <b>Fiedler</b></name>")
    assert(doc.find("/test/*[8]").first.dump          == "<name team=\"2\">Kathrin <b>Fiedler</b></name>")
    assert(doc.find("/test/*[9]").first.dump          == "<node id=\"1\">\n  <text>I am a text</text>\n</node>")
    assert(doc.find("/test/*[10]").first.dump         == "<node id=\"2\"/>")

    assert(soc.root.dump                              == "<root>\n  <node id=\"1\">\n    <text>I am a text</text>\n  </node>\n  <node id=\"2\"/>\n</root>")
  end
end
