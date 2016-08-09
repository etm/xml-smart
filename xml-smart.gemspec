Gem::Specification.new do |s|
  s.name                  = "xml-smart"
  s.version               = "0.3.19"
  s.platform              = Gem::Platform::RUBY
  s.license               = "LGPL-3.0"
  s.summary               = "An xml library that doesn't suck - since 2004."

  s.description           = "An xml library that doesn't suck (since 2004). Based on Nokogiri since 2012. For more info check out the Documentation link below."

  s.files                 = Dir['{lib/**/*,example/**/*}'] + %w(COPYING Rakefile xml-smart.gemspec README.rdoc AUTHORS)
  s.require_path          = 'lib'
  s.extra_rdoc_files      = ['README.rdoc']
  s.test_files            = Dir['test/tc_*.rb','test/*.xml','test/*.rng','test/*.xsd','test/smartrunner.rb','minitest/*']

  s.authors               = ['Juergen eTM Mangler']
  s.email                 = 'juergen.mangler@gmail.com'
  s.homepage              = 'http://www.wst.univie.ac.at/~mangler/xml-smart/'

  s.required_ruby_version = '>=1.9.3'

  s.add_runtime_dependency 'nokogiri', '~> 1', '>= 1.6.6'
  s.add_runtime_dependency 'term-ansicolor', '~> 1', '>= 1.3'
  s.add_runtime_dependency 'minitest', '~> 5', '>= 5.0.0'
end
