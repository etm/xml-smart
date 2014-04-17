require File.expand_path(::File.dirname(__FILE__) + '/../lib/xml/smart')
require File.expand_path(::File.dirname(__FILE__) + '/smartrunner.rb')

class TestConcurrent < Minitest::Test
  def test_unformated
    # Watch the power
    nums = 1000

    id      = 16
    e_matnr = "9906264"
    e_name  = "mangler"
    e_secid = 1
    exam    = true

    result = ""
    p = []
    Minitest::PerformanceReporter::start_timing "Concurrent - unformatted (#{nums} times)"
    0.upto(nums) do |i|
      p[i] = Thread.new do
        XML::Smart.modify(::File.dirname(__FILE__) + "/concurrent.xml","<solutions/>") do |xml|
          xml.unformated = true
          res = xml.find("/solutions/solution[@matnr=\"#{e_matnr}\" or @secid=\"#{e_secid}\"]").delete_all! if exam
          node = xml.root.add("solution",{:matnr => e_matnr, :name => e_name, :secid => e_secid, :assessment => id})
          1.upto(3) do |qbi|
            qid = qbi*2
            ques = node.add("question", :block => qbi, :question => qid)
          end

          result = xml.to_s 
        end  
      end  
    end

    p.each do |t|
      t.join
    end
    Minitest::PerformanceReporter::end_timing
  end

  def test_formated
    # Watch the power
    nums = 1000

    id      = 16
    e_matnr = "9906264"
    e_name  = "mangler"
    e_secid = 1
    exam    = true

    p = []
    Minitest::PerformanceReporter::start_timing "Concurrent - formatted (#{nums} times)"
    0.upto(nums) do |i|
      p[i] = Thread.new do
        XML::Smart.modify(::File.dirname(__FILE__) + "/concurrent.xml","<solutions/>") do |xml|
          res = xml.find("/solutions/solution[@matnr=\"#{e_matnr}\" or @secid=\"#{e_secid}\"]").delete_all! if exam
          node = xml.root.add("solution",{:matnr => e_matnr, :name => e_name, :secid => e_secid, :when => Time.now.xmlschema, :assessment => id})
          1.upto(3) do |qbi|
            qid = qbi*2
            ques = node.add("question", :block => qbi, :question => qid)
          end
        end  
      end  
    end

    p.each do |t|
      t.join
    end  
    Minitest::PerformanceReporter::end_timing
  end
end
