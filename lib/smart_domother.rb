module XML
  module Smart
    class Dom

      class Other < Text
        def otype
          case @text
            when Nokogiri::XML::Comment; :comment
            else; :unknown
          end
        end
      end
    
    end
  end
end  
