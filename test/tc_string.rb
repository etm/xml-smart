# coding: utf-8
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestString < Minitest::Test
  def test_String
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")

    # create a string
    xmldoc  = "<?xml version='1.0'?>\n"
    xmldoc << "<contributors>
      <guy id='1' nick='eTM'>Jürgen</guy>
      <guy id='2' nick='chris2'>Christian</guy>
      <guy id='3'>Emanuel</guy>
      <guy id='4'>41</guy>\n"
    xmldoc << "</contributors>"
    
    # parse it
    doc = XML::Smart.string(xmldoc)
    
    # output all guys with a nickname (the ü is a UTF-8 character in the output)
    # cause (always) everything that is returned is UTF-8
    nodes = doc.root.find("/contributors/guy[@nick]")
    assert("#{nodes[0]} (#{nodes[0].attributes.include?("nick")})" == "Jürgen (true)")
    assert("#{nodes[0]} (#{nodes[0].attributes["nick"]})" ==          "Jürgen (eTM)")
    assert("#{nodes[1]} (#{nodes[1].attributes.include?("nick")})" == "Christian (true)")
    assert("#{nodes[1]} (#{nodes[1].attributes["nick"]})" ==          "Christian (chris2)")
    assert(nodes[2].nil?)

    # query arthur dent
    assert((doc.root.find("/contributors/guy[@id='4']").first.to_i + 1) == 42)
    
    # write to a file
    File.unlink(::File.dirname(__FILE__) + "/EXAMPLE.str.xml") rescue nil
    File.open(::File.dirname(__FILE__) + "/EXAMPLE.str.xml",File::WRONLY|File::CREAT|File::TRUNC) { |f| doc.save_as f }

    assert(XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.str.xml").root.to_s == XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.str.xml.test").root.to_s)
  end
end
