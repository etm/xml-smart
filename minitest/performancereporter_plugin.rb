require 'time'

module Minitest
  def self.plugin_performancereporter_init(options)
    self.reporter << PerformanceReporter.new(options[:io], options)
  end

  class PerformanceReporter < StatisticsReporter
    @@timings = []

    def self.start_timing(name)
      @@timings << [name,false,Time.now.to_f]
    end

    def self.end_timing
      @@timings.last[1] = true
      @@timings.last[2] = Time.now.to_f - @@timings.last[2]
    end

    def report
      super
      if @@timings.length > 0
        puts "\nPERFORMANCE:"
        @@timings.each do |tim|
          puts "  * #{tim[0]}: #{'%f' % tim[2]} seconds" if tim[1]
        end
      end
    end
  end
end
