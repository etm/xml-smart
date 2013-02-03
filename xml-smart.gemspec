Gem::Specification.new do |s|
  s.name             = "xml-smart"
  s.version          = "0.3.2.3"
  s.platform         = Gem::Platform::RUBY
  s.summary          = "An xml library that doesn't suck - since 2004."

  s.description      = "An xml library that doesn't suck (since 2004). Based on Nokogiri since 2012. For more info check out the Documentation link below."

  s.files            = Dir['{lib/**/*,example/**/*}'] + %w(COPYING Rakefile xml-smart.gemspec README.rdoc AUTHORS)
  s.require_path     = 'lib'
  s.extra_rdoc_files = ['README.rdoc']
  s.test_files       = Dir['test/tc_*.rb','test/*.xml','test/*.rng','test/*.xsd','test/*.test','test/smartrunner.rb']

  s.authors          = ['Juergen eTM Mangler']
  s.email            = 'juergen.mangler@gmail.com'
  s.homepage         = 'http://www.wst.univie.ac.at/~mangler/xml-smart/'

  s.add_runtime_dependency 'nokogiri'
  s.add_runtime_dependency 'lockfile'
  s.add_runtime_dependency 'term-ansicolor'
  s.add_runtime_dependency 'minitest'
end
