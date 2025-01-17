$HASH = git log --pretty=%h -1
git show-ref | select-string $HASH | select-string -NotMatch HEAD | foreach {$_ -replace '.*/(.*)','$1'}
