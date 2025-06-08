ARG ARCH=aarch64
ARG REPO=axisecp
ARG SDK=acap-native-sdk
ARG UBUNTU_VERSION=24.04
ARG VERSION=12.4.0

FROM ${REPO}/${SDK}:${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION}

# Set general arguments
ARG ARCH
ARG SDK_LIB_PATH_BASE=/opt/axis/acapsdk/sysroots/${ARCH}/usr
ARG APP_RPATH=/usr/local/packages/mcpserver
ARG BUILD_DIR=/opt/build

#-------------------------------------------------------------------------------
# Build Monkey library
#-------------------------------------------------------------------------------

ARG MONKEY_VERSION=v1.5.6
ARG MONKEY_GIT_REPO=https://github.com/monkey/monkey
ARG MONKEY_DIR=${BUILD_DIR}/monkey
ARG MONKEY_SRC_DIR=${MONKEY_DIR}/src
ARG MONKEY_BUILD_DIR=${MONKEY_DIR}/build

WORKDIR ${MONKEY_DIR}
SHELL ["/bin/bash", "-o", "pipefail", "-c"]
RUN git clone -b $MONKEY_VERSION $MONKEY_GIT_REPO ${MONKEY_SRC_DIR##*/}

# Remove unused directories
WORKDIR ${MONKEY_SRC_DIR}
RUN rm -rf \
      deps/jemalloc \
      debian* \
      src/contrib \
      examples

# Configure, build and install Monkey files
COPY monkey.patch .
RUN git apply --ignore-space-change --ignore-whitespace monkey.patch && \
    . /opt/axis/acapsdk/environment-setup* && \
    ./configure \
    --enable-shared \
    --enable-plugins=fastcgi,liana \
    --malloc-libc \
    --prefix=${SDK_LIB_PATH_BASE} \
    --libdir=${SDK_LIB_PATH_BASE}/lib \
    --plugdir=${APP_RPATH}/lib/plugins \
    --sysconfdir=${APP_RPATH}/html \
    --logdir=${APP_RPATH}/logs \
    --pidfile=/tmp/monkey.pid && \
    make && \
    make install

WORKDIR ${MONKEY_BUILD_DIR}/lib
RUN cp -P ${SDK_LIB_PATH_BASE}/lib/libmonkey* .

WORKDIR ${MONKEY_BUILD_DIR}/lib/plugins
ARG MONKEY_PLUGINS="monkey-liana monkey-fastcgi"
RUN for plug in $MONKEY_PLUGINS; do \
      cp "$APP_RPATH/lib/plugins/$plug.so" . ; \
    done
