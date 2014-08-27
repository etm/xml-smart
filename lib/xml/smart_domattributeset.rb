module XML
  module Smart
    class Dom

      class AttributeSet
        include Enumerable

        def initialize(element)
          @element = element
          @set = @element.attributes
        end

        def ===(cls); self.is_a? cls; end

        def names
          @set.keys 
        end

        def has_attr?(a); @set.has_key?(a) end
        alias :include? :has_attr?
        alias :attr? :has_attr?
        alias :member? :has_attr?

        def [](name,attr=false)
          return nil unless has_attr?(name)
          if attr == false 
            @element.attribute(name).value
          else  
            Attribute.new(@element.attribute(name))
          end 
        end
        def []=(name,value);
          @element[name] = value
        end

        def length;      @set.length; end
        def empty?;      @set.empty?; end
        def delete_all!; @set.each { |k,a| a.remove }; end

        def each(&block)
          @set.each do |k,node|
            block.call Dom::smart_helper(node)
          end
        end

        def delete_at!(name)
          tmp = @set[name]
          if tmp.is_a?(Nokogiri::XML::Attr)
            tmp.remove
            true
          else
            false
          end
        end

        def delete_if!(&block)
          @set.each do |k,node|
            node.remove if block.call(Dom::smart_helper(node))
          end
        end
      end  
    
    end
  end  
end  

