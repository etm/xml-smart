require 'rubygems'
require 'rake/clean'
require 'rake/rdoctask'
require 'rake/testtask'
require 'rake/gempackagetask'
require 'rdoc/rdoc'

temp = `git ls-files`.split(/\r?\n/).collect{ |i| i unless i=~/^\.git/ }.compact
temp.each do |f|
  t = File.dirname(f)
  temp << t unless t == '.'
end  

PKG_FILE    = 'ruby-xml-smart'
PKG_FILES   = temp
PKG_VERSION = File.read('rbxs.h').to_s.match(/RBXS_VERSION\s+"([^"]+)"/)[1]
PKG_CURRENT = PKG_FILE + "-" + PKG_VERSION

task :default => [:config, :compile]

desc "Configuring library"
task :config do
  unless File.exists?('extconf.h')
    ruby 'extconf.rb'
  end  
end  

desc "Compiling library"
task :compile => :config do
  system "make"
end

desc "Installing library"
task :install => [:compile,:doc] do
  print "ROOT "
  system "su -c 'make install'"
end

desc "Clean directory"
task :clean do
  if File.exists?(".git")
    Dir.glob("**/*").collect{ |i| i unless i=~/^\.git/ || i=~/^rdoc/ || i=~/^extconf/ || i=~/^Makefile/ }.compact.each do |i|
      if !PKG_FILES.include?(i)
        rm_r i if File.exists?(i) 
      end
    end
  else
    system "make clean"
  end  
end  
task :distclean do
  if File.exists?(".git")
    Dir.glob("**/*").collect{ |i| i unless i=~/^\.git/ }.compact.each do |i|
      if !PKG_FILES.include?(i)
        rm_r i if File.exists?(i) 
      end
    end
  else
    system "make distclean"
  end   
end

spec = Gem::Specification.new do |s| 
  s.name = PKG_FILE
  s.version = PKG_VERSION
  s.author = "Jürgen Mangler"
  s.email = "juergen.mangler@univie.ac.at"
  s.homepage = "http://raa.ruby-lang.org/project/ruby-xml-smart/"
  s.platform = Gem::Platform::CURRENT
  s.summary = 'easy to use and stable libxml2 binding'
  s.files = PKG_FILES
  s.test_files = FileList["{test}/*_test.rb"].to_a
  s.require_path = ['.','lib']
  s.has_rdoc = true
  s.extensions = 'extconf.rb'
  s.autorequire = 'smart'
  s.rdoc_options  << '--charset=utf8' << '--line-numbers' << '--inline'
  s.extra_rdoc_files = ["README"]
  s.required_ruby_version = '>= 1.8.6'
end
 
Rake::GemPackageTask.new(spec) do |pkg| 
  pkg.need_tar = true 
end

desc "Generating RDoc documentation"
Rake::RDocTask.new(:doc) do |rdoc|
  rdoc.options << '--charset=utf8' << '--line-numbers' << '--inline'
  rdoc.main = 'README'
  rdoc.rdoc_dir = 'rdoc'
  rdoc.rdoc_files.include 'README'
  rdoc.rdoc_files.include 'rbxs.c'
  rdoc.rdoc_files.include 'rbxs_dom.c'
  rdoc.rdoc_files.include 'rbxs_qname.c'
  rdoc.rdoc_files.include 'rbxs_domelement.c'
  rdoc.rdoc_files.include 'rbxs_domnodeset.c'
  rdoc.rdoc_files.include 'rbxs_domattribute.c'
  rdoc.rdoc_files.include 'rbxs_domattributeset.c'
  rdoc.rdoc_files.include 'rbxs_domnamespace.c'
  rdoc.rdoc_files.include 'rbxs_domnamespaceset.c'
  rdoc.rdoc_files.include 'rbxs_domtext.c'
  rdoc.rdoc_files.include 'rbxs_domother.c'
  rdoc.rdoc_files.include 'rbxs_pull.c'
  rdoc.rdoc_files.include 'rbxs_pullattributeset.c'
  rdoc.rdoc_files.include 'rbxs_pullattribute.c'
end

Rake::TestTask.new do |t|
  t.libs << "test"
  t.pattern = "test/*_test.rb"
  t.verbose = true
end
