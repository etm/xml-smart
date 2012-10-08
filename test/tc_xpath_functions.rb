require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestXpathFunctions < Test::Unit::TestCase
  def test_xpath_functions
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml")
    assert(doc.root.find("count(/test/names/name)") == 6.0)
    assert(doc.root.find("string(/test/names/name/@team)") == '0')
  end
end
