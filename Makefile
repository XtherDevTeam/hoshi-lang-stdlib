HOSHIC_FLAGS = ./cmake-build-debug/libstd.a ./cmake-build-debug/libcrypto.a ./cmake-build-debug/libssl.a -I src

compiler-dependency:
	cd hoshi-lang && git submodule update --init --recursive && cd ..
	cd hoshi-lang && git checkout llvm && git pull -f && make cmake_production build_production && cd ..

cmake_debug: compiler-dependency
	cmake . -B cmake-build-debug  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=DEBUG -G "Unix Makefiles"

build_debug:
	cd cmake-build-debug; make all -j

clean:
	cd cmake-build-debug; make clean

cmake_production: compiler-dependency
	cmake . -B cmake-build-release  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=RELEASE -G "Unix Makefiles"

build_production:
	cd cmake-build-release; make all -j

clean_prod:
	cd cmake-build-release; make clean


# for build/{filename}, compile by `./cmake-build-debug/hoshi_lang examples/{filename}.hoshi -o build/{filename} --build-mode debug --preserve-intermediate`
build/%: examples/%.hoshi cmake_debug build_debug
	./hoshi-lang/cmake-build-release/hoshi_lang $< -o $@ $(HOSHIC_FLAGS) --build-mode debug --preserve-intermediate

build-release/%: examples/%.hoshi cmake_production build_production
	./hoshi-lang/cmake-build-release/hoshi_lang $< -o $@ $(HOSHIC_FLAGS) --build-mode release --preserve-intermediate

package:
	rm -rf build-package
	mkdir -p build-package/bin
	mkdir -p build-package/lib
	cp hoshi-lang/cmake-build-release/*hoshi* build-package/bin
	cp hoshi-lang/cmake-build-release/*elysia* build-package/bin
	cp hoshi-lang/mimalloc/build/*.a build-package/bin || true
	cp hoshi-lang/mimalloc/build/*.a build-package/bin || true
	# explicitly ignore non-existent files
	cp hoshi-lang/mimalloc/build/Release/*.lib build-package/bin || true
	cp hoshi-lang/mimalloc/build/Release/*.lib build-package/bin || true
	cp cmake-build-release/*crypto* build-package/bin
	cp cmake-build-release/*ssl* build-package/bin
	cp cmake-build-release/*std* build-package/bin
	cp -r hoshi-lang/lib/* build-package/lib/
	cp -r src/* build-package/lib/
	cp LICENSE build-package/

install-libs:
	cp -v cmake-build-release/*crypto* $(HOSHI_HOME)/bin
	cp -v cmake-build-release/*ssl* $(HOSHI_HOME)/bin
	cp -v cmake-build-release/*std* $(HOSHI_HOME)/bin
	cp -v -r src/* $(HOSHI_HOME)/lib