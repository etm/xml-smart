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

        def prefix
          if @ns.prefix
            @ns.prefix
          else
            @ns.document.custom_namespace_prefixes.find{|k,v| v == @ns.href}[0] rescue ''
          end
        end

        def ==(other)
          return false unless other
          return false unless other.respond_to?(:to_s)
          to_s == other.to_s
        end
      end

    end
  end
end
