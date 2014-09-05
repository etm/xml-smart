require 'rubygems'
require 'nokogiri'
require 'tempfile'

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
require File.expand_path(File.dirname(__FILE__) + '/smart_processinginstruction')

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

      def xpath_fast(path)
        ns = self.custom_namespace_prefixes.merge(self.document.user_custom_namespace_prefixes)
        ctx = XPathContext.new(self)
        ctx.register_namespaces(self.document.custom_namespace_prefixes.merge(self.document.user_custom_namespace_prefixes))
        path = path.gsub(/xmlns:/, ' :') unless Nokogiri.uses_libxml?
        ctx.evaluate(path)
      end

      def custom_namespace_prefixes_update
        result = {}

        diffs = []
        self.xpath('//namespace::*')
        ns = self.xpath('//namespace::*').to_a.delete_if do |n| 
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
        @custom_namespace_prefixes = result
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
        # return xpath(path,self.document.custom_namespace_prefixes.merge(self.document.user_custom_namespace_prefixes))
        return NodeSet.new(document) unless document

        ns = self.document.custom_namespace_prefixes.merge(self.document.user_custom_namespace_prefixes)
        ctx = XPathContext.new(self)
        ctx.register_namespaces(self.document.custom_namespace_prefixes.merge(self.document.user_custom_namespace_prefixes))
        path = path.gsub(/xmlns:/, ' :') unless Nokogiri.uses_libxml?
        ctx.evaluate(path)
      end

      def xpath_experimental
        return NodeSet.new(document) unless document
        @nsc ||= 0
        if @nsc != self.document.ns_counter
          @ctx = XPathContext.new(self)
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
  module Smart
    VERSION        = File.read(File.expand_path(File.dirname(__FILE__) + '/../../xml-smart.gemspec')).match(/\.version\s*=[^\n]+?([\d\.]+)[^\n]+\n/)[1]
    LIBXML_VERSION = Nokogiri::VERSION_INFO['libxml']['loaded']
    MUTEX = Mutex.new
    COPY = 0
    MOVE = 1

    def initialize(name,default=nil); open(name,default); end

    def self::modify(name,default=nil,&block)
      raise Error, 'first parameter has to be a filename or filehandle' unless name.is_a?(String) || name.is_a?(IO) || name.is_a?(Tempfile)
      raise Error, 'a block is mandatory' unless block_given?
      dom = io = nil
      begin
        if name.is_a?(String) && File.exists?(name)
          MUTEX.synchronize do
            io = ::Kernel::open(name,'r+')
            io.flock(File::LOCK_EX)
          end  
          dom = Dom.new Nokogiri::XML::parse(io){|config| config.noblanks.noent.nsclean.strict }
          io.rewind
        elsif name.is_a?(String) && !File.exists?(name)
          MUTEX.synchronize do
            io = ::Kernel::open(name,'w')
            io.flock(File::LOCK_EX)
          end  
          dom = Smart::string(default)
        elsif name.is_a?(IO) || name.is_a?(Tempfile)  
          MUTEX.synchronize do
            io = name
            io.flock(File::LOCK_EX)
          end
          dom = Dom.new Nokogiri::XML::parse(io){|config| config.noblanks.noent.nsclean.strict }
          io.rewind
        end
        block.call(dom)
        dom.save_as(io)
      rescue => e
        puts e.message
        raise Error, "could not open #{name}"
      ensure
        if io
          io.flush
          io.truncate(io.pos)
          io.flock(File::LOCK_UN)
          io.close if name.is_a?(String)
        end
      end
      nil
    end

    def self::open(name,default=nil)
      raise Error, 'first parameter has to be a filename or filehandle' unless name.is_a?(String) || name.is_a?(IO) || name.is_a?(Tempfile)
      raise Error, 'second parameter has to be an xml string' unless default.is_a?(String) || default.nil?
      dom = Smart::open_unprotected(name,default,true)
      if dom && block_given?
        yield dom
        nil
      else  
        dom
      end
    end

    def self::open_unprotected(name,default=nil,lock=false)
      raise Error, 'first parameter has to be a filename or filehandle' unless name.is_a?(String) || name.is_a?(IO) || name.is_a?(Tempfile)
      raise Error, 'second parameter has to be an xml string' unless default.is_a?(String) || default.nil?
      dom = begin
        io = name.is_a?(String) ? ::Kernel::open(name) : name
        begin
          io.flock(File::LOCK_EX) if lock
          Dom.new Nokogiri::XML::parse(io){|config| config.noblanks.noent.nsclean.strict }
        ensure
          io.flock(File::LOCK_UN)
        end
      rescue => e
        if default.nil?
          puts e.message
          raise Error, "could not open #{name}"
        else
          Smart::string(default)
        end
      end
      if block_given?
        yield dom
        nil
      else  
        dom
      end
    end

    def self::string(str)
      raise Error, 'first parameter has to be stringable (:to_s)' unless str.is_a?(String)
      dom = Dom.new Nokogiri::XML::parse(str.to_s){|config| config.noblanks.noent.nsclean.strict }
      if block_given?
        yield dom
        nil
      else  
        dom
      end
    end

    class Error < RuntimeError; end
  end
end
