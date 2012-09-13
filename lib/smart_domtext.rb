module XML
  module Smart
    class Dom

      class Text
        def initialize(text); @text = text; end

        def ===(cls); self.is_a? cls; end

        def dump; to_s; end
        def to_s; @text.content.to_s; end
        def to_f; @text.content.to_f; end
        def to_i; @text.content.to_i; end

        def text; @text.content; end
        def text=(t); @text.content = t.to_s if t.respond_to? :to_s; end

        def parent; Node.new(@text.parent); end

        def path; @text.path; end
      end
    
    end
  end
end  
