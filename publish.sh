#!/bin/bash
rm -rf build tmp wisardpkg.egg-info wisardpkg*.so dist
python3 setup.py test && \
python3 setup.py sdist && \
python3 -m twine upload -r pypitest dist/* && \
python3 -m twine upload -r pypi dist/* && \
echo "Published!!!"
rm -rf build tmp wisardpkg.egg-info wisardpkg*.so dist
