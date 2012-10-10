require 'rubygems'
require 'minitest/autorun'
require 'time'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestSetOrReplace < MiniTest::Unit::TestCase
  def test_set_or_replace
    nums  = 1000

    start_timing "set_or_replace (#{nums} times)"
    1.upto(nums) { |i|
      # create a XML document and copy the elements for each node to a file
      soc = XML::Smart.string("<?xml version='1.0'?><root><node id='1'><text>I am a text</text></node><node id='2'/></root>")
      soc.root.find("/root/node").each { |ele|
        cdoc = XML::Smart.open(::File.dirname(__FILE__) + "/#{ele.attributes["id"]}.xml")
        cdoc.root.replace_by(ele)
      }
    
      # create a XML document and copy the elements for each node to a file
      soc = XML::Smart.string("<?xml version='1.0'?><root><node id='1'><text>I am a text</text></node><node id='2'/></root>")
      soc.root.find("/root/node").each { |ele|
        cdoc = XML::Smart.open(::File.dirname(__FILE__) + "/#{ele.attributes["id"]}.xml")
        cdoc.root = ele
      }
    
      # create a file and copy a newly created XML document to it
      cdoc = XML::Smart.open(::File.dirname(__FILE__) + "/3.xml")
      cdoc.root = XML::Smart.string("<root><node id='1'><text>I am a text</text></node><node id='#{i}'/></root>").root
    }
    end_timing

    soc = XML::Smart.string("<?xml version='1.0'?><root><node id='1'><text>I am a text</text></node><node id='2'/></root>")
    eles = soc.root.find("/root/node")

    cdoc = XML::Smart.open(::File.dirname(__FILE__) + "/#{eles[0].attributes["id"]}.xml")
    cdoc.root.replace_by(eles[0])
    assert(cdoc.root.dump == "<node id=\"1\">\n  <text>I am a text</text>\n</node>")

    cdoc = XML::Smart.open(::File.dirname(__FILE__) + "/#{eles[1].attributes["id"]}.xml")
    cdoc.root.replace_by(eles[1])
    assert(cdoc.root.dump == "<node id=\"2\"/>")

    cdoc = XML::Smart.open(::File.dirname(__FILE__) + "/#{eles[0].attributes["id"]}.xml")
    cdoc.root = eles[0]
    assert(cdoc.root.dump == "<node id=\"1\">\n  <text>I am a text</text>\n</node>")

    cdoc = XML::Smart.open(::File.dirname(__FILE__) + "/#{eles[1].attributes["id"]}.xml")
    cdoc.root = eles[1]
    assert(cdoc.root.dump == "<node id=\"2\"/>")

    # create a file and copy a newly created XML document to it
    cdoc = XML::Smart.open(::File.dirname(__FILE__) + "/3.xml")
    cdoc.root = XML::Smart.string("<root><node id='1'><text>I am a text</text></node><node id='x'/></root>").root
    cdoc.root.dump == "<root>\n  <node id=\"1\">\n    <text>I am a text</text>\n  </node>\n  <node id=\"x\"/>\n</root>"
  end  
end  
