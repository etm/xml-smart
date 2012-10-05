require 'test/unit'
require 'test/unit/ui/console/testrunner'

$timings = []

class SmartRunner < Test::Unit::UI::Console::TestRunner
  def finished(elapsed_time)
    nl
    output("Finished in #{elapsed_time} seconds.")
    nl
    @faults.each_with_index do |fault, index|
      nl
      output("%3d) %s" % [index + 1, fault.long_display])
    end
    nl if @faults.length > 0
    $timings.each do |tim|
      output("#{tim[0]}: #{'%f' % tim[2]} seconds") if tim[1]
    end
    nl if $timings.length > 0
    output(@result)
  end
end

Test::Unit::AutoRunner::RUNNERS[:console] = proc do |r|
  SmartRunner
end

def start_timing(name)
  $timings << [name,false,Time.now.to_f]
end

def end_timing
  $timings.last[1] = true
  $timings.last[2] = Time.now.to_f - $timings.last[2]
end


