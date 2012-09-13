require 'rubygems'
require 'nokogiri'
require 'lockfile'

require File.expand_path(File.dirname(__FILE__) + '/smart_qname')
require File.expand_path(File.dirname(__FILE__) + '/smart_dom')
require File.expand_path(File.dirname(__FILE__) + '/smart_domelement')
require File.expand_path(File.dirname(__FILE__) + '/smart_domnodeset')
require File.expand_path(File.dirname(__FILE__) + '/smart_domtext')
require File.expand_path(File.dirname(__FILE__) + '/smart_domother')
require File.expand_path(File.dirname(__FILE__) + '/smart_domattributeset')
require File.expand_path(File.dirname(__FILE__) + '/smart_domattribute')
require File.expand_path(File.dirname(__FILE__) + '/smart_domnamespaceset')
require File.expand_path(File.dirname(__FILE__) + '/smart_domnamespace')

module Nokogiri
  module XML
    class Document
      def custom_namespace_prefixes
        @custom_namespace_prefixes ||= self.root.xpath_fast('//namespace::*').inject({}){|x,y| x[y.prefix] = y.href if y.prefix != 'xml'; x }
      end
      def custom_namespace_prefixes=(h)
        @custom_namespace_prefixes = h
      end
      def user_custom_namespace_prefixes
        @user_custom_namespace_prefixes ||= {}
      end
      def user_custom_namespace_prefixes=(h)
        @user_custom_namespace_prefixes = h
      end
    end

    class Node
      def xpath_fast(path,ns={})
        @oldns ||= {}
        if @oldns == ns
          @ctx ||= XPathContext.new(self)
        else  
          @ctx = XPathContext.new(self)
          @ctx.register_namespaces(ns)
          @oldns = ns
        end
        path = path.gsub(/xmlns:/, ' :') unless Nokogiri.uses_libxml?
        @ctx.evaluate(path)
      end
    end
  end
end

module XML
  VERSION        = '0.3.0'
  LIBXML_VERSION = Nokogiri::VERSION_INFO['libxml']['loaded']
  LOCKFILE = {
    :min_sleep => 0.25,
    :max_sleep => 5,
    :sleep_inc => 0.25
  }

  module Smart
    COPY = 0
    MOVE = 1

    def initialize(name,default=nil); open(name,default); end

    def self::modify(name,default=nil,&block)
      raise Error, 'first parameter has to be a filename or filehandle' unless name.is_a?(String) || name.is_a?(IO)
      raise Error, 'a block is mandatory' unless block_given?
      lfname = name.is_a?(String) ? name : name.fileno
      Lockfile.new(lfname + '.lock',LOCKFILE) do
        so = Smart::open(name,default)
        block.call(so)
        so.save_as(name)
      end
    end

    def self::open(name,default=nil)
      raise Error, 'first parameter has to be a filename or filehandle' unless name.is_a?(String) || name.is_a?(IO)
      raise Error, 'second parameter has to be an xml string' unless default.is_a?(String) || default.nil?
      begin
        io =  name.is_a?(String) ? ::Kernel::open(name) : name
        Dom.new Nokogiri::XML::parse(io){|config| config.noblanks.noent.nsclean.strict }
      rescue
        if default.nil?
          raise Error, "could not open #{name}"
        else
          Smart::string(default) unless default.nil?
        end
      end
    end

    def self::string(str)
      raise Error, 'first parameter has to be an xml string' unless name.is_a?(String)
      Dom.new Nokogiri::XML::parse(str){|config| config.noblanks.noent.nsclean.strict }
    end

    class Error < RuntimeError; end
  end
end
