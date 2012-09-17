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

        def [](name); name.respond_to?(:to_s) && @node.namespace_definitions.find{|n| n.prefix == name.to_s} ? Namespace.new(@node.namespace_definitions.find{|n| n.prefix == name.to_s}) : nil; end;
        def []=(name,value) 
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
            ns = nnod.add_namespace_definition(name,value)
            @node.namespace_definitions.each do |ns|
              nnod.add_namespace_definition(ns.prefix,ns.href)
            end
            nnew.add(nold.children)
            nold.replace_by(nnew)
            @node = nnod
            @parent.instance_variable_set(:@element,@node) 
            update_namespace_prefixes
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
          update_namespace_prefixes
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
            update_namespace_prefixes
            true
          else
            false
          end
        end

        def delete_if(&block)
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
          update_namespace_prefixes
          self
        end

        def update_namespace_prefixes
          @node.document.custom_namespace_prefixes = Hash[*@node.document.collect_namespaces.map{ |k,v| [k.sub(/^xmlns:/,''),v] }.flatten].merge(@node.document.user_custom_namespace_prefixes)
        end
        private :update_namespace_prefixes
      end  
    
    end
  end  
end  
