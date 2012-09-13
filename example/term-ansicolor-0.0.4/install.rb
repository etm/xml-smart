#!/usr/bin/env ruby

require 'rbconfig'
include Config
require 'fileutils'
include FileUtils::Verbose

libdir = CONFIG["sitelibdir"]
dest = File.join(libdir, 'term')
mkdir_p dest
install('lib/term/ansicolor.rb', dest)
  # vim: set et sw=2 ts=2:
