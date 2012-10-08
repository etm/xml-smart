require 'test/unit'
require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestRelaxng < Test::Unit::TestCase
  def test_relaxng
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/HELLO.xml")
    doc.xinclude!
    xsd = XML::Smart.open(::File.dirname(__FILE__) + "/HELLO.rng")

    nums = 10000
    # Watch the power
    start_timing "RelaxNG #validate_against (#{nums} times)"
    nums.times do
      doc.validate_against(xsd)
    end
    assert(doc.validate_against(xsd) == true)
    end_timing

    doc.find('/hellos/hello[3]').each do |h|
      h.qname.name = 'test'
    end
    assert(!doc.validate_against(xsd))
    
    doc.validate_against(xsd) do |err|
      assert("#{err.file} line #{err.line}: #{err.to_s}" =~ /HELLO.xml line 4: Did not expect element test there/)
    end
  end    
end  