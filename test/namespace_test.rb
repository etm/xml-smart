require 'test/unit'
require 'smart'

class NamespaceTest < Test::Unit::TestCase
  BASE = "http://rbxs"
  DEFAULT = "#{BASE}/default"
  NS1 = "#{BASE}/ns1"
  NS2 = "#{BASE}/ns2"
  ADDED = "#{BASE}/added"

  TEMPLATE = <<-EOT
    <?xml version="1.0" encoding="utf-8"?>
    <root xmlns:ns1="#{NS1}" xmlns="#{DEFAULT}">
      <n1 xmlns="#{NS2}">
        <n1s1 />
      </n1>
      <n2>
        <ns1:n2s1 />
        <n2s2 />
      </n2>
      <n3 xmlns="">
      </n3>
    </root>
  EOT

  TEMPLATE.sub!(/\A\s*/,'')
  
  def setup
    @doc = XML::Smart.string(TEMPLATE)

    @root = @doc.root
    @n1, @n2, @n3 = @root.children.to_a

    @n1s1 = @n1.children.to_a
    @n2s1, @n2s2 = @n2.children.to_a
  end
  
  def test_namespaces
    nss = @root.namespaces
    assert_equal 2,nss.length
    assert_equal NS1,nss["ns1"]
  end

  def test_namespace
    assert_equal DEFAULT,@root.namespace.href
    assert_equal NS2,@n1.namespace.href
    assert_equal DEFAULT,@n2.namespace.href
  end

  def test_find_namespace
    assert_nil @root.namespaces.find("x")
    assert_not_nil NS1,@root.namespaces.find("ns1").href
    assert_raises TypeError do
      @n1.namespaces.find(nil).href
    end	
  end

  def test_find_namespace_by_href
    assert_nil @root.namespaces.find("http://blah")
    assert_not_nil @root.namespaces.find(DEFAULT)
    assert_not_nil @root.namespaces.find(NS1)
    assert_nil @root.namespaces.find(NS2)
    assert_not_nil @n1.namespaces.find(NS2)
  end

  def test_add_namespace
    added_ns = @n2.namespaces["added"] = ADDED
    n2s3 = @n2.add("s3")
    n2s3.namespace = added_ns
    assert_equal "<added:s3/>",n2s3.dump
  end

  def test_find
    assert_not_nil @root.find("//n3").first

    assert_nil @root.find("//n1").first
    assert_not_nil @root.find("//ns:n1","ns"=>NS2).first

    assert_not_nil @root.find("//ns:n2s1","ns"=>NS1).first
  end

end

