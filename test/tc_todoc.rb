require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestToDoc < Minitest::Test
  def test_todoc
    doc = XML::Smart.open(::File.dirname(__FILE__) + "/EXAMPLE-NS.xml")
    doc.root.children[0].add('soap:haller')
    doc.register_namespace :d, 'http://example.org'

    nums = 10000

    assert(doc.find("/d:test/d:names").first.to_doc.root.namespaces.length == 2)
    assert(doc.find("/d:test/d:names/d:name[2]").first.to_doc.root.namespaces.length == 1)
    assert(doc.find("/d:test").first.to_doc.root.namespaces.length == 2)

    # Watch the power
    Minitest::PerformanceReporter::start_timing "#to_doc (#{nums} times)"
    nums.times do
      doc.find("/d:test/d:names/d:name[2]").first.to_doc
    end  
    Minitest::PerformanceReporter::end_timing
     
    assert(doc.find("/d:test/d:names/d:name[2]").first.to_doc.root.dump == '<name xmlns="http://example.org" team="1">J&#xFC;rgen</name>')
  end
end  
