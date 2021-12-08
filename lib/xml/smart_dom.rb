module XML
  module Smart
    class Dom
      def initialize(dom,basepath=nil)
        @dom = dom
        @dom.basepath = basepath
        @unformated = false
      end

      def ===(cls); self.is_a? cls; end

      def root
        Element.new(@dom.root)
      end
      def root=(nr)
        @dom.root.replace(nr.instance_variable_get(:@node)) if nr.instance_of? Element
      end

      def find(path)
        Dom::smart_helper(@dom.xpath_fast(path))
      end

      def to_s
        @dom.to_s
      end

      def namespaces
        @dom.custom_namespace_prefixes.merge @dom.user_custom_namespace_prefixes
      end
      def register_namespace(a,b)
        if a.respond_to?(:to_s) && b.respond_to?(:to_s) && !@dom.custom_namespace_prefixes.key?(a.to_s) && @dom.custom_namespace_prefixes.value?(b.to_s)
          @dom.user_custom_namespace_prefixes[a.to_s] = b.to_s
          @dom.ns_update
          true
        else
          false
        end
      end
      def unregister_namespace(a)
        if a.respond_to?(:to_s) && @dom.user_custom_namespace_prefixes.key?(a.to_s)
          @dom.user_custom_namespace_prefixes.delete(a.to_s)
          @dom.ns_update
          true
        else
          false
        end
      end

      def save_as(name)
        raise Error, 'first parameter has to be a filename or filehandle' unless name.is_a?(String) || name.is_a?(IO) || name.is_a?(Tempfile)
        begin
          io = name.is_a?(String) ? ::URI::open(name,'w') : name
        rescue
          raise Error, "could not open #{name}"
        end
        io.write serialize
        io.close unless name == io
      end

      def serialize
        if @unformated
          @dom.root.serialize(:encoding => 'UTF-8', :save_with => Nokogiri::XML::Node::SaveOptions::NO_DECLARATION | Nokogiri::XML::Node::SaveOptions::AS_XML)
        else
          @dom.root.serialize(:encoding => 'UTF-8', :save_with => Nokogiri::XML::Node::SaveOptions::FORMAT | Nokogiri::XML::Node::SaveOptions::AS_XML)
        end
      end

      def unformated=(val); @unformated = (val.is_a?(TrueClass) ? true : false); end
      def unformated?; @unformated; end

      def xinclude!(basedir=nil)
        Element.new(@dom.root).xinclude!(basedir)
      end

      def self::smart_helper(node)
        if node.instance_of? Nokogiri::XML::Element
          XML::Smart::Dom::Element.new(node)
        elsif node.instance_of? Nokogiri::XML::Attr
          XML::Smart::Dom::Attribute.new(node)
        elsif node.instance_of? Nokogiri::XML::NodeSet
          XML::Smart::Dom::NodeSet.new(node)
        elsif node.instance_of?(String) || node.instance_of?(TrueClass) || node.instance_of?(FalseClass) || node.instance_of?(Float)
          node
        elsif node.instance_of? Nokogiri::XML::Text
          XML::Smart::Dom::Text.new(node)
        elsif node.instance_of? Nokogiri::XML::Namespace
          XML::Smart::Dom::Namespace.new(node)
        elsif node.instance_of? Nokogiri::XML::Document
          XML::Smart::Dom.new(node)
        elsif node.instance_of? Nokogiri::XML::ProcessingInstruction
          XML::Smart::ProcessingInstruction.new(node)
        elsif node.nil?
          nil
        else
          XML::Smart::Dom::Other.new(node)
        end
      end

      def validate_against(doc,&errbl)
        raise Error, 'first parameter has to XML::Smart::Dom document' unless doc.instance_of? Dom
        res = if doc.root.namespaces.has_ns?("http://relaxng.org/ns/structure/1.0")
          Nokogiri::XML::RelaxNG.from_document(doc.instance_variable_get(:@dom)).validate(@dom)
        elsif doc.root.namespaces.has_ns?("http://www.w3.org/2001/XMLSchema")
          tdoc = Nokogiri::XSLT.parse(File.read(File.expand_path(File.dirname(__FILE__) + '/XSDtoRNG.xsl')))
          rdoc = tdoc.transform(doc.instance_variable_get(:@dom))
          Nokogiri::XML::RelaxNG.from_document(rdoc).validate(@dom)
        end
        res.each { |err| errbl.call err } if block_given?
        res.empty?
      end

      def transform_with(doc,params=nil)
        raise Error, 'first parameter has to XML::Smart::Dom document' unless doc.instance_of? Dom
        res = Nokogiri::XSLT::Stylesheet.parse_stylesheet_doc(doc.instance_variable_get(:@dom)).transform(@dom,params)
        if res.children.length != 0 && res.children.first.class == Nokogiri::XML::Text
          Text.new(res.children.first).text
        else
          Dom::smart_helper(res)
        end
      end

    end
  end
end
