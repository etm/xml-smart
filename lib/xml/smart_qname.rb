module XML
  module Smart

    class QName
      def initialize(element); @element = element; end

      def ==(str); @element.name == str; end
      def ===(cls); self.is_a? cls; end
      def +(str); @element.name + str; end

      def to_s; prefix + @element.name; end
      def name; @element.name; end
      def name=(n); @element.name = n; end

      def prefix; @element.namespace && @element.namespace.prefix ? @element.namespace.prefix : "xmlns"; end
      def href; @element.namespace.href; end
    end
    
  end
end  
