#!/bin/bash
rm -rf build tmp wisardpkg.egg-info wisardpkg*.so dist
make geninclude && \
make cpptest && \
IFS='"' read -ra VERSION <<< $(cat src/version.h) && \
git commit -a -m "Launging a new version" && \
git tag v${VERSION[1]} && \
python3 setup.py test && \
python3 setup.py sdist && \
python3 -m twine upload -r pypitest dist/* && \
python3 -m twine upload -r pypi dist/* && \
echo "Published!!!"
rm -rf build tmp wisardpkg.egg-info wisardpkg*.so dist
