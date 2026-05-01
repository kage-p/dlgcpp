
cd ..

git submodule set-branch -b v1.17.x 3rdparty/googletest
git submodule sync
git submodule update --init --recursive --remote

@pause