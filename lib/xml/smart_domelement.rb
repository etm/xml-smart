module XML
  module Smart
    class Dom

      class Element
        def initialize(element)
          @node = element
        end

        def ===(cls); self.is_a? cls; end

        def find(xpath)
          Dom::smart_helper(@node.xpath_fast(xpath))
        end

        def add_helper(attrs)
          if attrs.length>0 && attrs[0].is_a?(String) && attrs[0][0] != '?'
            pfx = ''
            ns = nil
            attrs[0] = attrs[0].dup
            if attrs[0].sub!(/([^:]+):/, '')
              pfx = $1
              if @node.document.user_custom_namespace_prefixes.has_key?(pfx)
                @node.document.custom_namespace_prefixes.each do |k,v|
                  if @node.document.user_custom_namespace_prefixes[pfx] == v
                    ns = @node.document.user_custom_namespace_prefixes[pfx]
                  end
                end
              end
              if ns.nil?
                if @node.document.custom_namespace_prefixes.has_key?(pfx)
                  ns = @node.document.custom_namespace_prefixes[pfx]
                else
                  raise Error, 'No valid namespace'
                end
              end
            end
            tmp = Nokogiri::XML::Node.new attrs[0], @node.document
            unless ns.nil?
              @node.namespace_scopes.each do |nss|
                tmp.namespace = nss if nss.href == ns
              end
            end
            [1,2].each do |i|
              if attrs.length > i
                case attrs[i]
                  when Hash
                    attrs[i].each do |k,v|
                      if k.respond_to?(:to_s)
                        tmp[k.to_s] = v.respond_to?(:to_s) ? v.to_s : ''
                      else
                        raise Error, 'use something that has provides #to_s'
                      end
                    end
                  else
                    tmp.content = attrs[i].respond_to?(:to_s) ? attrs[i].to_s : ''
                end
              end
            end
            return [tmp,false]
          elsif attrs.length == 1 && attrs[0].is_a?(XML::Smart::Dom::Element)
            ele = attrs[0].instance_variable_get(:@node)
            same = ele.document.root.pointer_id == @node.document.root.pointer_id
            return [same ? ele : ele.dup, !same]
          elsif attrs.length == 1 && attrs[0].is_a?(XML::Smart::Dom::NodeSet)
            nos = attrs[0].instance_variable_get(:@nodeset)
            if nos.length > 0
              same = nos.first.document.root.pointer_id == @node.document.root.pointer_id
              if same
                return [nos, false]
              else
                tnos = nos.map{|e|e.dup}
                return [Nokogiri::XML::NodeSet.new(nos.first.document,tnos), true]
              end
            else
              return [nos, false]
            end
          elsif attrs.length == 2 && attrs[0].is_a?(XML::Smart::Dom::Element) && (attrs[1] == XML::Smart::COPY || attrs[1] == XML::Smart::MOVE)
            ele = attrs[0].instance_variable_get(:@node)
            same = ele.document.root.pointer_id == @node.document.root.pointer_id
            return [attrs[1] == XML::Smart::COPY ? ele.dup : ele, !same]
          elsif attrs.length == 2 && attrs[0].is_a?(XML::Smart::Dom::NodeSet) && (attrs[1] == XML::Smart::COPY || attrs[1] == XML::Smart::MOVE)
            nos = attrs[0].instance_variable_get(:@nodeset)
            if nos.length > 0
              same = nos.first.document.root.pointer_id == @node.document.root.pointer_id
              if attrs[1] == XML::Smart::COPY
                tnos = nos.map{|e|e.dup}
                nos = Nokogiri::XML::NodeSet.new(nos.first.document,tnos)
              end
              return [nos, !same]
            else
              return [nos, false]
            end
          elsif attrs.length == 2 && attrs[0].is_a?(String) && attrs[1].is_a?(String) && attrs[0][0] == '?'
            tmp = Nokogiri::XML::ProcessingInstruction.new @node.document, attrs[0].sub(/./,''), attrs[1]
            return [tmp,false]
          end
          return [nil, false]
        end
        private :add_helper
        def add(*attrs)
          tmp, update = add_helper(attrs)
          res = Dom::smart_helper(@node.add_child tmp)
          if update
            @node.document.custom_namespace_prefixes_update
            @node.document.ns_update
          end
          res
        end
        def append(*attrs)
          add(*attrs)
        end
        def prepend(*attrs)
          c = children
          c.empty? ? add(*attrs) : c.first.add_before(*attrs)
        end
        def add_before(*attrs)
          tmp, update = add_helper(attrs)
          res = Dom::smart_helper(@node.add_previous_sibling tmp)
          if update
            @node.document.custom_namespace_prefixes_update
            @node.document.ns_update
          end
          res
        end
        def add_after(*attrs)
          tmp, update = add_helper(attrs)
          res = Dom::smart_helper(@node.add_next_sibling tmp)
          if update
            @node.document.custom_namespace_prefixes_update
            @node.document.ns_update
          end
          res
        end

        def dump
          doc = Nokogiri::XML::Document.new
          doc.root = @node
          doc.root.to_s
        end

        def to_s; @node.content; end
        def to_i; @node.content.to_i; end
        def to_f; @node.content.to_f; end

        def namespace?; !@node.namespace.nil?; end
        def namespace; namespace? ? Namespace.new(@node.namespace) : nil; end
        def namespace=(n)
          n = case n
            when Namespace
              n.prefix
            when String
              n
            else
              return
          end
          tmp = @node.document.custom_namespace_prefixes[n] || @node.document.user_custom_namespace_prefixes[n]
          unless tmp.nil?
            @node.namespace_scopes.each do |nss|
              @node.namespace = nss if nss.href == tmp
            end
          end
        end
        def namespaces; NamespaceSet.new(self,@node); end

        def xinclude!(basedir=nil)
          @node.do_xinclude_manual(basedir)
          @node.document.custom_namespace_prefixes_update
          @node.document.ns_update
          true
        end

        def replace_by(n)
          case n
            when Element; Element.new @node.replace(n.instance_variable_get(:@node).dup)
            when NodeSet; NodeSet.new @node.replace(n.instance_variable_get(:@nodeset).dup)
            else
              nil
          end
        end

        def qname; QName.new @node; end
        def attributes; AttributeSet.new @node; end

        def text; @node.xpath_fast("string(text())"); end
        def text=(t); @node.content = t.to_s if t.respond_to? :to_s; end

        def children; find('*|text()'); end
        def children?; find('*|text()').length > 0 end
        def parent
          Dom::smart_helper(@node.parent)
        end
        def parent?; !@node.parent.nil?; end

        def empty?; !children?; end
        def mixed?;
          @node.xpath_fast('*').length > 0 &&  @node.xpath_fast("string(text())") != '';
        end
        def text_only?;
          @node.xpath_fast('*').length == 0 &&  @node.xpath_fast("string(text())") != '';
        end
        def element_only?;
          @node.xpath_fast('*').length > 0 &&  @node.xpath_fast("string(text())") == '';
        end

        def path; @node.path[-1] != ']' ? @node.path + '[1]' : @node.path; end

        def ==(other)
          return false unless other
          return false unless other.respond_to?(:unique_id)
          unique_id == other.unique_id
        end
        def unique_id; @node.pointer_id; end

        def to_doc
          doc = Nokogiri::XML::Document.new
          doc.root = @node
          Dom.new(doc)
        end
      end

    end
  end
end
