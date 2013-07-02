require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestPi < MiniTest::Unit::TestCase
  def test_pi
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    pi = doc.root.add('?tobias', 'dj')
    assert pi.dump == "<?tobias dj?>"
    assert pi.path == "/test/processing-instruction('tobias')"
    assert pi.parent? == true
    assert pi.parent.unique_id == doc.root.unique_id
    assert pi.qname.name == 'tobias'
    assert pi.content == 'dj'
    pi.content = 'vj'
    assert pi.content == 'vj'

    doc.root.add_before('?xsl-stylesheet', 'a="3" b="7"')
    doc.root.children.delete_all!
    puts doc.to_s
    assert doc.to_s == "<?xml version=\"1.0\"?>\n<?xsl-stylesheet a=\"3\" b=\"7\"?>\n<test xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\" xml:lang=\"de\">\n  <?tobias vj?>\n</test>\n"
  end   
end  
