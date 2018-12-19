#!/bin/bash
make geninclude && \
make cpptest && \
IFS='"' read -ra VERSION <<< $(cat src/version.h) && \
git add . && \
git commit -m "Launging a new version" && \
git tag v${VERSION[1]} && \
git push && \
git push --tags && \
echo "Released!!!"
