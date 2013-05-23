require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestNested < MiniTest::Unit::TestCase
  def test_nested
    nums  = 5000
    soc = XML::Smart.string("<?xml version='1.0'?><root><node id='1'><text>I am a text</text></node><node id='2'/></root>")
    # Watch the power
    start_timing "Nested (#{nums} times)"
    1.upto(nums) { |i|
      # create a XML document and copy the elements for each node to a file
      soc.find("/root/node").each do |ele|
        ts = ele.find('text')
        if ts.any?
          a = ts.first.to_doc
        end  
      end
      ts = soc.find("/root/node").first
      ts.add('text','Haller')
    }
    end_timing
  end
end
