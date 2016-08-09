module XML
  module Smart
    class Dom

      class Element
        def initialize(element)
          @element = element
        end

        def ===(cls); self.is_a? cls; end

        def find(xpath)
          Dom::smart_helper(@element.xpath_fast(xpath))
        end

        def add_helper(attrs)
          if attrs.length>0 && attrs[0].is_a?(String) && attrs[0][0] != '?'
            pfx = ''
            ns = nil
            attrs[0] = attrs[0].dup
            if attrs[0].sub!(/([^:]+):/, '')
              pfx = $1
              if @element.document.user_custom_namespace_prefixes.has_key?(pfx)
                @element.document.custom_namespace_prefixes.each do |k,v|
                  if @element.document.user_custom_namespace_prefixes[pfx] == v
                    ns = @element.document.user_custom_namespace_prefixes[pfx]
                  end
                end
              end
              if ns.nil?
                if @element.document.custom_namespace_prefixes.has_key?(pfx)
                  ns = @element.document.custom_namespace_prefixes[pfx]
                else
                  raise Error, 'No valid namespace'
                end
              end
            end
            tmp = Nokogiri::XML::Node.new attrs[0], @element.document
            unless ns.nil?
              @element.namespace_scopes.each do |nss|
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
            ele = attrs[0].instance_variable_get(:@element)
            same = ele.document.root.pointer_id == @element.document.root.pointer_id
            return [same ? ele : ele.dup, !same]
          elsif attrs.length == 1 && attrs[0].is_a?(XML::Smart::Dom::NodeSet)
            nos = attrs[0].instance_variable_get(:@nodeset)
            if nos.length > 0
              same = nos.first.document.root.pointer_id == @element.document.root.pointer_id
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
            ele = attrs[0].instance_variable_get(:@element)
            same = ele.document.root.pointer_id == @element.document.root.pointer_id
            return [attrs[1] == XML::Smart::COPY ? ele.dup : ele, !same]
          elsif attrs.length == 2 && attrs[0].is_a?(XML::Smart::Dom::NodeSet) && (attrs[1] == XML::Smart::COPY || attrs[1] == XML::Smart::MOVE)
            nos = attrs[0].instance_variable_get(:@nodeset)
            if nos.length > 0
              same = nos.first.document.root.pointer_id == @element.document.root.pointer_id
              if attrs[1] == XML::Smart::COPY
                tnos = nos.map{|e|e.dup}
                nos = Nokogiri::XML::NodeSet.new(nos.first.document,tnos)
              end
              return [nos, !same]
            else
              return [nos, false]
            end
          elsif attrs.length == 2 && attrs[0].is_a?(String) && attrs[1].is_a?(String) && attrs[0][0] == '?'
            tmp = Nokogiri::XML::ProcessingInstruction.new @element.document, attrs[0].sub(/./,''), attrs[1]
            return [tmp,false]
          end
          return [nil, false]
        end
        private :add_helper
        def add(*attrs)
          tmp, update = add_helper(attrs)
          res = Dom::smart_helper(@element.add_child tmp)
          if update
            @element.document.custom_namespace_prefixes_update
            @element.document.ns_update
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
          res = Dom::smart_helper(@element.add_previous_sibling tmp)
          if update
            @element.document.custom_namespace_prefixes_update
            @element.document.ns_update
          end
          res
        end
        def add_after(*attrs)
          tmp, update = add_helper(attrs)
          res = Dom::smart_helper(@element.add_next_sibling tmp)
          if update
            @element.document.custom_namespace_prefixes_update
            @element.document.ns_update
          end
          res
        end

        def dump
          doc = Nokogiri::XML::Document.new
          doc.root = @element
          doc.root.to_s
        end

        def to_s; @element.content; end
        def to_i; @element.content.to_i; end
        def to_f; @element.content.to_f; end

        def namespace?; !@element.namespace.nil?; end
        def namespace; namespace? ? Namespace.new(@element.namespace) : nil; end
        def namespace=(n)
          n = case n
            when Namespace
              n.prefix
            when String
              n
            else
              return
          end
          tmp = @element.document.custom_namespace_prefixes[n] || @element.document.user_custom_namespace_prefixes[n]
          unless tmp.nil?
            @element.namespace_scopes.each do |nss|
              @element.namespace = nss if nss.href == tmp
            end
          end
        end
        def namespaces; NamespaceSet.new(self,@element); end

        def xinclude!(basedir=nil)
          @element.do_xinclude_manual(basedir)
          @element.document.custom_namespace_prefixes_update
          @element.document.ns_update
          true
        end

        def replace_by(n)
          case n
            when Element; Element.new @element.replace(n.instance_variable_get(:@element))
            when NodeSet; NodeSet.new @element.replace(n.instance_variable_get(:@nodeset))
            else
              nil
          end
        end

        def qname; QName.new @element; end
        def attributes; AttributeSet.new @element; end

        def text; @element.xpath_fast("string(text())"); end
        def text=(t); @element.content = t.to_s if t.respond_to? :to_s; end

        def children; find('*|text()'); end
        def children?; find('*|text()').length > 0 end
        def parent
          Dom::smart_helper(@element.parent)
        end
        def parent?; !@element.parent.nil?; end

        def empty?; !children?; end
        def mixed?;
          @element.xpath_fast('*').length > 0 &&  @element.xpath_fast("string(text())") != '';
        end
        def text_only?;
          @element.xpath_fast('*').length == 0 &&  @element.xpath_fast("string(text())") != '';
        end
        def element_only?;
          @element.xpath_fast('*').length > 0 &&  @element.xpath_fast("string(text())") == '';
        end

        def path; @element.path[-1] != ']' ? @element.path + '[1]' : @element.path; end

        def ==(other)
          return false unless other
          return false unless other.respond_to?(:unique_id)
          unique_id == other.unique_id
        end
        def unique_id; @element.pointer_id; end

        def to_doc
          doc = Nokogiri::XML::Document.new
          doc.root = @element
          Dom.new(doc)
        end
      end

    end
  end
end
