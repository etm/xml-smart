require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestBasic < Test::Unit::TestCase
  def test_criss_cross
    # When a string as second paramter is provided, then a empty
    # xml file is created if it not exists. A block has to be supplied
    #
    # XML::Smart.modify(FILE,STRING) {}             ... create file if !exists?
    # XML::Smart.modify(FILE) {}                    ... just open file change and write back (LOCKTIMEOUT defaults to 7)
    
    File.delete ::File.dirname(__FILE__) + "/EXAMPLE.tmp.xml"
    t1 = Thread.new do
      XML::Smart.modify(::File.dirname(__FILE__) + "/EXAMPLE.tmp.xml","<elements/>") { |doc|
        node = doc.root.add("element","Thread 1")
      }
      sleep 1
    end
    t2 = Thread.new do
      Thread.pass
      XML::Smart.modify(::File.dirname(__FILE__) + "/EXAMPLE.tmp.xml","<elements/>") { |doc|
        node = doc.root.add("element","Thread 2")
      }  
    end
    t1.join
    t2.join

    # Print the document
    assert(XML::Smart.open(File.dirname($0) + "/EXAMPLE.tmp.xml").root.to_s == XML::Smart.open(File.dirname($0) + "/EXAMPLE.tmp.xml.test").root.to_s)
  end
end
