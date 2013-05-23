require 'rubygems'
gem 'minitest', '=4.7.4'
require 'minitest/autorun'
require 'time'

$timings = []

def start_timing(name)
  $timings << [name,false,Time.now.to_f]
end

def end_timing
  $timings.last[1] = true
  $timings.last[2] = Time.now.to_f - $timings.last[2]
end

class MiniTest::Unit
  OLD_run_anything = instance_method(:_run_anything)

  def _run_anything type
    OLD_run_anything.bind(self).call(type)
    if $timings.length > 0
      puts
      puts "PERFORMANCE:"
      $timings.each do |tim|
        puts "  * #{tim[0]}: #{'%f' % tim[2]} seconds" if tim[1]
      end
    end
  end
end
