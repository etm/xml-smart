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
      def ns_counter
        @ns_counter ||= 1
      end
      def ns_update
        @ns_counter ||= 1
        @ns_counter += 1
        self
      end

      def custom_namespace_prefixes_update
        result = {}

        diffs = []
        ns = self.xpath_plain('//namespace::*').to_a.delete_if do |n| 
          if diffs.include? n.href
            true
          else  
            diffs.push(n.href).uniq!
            false
          end  
        end 

        de = ns.find_all{|n| n.prefix.nil?}
        if de.length == 1
          result['xmlns'] = de[0].href
        end
        if de.length > 1
          i = 0
          de.each do |n|
            unless result.has_value? n.href
              result["xmlns#{i}"] = n.href
              i += 1
            end  
          end
        end

        ns.find_all{|n| !n.prefix.nil? && !(n.prefix == 'xml')}.each do |n|
          result[n.prefix] = n.href
        end
        result
      end
      def custom_namespace_prefixes
        @custom_namespace_prefixes || custom_namespace_prefixes_update
      end
      def user_custom_namespace_prefixes
        @user_custom_namespace_prefixes ||= {}
      end
      def user_custom_namespace_prefixes=(h)
        @user_custom_namespace_prefixes = h
      end
    end

    class Node
      def xpath_plain(path)
        XPathContext.new(self).evaluate(path)
      end
      def xpath_fast(path)
        @nsc ||= 0
        if @nsc != self.document.ns_counter
          @ctx ||= XPathContext.new(self)
          @ctx.register_namespaces(self.document.custom_namespace_prefixes.merge(self.document.user_custom_namespace_prefixes))
          @nsc = self.document.ns_counter
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
