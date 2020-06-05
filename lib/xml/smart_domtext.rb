module XML
  module Smart
    class Dom

      class Text
        def initialize(text); @node = text; end

        def ===(cls); self.is_a? cls; end

        def dump; to_s; end
        def to_s; @node.content.to_s; end
        def to_f; @node.content.to_f; end
        def to_i; @node.content.to_i; end

        def text; @node.content; end
        def text=(t); @node.content = t.to_s if t.respond_to? :to_s; end

        def parent; Node.new(@node.parent); end

        def path; @node.path; end
      end

    end
  end
end
