module TestMixin
  def setup
  end

  def teardown
  end

  def wf_assert(what,cond=true)
    if cond
      assert($long_track.include?(what),"Missing \"#{what}\":\n#{$long_track}")
    else  
      assert(!$long_track.include?(what),"Missing \"#{what}\":\n#{$long_track}")
    end
  end
  def wf_sassert(what,cond=true)
    if cond
      assert($short_track.include?(what),"#{$short_track}\nNot Present \"#{what}\":\n#{$long_track}")
    else  
      assert(!$short_track.include?(what),"#{$short_track}\nNot Present \"#{what}\":\n#{$long_track}")
    end
  end
  def wf_rsassert(pat='')
    assert($short_track =~ /#{pat}/,"Somehow executed different #{$short_track} should be '#{pat}'")
  end
end
