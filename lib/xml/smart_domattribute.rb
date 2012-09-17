module XML
  module Smart
    class Dom

      class Attribute
        def initialize(attr)
          @attr = attr
        end

        def ===(cls); self.is_a? cls; end

        def to_s; @attr.content; end
        def to_i; @attr.content.to_i; end
        def to_f; @attr.content.to_f; end

        def qname; QName.new @attr; end
        def path; @attr.path; end

        def value;  @attr.content; end;
        def value=(val); @attr.content = (val); end;

        def ==(other)
          return false unless other
          return false unless other.respond_to?(:unique_id)
          unique_id == other.unique_id
        end
        def unique_id; @attr.pointer_id; end

        def parent; Dom::smart_helper(@attr.parent); end
      end

    end  
  end  
end  
