FROM klee/klee:2.1

RUN sudo apt-get update && sudo apt-get install -y --no-install-recommends git subversion autoconf automake libtool gettext libpcre2-dev

RUN pip install --upgrade wllvm && \
	export LLVM_COMPILER=clang

RUN svn co svn://vcs.pcre.org/pcre2/code/trunk pcre

RUN export LLVM_COMPILER=clang && \
	cd pcre && \
	autoreconf --install && \
	autoconf && \
	CC=wllvm CFLAGS="-g -O1 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES -DFORTIFY_SOURCE=0 -U__OPTIMIZE" ./configure && \
	make && \
	find . -executable -type f | xargs -I '{}' extract-bc '{}'

RUN git clone https://github.com/sartura/libyang

RUN export LLVM_COMPILER=clang && \
	cd libyang && \
	git checkout klee && \
	mkdir build && \
	cd build && \
	cmake -DCMAKE_C_COMPILER=wllvm -DCMAKE_C_FLAGS="-g -O1 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES -DFORTIFY_SOURCE=0 -U__OPTIMIZE__" -DENABLE_FUZZ_TARGETS=ON .. && \
	make && \
	find . -executable -type f | xargs -I '{}' extract-bc '{}' 
	
WORKDIR /home/klee/libyang/build
CMD klee --optimize --libc=uclibc --posix-runtime --link-llvm-lib=/home/klee/libyang/build/libyang.so.2.0.0.bc --link-llvm-lib=/home/klee/pcre/.libs/libpcre2-8.so.0.10.0.bc /home/klee/libyang/build/tests/fuzz/yang_parse_module.bc -sym-stdin 256
