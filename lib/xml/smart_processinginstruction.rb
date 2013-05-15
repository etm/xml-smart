module XML
  module Smart

    class ProcessingInstruction
      def initialize(element)
        @element = element
      end

      def ===(cls); self.is_a? cls; end

      def dump; @element.to_s; end

      def replace_by(n)
        case n
          when ProcessingInstruction; ProcessingInstruction.new @element.replace(n.instance_variable_get(:@element))
          else
            nil
        end
      end

      def qname; QName.new @element; end

      def content; @element.content end
      def content=(t); @element.content = t.to_s if t.respond_to? :to_s; end

      def parent
        Dom::smart_helper(@element.parent)
      end
      def parent?; !@element.parent.nil?; end

      def path; @element.path; end

      def ==(other)
        return false unless other
        return false unless other.respond_to?(:unique_id)
        unique_id == other.unique_id
      end
      def unique_id; @element.pointer_id; end
    end
  end
end
