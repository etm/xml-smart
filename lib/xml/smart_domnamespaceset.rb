module XML
  module Smart
    class Dom

      class NamespaceSet
        include Enumerable

        def initialize(parent,node)
          @parent = parent
          @node = node
        end

        def ===(cls); self.is_a? cls; end

        def has_ns?(a); @node.namespace_definitions.one?{|e| e.href == a || e.prefix == a}; end
        alias :include? :has_ns?
        alias :ns? :has_ns?
        alias :member? :has_ns?

        def [](name)
          if name.nil? || name.respond_to?(:to_s)
            name = name.to_s unless name.nil?
            name = nil if name =~ /^xmlns\d*$/
            nd = @node.namespace_definitions.find{|n| n.prefix == name}
            (name.respond_to?(:to_s) || name.nil?) && nd ? Namespace.new(nd) : nil; 
          end  
        end
        def []=(name,value) 
          name = nil if name == 'xmlns'
          self.add(name,value)
        end

        def add(name,value)
          if (name.respond_to?(:to_s) || name.nil?) && value.respond_to?(:to_s) 
            nnod = Nokogiri::XML::Node.new(@node.name,@node.document)
            nnew = Element.new(nnod)
            nold = Element.new(@node)
            nold.attributes.each do |attr|
              nnew.attributes[attr.qname.name] = attr.value
            end
            ns = nnod.add_namespace_definition(name.nil? ? nil : name.to_s,value.to_s)
            @node.namespace_definitions.each do |ns|
              nnod.add_namespace_definition(ns.prefix,ns.href)
            end
            nnew.add(nold.children)
            nold.replace_by(nnew)
            @node = nnod
            @parent.instance_variable_set(:@element,@node) 
            @node.document.custom_namespace_prefixes_update
            @node.document.ns_update
            Namespace.new(ns)
          end  
        end

        def length;      @node.namespace_definitions.length; end
        def empty?;      @node.namespace_definitions.empty?; end
        def delete_all!
          nnod = Nokogiri::XML::Node.new(@node.name,@node.document)
          nnew = Element.new(nnod)
          nold = Element.new(@node)
          nold.attributes.each do |attr|
            nnew.attributes[attr.qname.name] = attr.value
          end
          nnew.add(nold.children)
          nold.replace_by(nnew)
          @node = nnod
          @parent.instance_variable_set(:@element,@node)
          @node.document.custom_namespace_prefixes_update
          @node.document.ns_update
          nil
        end

        def each(&block)
          @node.namespace_definitions.each do |ns|
            block.call Namespace.new(ns)
          end
        end

        def delete_at(name)
          if (name.respond_to?(:to_s) || name.nil?)
            return false unless @node.namespace_definitions.one?{|e| e.prefix == name}

            nnod = Nokogiri::XML::Node.new(@node.name,@node.document)
            nnew = Element.new(nnod)
            nold = Element.new(@node)
            nold.attributes.each do |attr|
              nnew.attributes[attr.qname.name] = attr.value
            end
            @node.namespace_definitions.each do |ns|
              nnod.add_namespace_definition(ns.prefix,ns.href) unless ns.prefix == name
            end
            nnew.add(nold.children)
            nold.replace_by(nnew)
            @node = nnod
            @parent.instance_variable_set(:@element,@node)
            @node.document.custom_namespace_prefixes_update
            @node.document.ns_update
            true
          else
            false
          end
        end

        def delete_if(&block)
          return self if block.nil?
          nnod = Nokogiri::XML::Node.new(@node.name,@node.document)
          nnew = Element.new(nnod)
          nold = Element.new(@node)
          nold.attributes.each do |attr|
            nnew.attributes[attr.qname.name] = attr.value
          end
          @node.namespace_definitions.each do |ns| 
            nnod.add_namespace_definition(ns.prefix,ns.href) unless block.call(Dom::smart_helper(ns))
          end
          nnew.add(nold.children)
          nold.replace_by(nnew)
          @node = nnod
          @parent.instance_variable_set(:@element,@node)
          @node.document.custom_namespace_prefixes_update
          @node.document.ns_update
          self
        end
      end  
    
    end
  end  
end  
