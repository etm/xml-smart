require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestBasic < MiniTest::Test
  def test_criss_cross
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    node = doc.root.find("/test/names").first

    assert(node.add("test_node", {"attr" => 12}, "Test1") === XML::Smart::Dom::Element)
    assert(node.add("test_node", "Test2", {"attr" => 13}) === XML::Smart::Dom::Element)
    assert(node.add("test_node", {"attr" => 14, :second => 'a'}) === XML::Smart::Dom::Element)
    assert(node.add("test_node", "Test4") === XML::Smart::Dom::Element)
    assert(node.add("test_node") === XML::Smart::Dom::Element)
    assert(node.add("test_node",'5') === XML::Smart::Dom::Element)
    assert(node.add("test_node",6) === XML::Smart::Dom::Element)
    assert(node.add("test_node",0) === XML::Smart::Dom::Element)

    assert(doc.find("number(/test/names/test_node[2])").nan?)
    assert(doc.find("number(/test/names/test_node[7])") == 5)
    assert(doc.find("number(/test/names/test_node[8])") == 6)
    assert(doc.find("number(/test/names/test_node[2]/@attr)") == 12)

    assert(doc.find("string(/test/names/test_node[1])") == "ac deity ")
    assert(doc.find("string(/test/names/test_node[2])") == "Test1")
    assert(doc.find("string(/test/names/test_node[3])") == "Test2")
    assert(doc.find("string(/test/names/test_node[4])") == "")
    assert(doc.find("string(/test/names/test_node[5])") == "Test4")
    assert(doc.find("string(/test/names/test_node[6])") == "")
    assert(doc.find("string(/test/names/test_node[7])") == "5")
    assert(doc.find("string(/test/names/test_node[8])") == "6")

    assert(doc.find("/test/names/test_node").first.dump      == "<test_node>a<b/>c<!-- argh --><![CDATA[ deity ]]></test_node>")
    assert(doc.find("/test/names/test_node").last.dump       == "<test_node>0</test_node>")
    assert(doc.find("/test/names/test_node")[0].dump         == "<test_node>a<b/>c<!-- argh --><![CDATA[ deity ]]></test_node>")
    assert(doc.find("/test/names/test_node")[1].dump         == "<test_node attr=\"12\">Test1</test_node>")
    assert(doc.find("/test/names/test_node")[2].dump         == "<test_node attr=\"13\">Test2</test_node>")
    assert(doc.find("/test/names/test_node")[4].dump         == "<test_node>Test4</test_node>")
    assert(doc.find("/test/names/test_node")[5].dump         == "<test_node/>")
    assert(doc.find("/test/names/test_node")[6].dump         == "<test_node>5</test_node>")
    assert(doc.find("/test/names/test_node")[7].dump         == "<test_node>6</test_node>")
    assert(doc.find("/test/names/test_node")[0..1].last.dump == "<test_node attr=\"12\">Test1</test_node>")
    assert(doc.find("/test/names/test_node")[0,2].last.dump  == "<test_node attr=\"12\">Test1</test_node>")
    assert(doc.find("/test/names/test_node")[-1].dump        == "<test_node>0</test_node>")

    tmp = doc.find("/test/names/test_node")[3].dump
    assert(tmp == "<test_node second=\"a\" attr=\"14\"/>" || tmp == "<test_node attr=\"14\" second=\"a\"/>")
                                                            
    nodes = doc.find("/test/names/test_node")                
    assert(nodes.length                                      == 9)
    assert(nodes.empty?                                      == false)
    assert(nodes.delete_all!                                 == true)
    assert(nodes.length                                      == 0)
    assert(nodes.empty?                                      == true)
    assert(doc.find("/test/names/test_node").length          == 0)
    assert(doc.find("/test/names/test_node").empty?          == true)

    count = 0
    nodes = doc.find("/test/colors/*")
    assert(nodes.length == 3)
    nodes.delete_if do |node|
      count += 1
      count % 2 == 0
    end  
    assert(nodes.length == 2)
    assert(doc.find("/test/colors/*")[0].dump == "<blue taint=\"true\">Yeah</blue>")
    assert(doc.find("/test/colors/*")[1].dump == "<green taint=\"true\"/>")

    node = doc.find("/test/names").first
    assert(node.children[2].to_s == "Michel")

    assert(doc.root.attributes['lang'] == 'de')
    assert(doc.root.attributes['lang',true] === XML::Smart::Dom::Attribute)
    assert(doc.find("//@xml:lang")[0].value == 'de')
    doc.find("//@xml:lang")[0].value = 'en'
    assert(doc.find("//@xml:lang")[0].value == 'en')

    assert(doc.namespaces == {"soap"=>"http://schemas.xmlsoap.org/wsdl/soap/"})
    doc.register_namespace 's', 'http://schemas.xmlsoap.org/wsdl/soap/'
    assert(doc.namespaces == {"soap"=>"http://schemas.xmlsoap.org/wsdl/soap/", "s"=>"http://schemas.xmlsoap.org/wsdl/soap/"})
    assert(doc.find("//soap:hallo").length == 1)
    assert(doc.find("//s:hallo").length == 1)

    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE-NS.xml")
    assert(doc.namespaces == {"xmlns0"=>"http://example.org", "xmlns1"=>"http://example1.org", "soap"=>"http://schemas.xmlsoap.org/wsdl/soap/"})
    assert(doc.find('//xmlns0:names/xmlns0:name')[2].dump == "<name team=\"1\">Michel</name>")

    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE-NSE.xml")
    node = doc.find("//soap:hallo")[0]
    doc.register_namespace 'x', 'http://xxx.org'
    assert(node.namespace.to_s   == "http://schemas.xmlsoap.org/wsdl/soap/")
    assert(node.namespace.prefix == "soap")
    assert(node.namespace.href   == "http://schemas.xmlsoap.org/wsdl/soap/")
    assert(node.qname.prefix     == "soap")
    assert(node.qname.href       == "http://schemas.xmlsoap.org/wsdl/soap/")

    node.namespace = 'x'
    assert(node.dump == "<test:hallo/>")
    assert(doc.root.namespaces.length == 3)
    node.namespace = 'xmlns'
    assert(node.dump == "<hallo/>");

    node = doc.find("/xmlns:test")[0]
    assert(node.qname.prefix == "")
    assert(node.qname.href   == "http://default.org")

    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    node = doc.root.find("/test/names").first
    node.append("test_node", {"attr" => 12}, "Test2")
    node.prepend("test_node", {"attr" => 12}, "Test3")
    assert(node.children.first.to_s == "Test3")
    assert(node.children.last.to_s == "Test2")
  end
end
