FROM ubuntu:16.04
RUN apt-get update && apt-get upgrade -y && apt-get dist-upgrade -y
RUN apt-get install locales

# Set the locale
RUN locale-gen en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# Install dependencies
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get install software-properties-common -y && add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt-get update && \
    apt-get install -qq wget unzip build-essential cmake gcc-7 g++-7 \
    libcunit1-dev libudev-dev libicu-dev zlib1g-dev libssl-dev git \
    libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev \
    python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev \
    libjasper-dev libdc1394-22-dev autoconf libtool libcurl4-openssl-dev uuid-dev \
    pkg-config unzip ffmpeg qtbase5-dev python-dev python3-dev python-numpy \
    python3-numpy libopencv-dev libgtk-3-dev libdc1394-22 libdc1394-22-dev \
    libjpeg-dev libpng12-dev libtiff5-dev libjasper-dev libavcodec-dev \
    libavformat-dev libswscale-dev libxine2-dev libgstreamer0.10-dev \
    libgstreamer-plugins-base0.10-dev libv4l-dev libtbb-dev libfaac-dev \
    libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev \
    libvorbis-dev libxvidcore-dev v4l-utils vtk6 liblapacke-dev libopenblas-dev \
    libgdal-dev checkinstall && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 --slave /usr/bin/g++ g++ /usr/bin/g++-7 && \
    update-alternatives --config gcc 
RUN mkdir -p /workdir
WORKDIR /workdir
RUN wget https://github.com/opencv/opencv/archive/3.4.0.zip && \
    unzip 3.4.0.zip && cd opencv-3.4.0 && mkdir build && cd build && \
    cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D FORCE_VTK=ON -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON \
    -D WITH_CUBLAS=ON -D CUDA_NVCC_FLAGS="-D_FORCE_INLINES" -D WITH_GDAL=ON \
    -D WITH_XINE=ON -D BUILD_EXAMPLES=ON .. && make -j $(($(nproc) + 1)) && \
    /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf' && ldconfig && \
    apt-get update
RUN cd opencv-3.4.0/build && checkinstall -y
RUN git clone https://github.com/mrtazz/restclient-cpp.git && \
    cd restclient-cpp && ./autogen.sh && ./configure && make install
RUN wget http://zlib.net/zlib-1.2.11.tar.gz && \
    tar -zxf zlib-1.2.11.tar.gz && \
    cd zlib-1.2.11 && ./configure && make && make install
RUN wget http://www.openssl.org/source/openssl-1.1.0g.tar.gz && \
    tar -zxf openssl-1.1.0g.tar.gz && \
    cd openssl-1.1.0g && ./config && make && make install
RUN git clone https://github.com/uNetworking/uWebSockets.git && \
    cd uWebSockets && make && make install && cp libuWS.so /usr/local/lib/
RUN wget ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.41.tar.gz && \
    tar -zxf pcre-8.41.tar.gz && cd pcre-8.41 && ./configure && make && make install
RUN git clone https://github.com/fdintino/nginx-upload-module.git
RUN wget http://nginx.org/download/nginx-1.13.9.tar.gz && \
    tar -zxf nginx-1.13.9.tar.gz && \
    cd nginx-1.13.9 && \
    ./configure --prefix=/usr/share/nginx \
            --sbin-path=/usr/sbin/nginx \
            --modules-path=/usr/lib/nginx/modules \
            --conf-path=/etc/nginx/nginx.conf \
            --error-log-path=/var/log/nginx/error.log \
            --http-log-path=/var/log/nginx/access.log \
            --pid-path=/run/nginx.pid \
            --lock-path=/var/lock/nginx.lock \
            --user=www-data \
            --group=www-data \
            --build=Ubuntu \
            --http-client-body-temp-path=/var/lib/nginx/body \
            --http-fastcgi-temp-path=/var/lib/nginx/fastcgi \
            --http-proxy-temp-path=/var/lib/nginx/proxy \
            --http-scgi-temp-path=/var/lib/nginx/scgi \
            --http-uwsgi-temp-path=/var/lib/nginx/uwsgi \
            --with-openssl=../openssl-1.1.0g \
            --with-openssl-opt=enable-ec_nistp_64_gcc_128 \
            --with-openssl-opt=no-nextprotoneg \
            --with-openssl-opt=no-weak-ssl-ciphers \
            --with-openssl-opt=no-ssl3 \
            --with-pcre=../pcre-8.41 \
            --with-pcre-jit \
            --with-zlib=../zlib-1.2.11 \
            --with-compat \
            --with-file-aio \
            --with-threads \
            --with-http_addition_module \
            --with-http_auth_request_module \
            --with-http_dav_module \
            --with-http_flv_module \
            --with-http_gunzip_module \
            --with-http_gzip_static_module \
            --with-http_mp4_module \
            --with-http_random_index_module \
            --with-http_realip_module \
            --with-http_slice_module \
            --with-http_ssl_module \
            --with-http_sub_module \
            --with-http_stub_status_module \
            --with-http_v2_module \
            --with-http_secure_link_module \
            --with-mail \
            --with-mail_ssl_module \
            --with-stream \
            --with-stream_realip_module \
            --with-stream_ssl_module \
            --with-stream_ssl_preread_module \
            --with-debug \
            --with-cc-opt='-g -O2 -fPIC -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time -D_FORTIFY_SOURCE=2' \
            --with-ld-opt='-Wl,-Bsymbolic-functions -fPIC -Wl,-z,relro -Wl,-z,now' \
            --add-module=../nginx-upload-module && \
    make -j && \
    make install
# Grab the checked out source
RUN apt-get install unrar
RUN mkdir /workdir2
WORKDIR /workdir2
COPY . /workdir2
RUN ./compile && cp server manga_server_config.json /usr/local/bin && cp server_startup.sh /etc/nginx && ./server_setup.sh && cd mangahandler && \
    ./compile && cp mangahandler /usr/local/bin && cp ../nginx.conf /etc/nginx/ && \
    cp -r ../sites-enabled ../sites-available /etc/nginx/ && mkdir -p /var/lib/nginx/ && mkdir -p /var/www/ && cp ../privkey.pem ../cert.pem /roboscans/certs/ && nginx -t
WORKDIR /etc/nginx
RUN rm -rf /workdir && rm -rf /workdir2
VOLUME ["/roboscans/tmp/", "/roboscans/manga/", "/roboscans/www/", "/roboscans/logging/", "/roboscans/certs/"]
EXPOSE 80 443
ENV LD_LIBRARY_PATH="/usr/local/lib:${LD_LIBRARY_PATH}"
ENTRYPOINT ["./server_startup.sh"]
