require 'rubygems'
require 'minitest/autorun'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestXSL < MiniTest::Unit::TestCase
  def test_xsl
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/XSL_BASE.xml")
    style = XML::Smart.open(::File.dirname(__FILE__) + "/XSL_DOCUMENT.xml")

    nums = 1000

    # Watch the power
    start_timing "#transform_with (#{nums} times)"
    nums.times do
      doc.transform_with(style)
    end  
    end_timing

    assert(doc.root.to_s == XML::Smart.open(::File.dirname(__FILE__) + "/XSL_BASE.xml.test").root.to_s)
  end
end  
