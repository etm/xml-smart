module XML
  module Smart
    class Dom

      class Attribute
        alias :text :value
        alias :text= :value=

        def initialize(attr)
          @node = attr
        end

        def ===(cls); self.is_a? cls; end

        def to_s; @node.content; end
        def to_i; @node.content.to_i; end
        def to_f; @node.content.to_f; end

        def qname; QName.new @node; end
        def path; @node.path; end

        def value;  @node.content; end;
        def value=(val); @node.content = val; end;

        def ==(other)
          return false unless other
          return false unless other.respond_to?(:unique_id)
          unique_id == other.unique_id
        end
        def unique_id; @node.pointer_id; end

        def parent; Dom::smart_helper(@node.parent); end
      end

    end
  end
end
