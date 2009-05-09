require 'mkmf'
require 'rbconfig'
require 'pp'

PKG_NAME = 'xml/smart'

include Config

unless File.exists?('extconf.h')
  $CFLAGS  = '-g -Wall ' + $CFLAGS
  $LIBPATH.push(Config::CONFIG['libdir'])
  unless have_library("z", "inflate")
    puts "Error: zlib is needed"
    exit 1
  else
    $defs.push('-DHAVE_ZLIB_H')
  end
  unless have_library("xml2", "xmlXPtrNewRange")
    puts "Error: libxml2 is needed"
    puts "  Install libxml2 library or try one of the following options to extconf.rb:"
    puts "    --with-xml2-dir=/path/to/libxml2"
    puts "    --with-xml2-lib=/path/to/libxml2/lib"
    puts "    --with-xml2-include=/path/to/libxml2/include"
    exit 1
  end
  $LDFLAGS << ' ' + `xml2-config --libs`.chomp
  $CFLAGS  << ' ' + `xml2-config --cflags`.chomp
  unless have_library("xslt", "xsltParseStylesheetFile")
    puts "Error: libxslt is needed"
    puts "  Install libxslt library or try one of the following options to extconf.rb:"
    puts "    --with-xslt-dir=/path/to/libxslt"
    puts "    --with-xslt-lib=/path/to/libxslt/lib"
    puts "    --with-xslt-include=/path/to/libxslt/include"
    exit 1
  end
  $LDFLAGS << ' ' + `xslt-config --libs`.chomp
  $CFLAGS  << ' ' + `xslt-config --cflags`.chomp
  if enable_config("exslt", true)
    have_library "exslt", "exsltRegisterAll"
    $CFLAGS << ' -DUSE_EXSLT'
  end
  create_header()
  create_makefile(PKG_NAME,".")
end
