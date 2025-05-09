## XML::Smart - A Ruby class for fast and simple XML access

Copyright (C) 2004-2012 Jürgen Mangler <juergen.mangler@univie.ac.at>

Ruby/XML/Smart is freely distributable according to the terms of the GNU Lesser General Public License (see the file 'COPYING').

This program is distributed without any warranty. See the file 'COPYING' for details.

## Installation

- Based on Nokogiri 1.5.5
- Thus works for MRI ruby 1.8.7, 1.9.x, JRuby (basically everything that is Nokogiri ready)
- Works on Linux, OSX, Windows


Fedora:
```bash
  sudo dnf install @buildsys-build @development-tools @c-development ruby-devel libxml2-devel libxslt-devel zlib-devel libicu-devel libffi-devel
```

Ubuntu:
```bash
  sudo apt install build-essential ruby-dev libxml2-dev libxslt-dev libz-dev libssl-dev libicu-dev
```

Afterwards:
```bash
  gem install --user xml-smart
```

## Features

### Principle of least surprise

- Nokogiri is messy - violence always is
- libxml smells like old-school C-Binding ;-)
- rex is ... strange

### Inherits Nokogiris GREAT compatibility for platforms and Ruby interpreters

### XML only

Sorry, no css selectors, html, ... go to Nokogiri if you look for these.

### Great and convenient namespace support, compared to everything else out there

Use namespaces in xpaths without any additional work:

```ruby
  require 'xml/smart'

  doc = XML::Smart.string('<test xmlns:aaa="uri:aaa"><aaa:test/></test>')
  doc.find('string(aaa:test)')
  doc.close
```

Register your own shortcusts to be available in all XPaths:

```ruby
  require 'xml/smart'

  doc = XML::Smart.string('<test xmlns:aaa="uri:aaa"><aaa:test/></test>')
  doc.register_namespace :a, 'uri:aaa'
  doc.find('string(a:test)')
  doc.close
```

### NFS safe file locking while editing / reading from an XML file

```ruby
  XML::Smart.modify("EXAMPLE.xml","<test><names/></test>") do |doc|
    doc.root.add "test_node"
  end
```

### check against relaxng and xml schema

Libxml2, the basis for nokogiri has only partial xml-schema support, but full
relaxng support. Thus checking against xml-schema with nokogiri may return
positive results, although it is a lie. XML::Smart internally converts
xml-schema to relaxng, thus allowing for seamless schema usage:

```ruby
  require 'xml/smart'

  doc = XML::Smart.string('<test xmlns:aaa="uri:aaa"><aaa:test/></test>')
  doc.validate_against(XML::Smart.open_unprotected('xmlschema.xml'))
  doc.validate_against(XML::Smart.open_unprotected('xmlschema.xml'))
  doc.find('string(a:test)')
  doc.close
```

### xinclude

Libxml2, the basis for nokogiri does not support https xincludes. Nokogiri may impelement this in the future (issue
https://github.com/sparklemotion/nokogiri/issues/1321), but for now we do support it:

```ruby
  doc.xinclude!
```

or

```ruby
  doc.find('//someelement').first.xinclude!
```

## Changes since 0.2.x (see Changelog)

- qname instead of name
- #register_namespace instead of #namespaces= to register shortcuts
- no more namespaces for #find necessary, all namespaces in document are automatically available.
- signals removed
- pull parser removed

## Documentation

Sorry, no inline code documentation yet. We have an extensive test suite, look for examples there for now.
