require 'test/unit'

$timings = []

def start_timing(name)
  $timings << [name,false,Time.now.to_f]
end

def end_timing
  $timings.last[1] = true
  $timings.last[2] = Time.now.to_f - $timings.last[2]
end


