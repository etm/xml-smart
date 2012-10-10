require 'rake'
require 'rubygems/package_task'
require 'rake/testtask'

Rake::TestTask.new do |t|
  t.libs << "test"
  t.test_files = FileList['./test/tc_*.rb']
  t.verbose = false
end

spec = eval(File.read('xml-smart.gemspec'))
Gem::PackageTask.new(spec) do |pkg|
  pkg.need_zip = true
  pkg.need_tar = true
end

# class SmartRunner < Test::Unit::UI::Console::TestRunner
#   def finished(elapsed_time)
#     nl
#     output("Finished in #{elapsed_time} seconds.")
#     nl
#     @faults.each_with_index do |fault, index|
#       nl
#       output("%3d) %s" % [index + 1, fault.long_display])
#     end
#     nl if @faults.length > 0
#     $timings.each do |tim|
#       output("#{tim[0]}: #{'%f' % tim[2]} seconds") if tim[1]
#     end
#     nl if $timings.length > 0
#     output(@result)
#   end
# end
