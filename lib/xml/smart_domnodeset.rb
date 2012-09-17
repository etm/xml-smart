module XML
  module Smart
    class Dom

      class NodeSet
        include Enumerable

        def initialize(nodeset)
          @nodeset = nodeset
        end

        def ===(cls); self.is_a? cls; end

        def first;        Dom::smart_helper(@nodeset.first); end
        def last;         Dom::smart_helper(@nodeset.last); end
        def at(*a);  Dom::smart_helper(@nodeset[*a]); end
        def [](*a);  Dom::smart_helper(@nodeset[*a]); end

        def length;       @nodeset.length; end
        def empty?;       @nodeset.empty?; end
        def delete_all!;  @nodeset.remove; @nodeset.to_a.each { |n| @nodeset.delete(n) }; true  end

        def each(&block) 
          @nodeset.each do |node| 
            block.call Dom::smart_helper(node)
          end
        end

        def delete_at(*a)
          tmp = @nodeset[*a]
          case tmp
            when Nokogiri::XML::NodeSet
              tmp.each { |node| @nodeset.delete(node.remove) }
              true
            when Nokogiri::XML::Node
              @nodeset.delete(tmp.remove)
              true
            else
              false
          end
        end

        def delete_if(&block) 
          tmp = []
          @nodeset.each do |node| 
            if block.call(Dom::smart_helper(node))
              tmp << node.remove
            end  
          end    
          tmp.each { |t| @nodeset.delete(t) }
          self
        end

      end

    end
  end
end  

