module XML
  module Smart

    class ProcessingInstruction
      def initialize(element)
        @node = element
      end

      def ===(cls); self.is_a? cls; end

      def dump; @node.to_s; end

      def replace_by(n)
        case n
          when ProcessingInstruction; ProcessingInstruction.new @node.replace(n.instance_variable_get(:@node))
          else
            nil
        end
      end

      def qname; QName.new @node; end

      def content; @node.content end
      def content=(t); @node.content = t.to_s if t.respond_to? :to_s; end

      def parent
        Dom::smart_helper(@node.parent)
      end
      def parent?; !@node.parent.nil?; end

      def path; @node.path; end

      def ==(other)
        return false unless other
        return false unless other.respond_to?(:unique_id)
        unique_id == other.unique_id
      end
      def unique_id; @node.pointer_id; end
    end
  end
end
