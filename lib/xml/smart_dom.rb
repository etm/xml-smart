module XML
  module Smart
    class Dom
      def initialize(dom)
        @dom = dom
        @save_unformated = false
      end

      def ===(cls); self.is_a? cls; end

      def root
        Element.new(@dom.root)
      end
      def root=(nr)
        @dom.root.replace(nr.instance_variable_get(:@element)) if nr.instance_of? Element
      end

      def find(path)
        Element.new(@dom.root).find(path)
      end

      def to_s
        @dom.to_s
      end

      def namespaces
        @dom.custom_namespace_prefixes.merge @dom.user_custom_namespace_prefixes
      end
      def register_namespace(a,b)
        if a.respond_to?(:to_s) && b.respond_to?(:to_s) && !@dom.custom_namespace_prefixes.key?(a.to_s)
          @dom.user_custom_namespace_prefixes[a.to_s] = b.to_s
          @dom.ns_update
        end
      end
      def unregister_namespace(a)
        if a.respond_to?(:to_s) && @dom.user_custom_namespace_prefixes.key?(a.to_s)
          @dom.user_custom_namespace_prefixes.delete(a.to_s)
          @dom.ns_update
        end
      end

      def save_as(name)
        raise Error, 'first parameter has to be a filename or filehandle' unless name.is_a?(String) || name.is_a?(IO)
        begin
          io = name.is_a?(String) ? ::Kernel::open(name,'w') : name
        rescue
          raise Error, "could not open #{name}"
        end
        ftext = if @save_unformated
          @dom.root.serialize(:encoding => 'UTF-8', :save_with => Nokogiri::XML::Node::SaveOptions::NO_DECLARATION | Nokogiri::XML::Node::SaveOptions::AS_XML)
        else
          @dom.root.serialize(:encoding => 'UTF-8', :save_with => Nokogiri::XML::Node::SaveOptions::FORMAT | Nokogiri::XML::Node::SaveOptions::AS_XML)
        end
        io.write ftext
        io.close
      end

      def save_unformated=(val); @save_unformated = (val.is_a?(TrueClass) ? true : false); end
      def save_unformated?; @save_unformated; end

      def xinclude!
        Element.new(@dom.root).xinclude!
      end  

      def self::smart_helper(node)
        if node.instance_of? Nokogiri::XML::Element
          Element.new(node)
        elsif node.instance_of? Nokogiri::XML::Attr
          Attribute.new(node)
        elsif node.instance_of? Nokogiri::XML::NodeSet  
          NodeSet.new(node)
        elsif node.instance_of?(String) || node.instance_of?(TrueClass) || node.instance_of?(FalseClass) || node.instance_of?(Float)  
          node
        elsif node.instance_of? Nokogiri::XML::Text  
          Text.new(node)
        elsif node.instance_of? Nokogiri::XML::Namespace  
          Namespace.new(node)
        else
          Other.new(node)
        end  
      end

      def validate_against(doc)
        raise Error, 'first parameter has to XML::Smart::Dom document' unless doc.instance_of? Dom
        res = if doc.root.namespaces.has_ns?("http://relaxng.org/ns/structure/1.0")
          Nokogiri::XML::RelaxNG.from_document(doc.instance_variable_get(:@dom)).validate(@dom)
        elsif doc.root.namespaces.has_ns?("http://www.w3.org/2001/XMLSchema")
          tdoc = Nokogiri::XSLT.parse(File.read(File.expand_path(File.dirname(__FILE__) + '/XSDtoRNG.xsl')))
          rdoc = tdoc.transform(doc.instance_variable_get(:@dom))
          Nokogiri::XML::RelaxNG.from_document(rdoc).validate(@dom)
        end
        res.each { |err| $stderr.puts "#{err.file} line #{err.line}: #{err.to_s}" }
        res.empty?
      end

      def transform_with(doc,params=nil)
        raise Error, 'first parameter has to XML::Smart::Dom document' unless doc.instance_of? Dom
        Dom.new Nokogiri::XSLT::Stylesheet.parse_stylesheet_doc(doc.instance_variable_get(:@dom)).transform(@dom,params)
      end

    end
  end
end
