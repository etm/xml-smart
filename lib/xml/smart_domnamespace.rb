module XML
  module Smart
    class Dom

      class Namespace
        def initialize(ns)
          @ns = ns
        end

        def ===(cls); self.is_a? cls; end

        def href; @ns.href; end
        alias :to_s :href

        def prefix; @ns.prefix || "xmlns"; end

        def ==(other)
          return false unless other
          return false unless other.respond_to?(:to_s)
          to_s == other.to_s
        end
      end
   
    end
  end
end
