module XML
  module Smart
    class Dom

      class Other < Text
        def otype
          case @node
            when Nokogiri::XML::Comment; :comment
            else; :unknown
          end
        end
      end

    end
  end
end
