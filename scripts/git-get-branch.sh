#! /bin/bash

git show-ref | grep $(git log --pretty=%h -1) | sed 's|.*/\(.*\)|\1|' | sort -u | grep -v HEAD
