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
          if attrs.length>0 && attrs[0].is_a?(String)
            pfx = ''
            if attrs[0].sub!(/([^:]+):/, '')
              pfx = $1
              if @element.document.user_custom_namespace_prefixes.has_key?(pfx)
                @element.document.custom_namespace_prefixes.each do |k,v|
                  if @element.document.user_custom_namespace_prefixes[pfx] == v
                    pfx = k
                  end
                end  
              end
              raise Error, 'No valid namespace' if !@element.document.custom_namespace_prefixes.has_key?(pfx)
            end  
            tmp = Nokogiri::XML::Node.new attrs[0], @element.document
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
            return tmp
          end
          if attrs.length == 1 && attrs[0].is_a?(XML::Smart::Dom::Element)
            ele = attrs[0].instance_variable_get(:@element)
            return ele.document.root.pointer_id == @element.document.root.pointer_id ? ele : ele.dup
          end
          if attrs.length == 1 && attrs[0].is_a?(XML::Smart::Dom::NodeSet)
            nos = attrs[0].instance_variable_get(:@nodeset)
            if nos.length > 0
              if nos.first.document.root.pointer_id == @element.document.root.pointer_id
                return nos
              else
                xnos = nos.to_a
                tnos = nos.map{|e|e.dup}
                xnos.each{|e|nos.delete(e)}
                tnos.each{|e|nos.push(e)}
                return nos
              end
            else
              return nos
            end
          end
          if attrs.length == 2 && attrs[0].is_a?(XML::Smart::Dom::Element) && (attrs[1] == XML::Smart::COPY || attrs[1] == XML::Smart::MOVE)
            ele = attrs[0].instance_variable_get(:@element)
            return attrs[1] == XML::Smart::COPY ? ele.dup : ele
          end
          if attrs.length == 2 && attrs[0].is_a?(XML::Smart::Dom::NodeSet) && (attrs[1] == XML::Smart::COPY || attrs[1] == XML::Smart::MOVE)
            nos = attrs[0].instance_variable_get(:@nodeset)
            if attrs[1] == XML::Smart::COPY
              xnos = nos.to_a
              tnos = nos.map{|e|e.dup}
              xnos.each{|e|nos.delete(e)}
              tnos.each{|e|nos.push(e)}
              return nos
            else
              return nos
            end
          end
          nil
        end
        def add(*attrs);        Element.new(@element.add_child add_helper(attrs));            end
        def add_before(*attrs); Element.new(@element.add_previous_sibling add_helper(attrs)); end
        def add_after(*attrs);  Element.new(@element.add_next_sibling add_helper(attrs));     end
        private :add_helper

        def dump; @element.to_s; end
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

        def xinclude!; @element.do_xinclude; true; end

        def replace_by(n)
          case n
            when Element; Element.new @element.replace(n.instance_variable_get(:@element))
            when NodeSet; NodeSet.new @element.replace(n.instance_variable_get(:@nodeset))
            else
              nil
          end
        end

        def qname; QName.new @element; end
        def attributes; AttributeSet.new @element.attributes; end

        def text; @element.xpath_fast("string(text())"); end
        def text=(t); @element.content = t.to_s if t.respond_to? :to_s; end

        def children; NodeSet.new(@element.children); end
        def children?; NodeSet.new(@element.children).length > 0 end
        def parent; Element.new(@element.parent); end
        def parent?; !@element.parent.nil?; end

        def empty?; @element.blank?; end
        def mixed?;
          @element.xpath_fast('*').length > 0 &&  @element.xpath_fast("string(text())") != '';
        end
        def text_only?;
          @element.xpath_fast('*').length == 0 &&  @element.xpath_fast("string(text())") != '';
        end
        def element_only?;
          @element.xpath_fast('*').length > 0 &&  @element.xpath_fast("string(text())") == '';
        end

        def path; @element.path; end

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
