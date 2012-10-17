module XML
  module Smart

    class QName
      def initialize(element); @element = element; end

      def ==(str); @element.name == str; end
      def ===(cls); self.is_a? cls; end
      def +(str); @element.name + str; end

      def to_s; (prefix.nil? || prefix == '' ? '' : prefix + ':') + @element.name; end
      def to_sym; to_s.to_sym; end
      def name; @element.name; end
      def name=(n); @element.name = n; end

      def prefix
        if @element.namespace 
          if @element.namespace.prefix.nil?
            ''
          else  
            @element.namespace.prefix
          end  
        else
          nil
        end
      end

      def href
        @element.namespace ? @element.namespace.href : nil
      end  
    end
    
  end
end  
