require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestCreate < Minitest::Test
  def test_create
    # When a string as second paramter is provided, then a empty
    # xml file is created if it not exists. A block has to be supplied
    #
    # XML::Smart.modify(FILE,STRING) {}             ... create file if !exists?
    # XML::Smart.modify(FILE) {}                    ... just open file change and write back (LOCKTIMEOUT defaults to 7)
    
    File.unlink ::File.dirname(__FILE__) + "/EXAMPLE.tmp.xml" rescue nil
    t1 = Thread.new do
      XML::Smart.modify(::File.dirname(__FILE__) + "/EXAMPLE.tmp.xml","<elements/>") { |doc|
        node = doc.root.add("element","Thread 1")
        sleep 1
      }
    end
    t2 = Thread.new do
      sleep 0.5
      XML::Smart.modify(::File.dirname(__FILE__) + "/EXAMPLE.tmp.xml","<elements/>") { |doc|
        node = doc.root.add("element","Thread 2")
      }  
    end
    t1.join
    t2.join

    assert(XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.tmp.xml").root.to_s == XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE.tmp.xml.test").root.to_s)
  end
end
