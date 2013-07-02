require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestXSL < MiniTest::Unit::TestCase
  def test_version
    assert XML::Smart::VERSION.is_a? String
    assert XML::Smart::VERSION =~ /\d+\.\d+\.\d+(\.\d+)?/
  end
end  
