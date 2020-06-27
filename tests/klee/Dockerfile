FROM klee/klee:2.1

RUN sudo apt-get update && sudo apt-get install -y --no-install-recommends git subversion autoconf automake libtool gettext

RUN pip install --upgrade wllvm && \
	export LLVM_COMPILER=clang

RUN export LLVM_COMPILER=clang && \
	svn co svn://vcs.pcre.org/pcre2/code/trunk pcre && \
	cd pcre && \
	autoreconf --install && \
	autoconf && \
	CC=wllvm CFLAGS="-g -O1 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES -DFORTIFY_SOURCE=0 -U__OPTIMIZE" ./configure && \
	make && \
	find . -executable -type f | xargs -I '{}' extract-bc '{}'

RUN git clone https://github.com/sartura/libyang && \
	cd libyang && \
	git checkout klee && \
	mkdir build && \
	cd build && \
	cmake -DCMAKE_C_COMPILER=wllvm -DCMAKE_C_FLAGS="-g -O1 -Xclang -disable-llmv-passes -D__NO_STRING_INLINES -DFORTIFY_SOURCE=0 -U__OPTIMIZE__" -DENABLE_FUZZ_TARGETS=ON .. && \
	make && \
	find . -executable -type f | xargs -I '{}' extract-bc '{}' 
	
