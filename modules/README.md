cd opencv
git checkout 3.4
mkdir build && cd build
cmake \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=/your/path/to/opencv-compile \
-DBUILD_LIST=core,imgcodecs,python_bindings_generator,python3,av_stream,dnn_innference \
-DOPENCV_EXTRA_MODULES_PATH=/your/path/to/pybindcpp/modules/  \
-DBUILD_opencv_python3=ON \
-DSOFTFP=ON \
..