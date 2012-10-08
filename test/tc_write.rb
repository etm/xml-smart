require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestWrite < Test::Unit::TestCase
  def test_write
    tim = Time.now

    # When a closure is used, then the changes are written back to the disk
    XML::Smart.modify(::File.dirname(__FILE__) + "/EXAMPLE.xml") do |doc|
      node = doc.root.find("/test/names/name")[0]
      node.text = tim
    end
    
    # Print the document
    assert(XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.xml").find("string(/test/names/name)") == tim.to_s)
  end
end
