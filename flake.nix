{
  description = "CPLEX C++ projects for OeGOR Summer School 2024, Krems, Austria";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        # CPLEX Custom Package Derivation
        cplex = pkgs.callPackage ({ lib, stdenv, makeWrapper, openjdk, gtk2, xorg, glibcLocales }:
          {
            pname = "cplex";
            version = "128";

            src = "/home/onyr/cplex2210"; # IMPORTANT: add your CPLEX installer bin path here

            nativeBuildInputs = [ makeWrapper ];
            buildInputs = [ openjdk gtk2 xorg.libXtst glibcLocales ];

            unpackPhase = "cp $src $name";

            patchPhase = ''
              sed -i -e 's|/usr/bin/tr"|tr"         |' $name
            '';

            buildPhase = ''
              sh $name -i silent -DLICENSE_ACCEPTED=TRUE -DUSER_INSTALL_DIR=$out
            '';

            installPhase = ''
              mkdir -p $out/bin
              ln -s $out/opl/bin/x86-64_linux/oplrun\
                $out/opl/bin/x86-64_linux/oplrunjava\
                $out/opl/oplide/oplide\
                $out/cplex/bin/x86-64_linux/cplex\
                $out/cpoptimizer/bin/x86-64_linux/cpoptimizer\
                $out/bin
            '';

            fixupPhase = let
              libraryPath = lib.makeLibraryPath [ stdenv.cc.cc gtk2 xorg.libXtst ];
            in ''
              interpreter=${stdenv.cc.libc}/lib/ld-linux-x86-64.so.2

              for pgm in $out/opl/bin/x86-64_linux/oplrun $out/opl/bin/x86-64_linux/oplrunjava $out/opl/oplide/oplide;
              do
                patchelf --set-interpreter "$interpreter" $pgm;
                wrapProgram $pgm \
                  --prefix LD_LIBRARY_PATH : $out/opl/bin/x86-64_linux:${libraryPath} \
                  --set LOCALE_ARCHIVE ${glibcLocales}/lib/locale/locale-archive;
              done

              for pgm in $out/cplex/bin/x86-64_linux/cplex $out/cpoptimizer/bin/x86-64_linux/cpoptimizer $out/opl/oplide/jre/bin/*;
              do
                if grep ELF $pgm > /dev/null;
                then
                  patchelf --set-interpreter "$interpreter" $pgm;
                fi
              done
            '';

            passthru = {
              libArch = "x86-64_linux";
              libSuffix = "2210";
            };

            meta = with lib; {
              description = "Optimization solver for mathematical programming";
              homepage = "https://www.ibm.com/be-en/marketplace/ibm-ilog-cplex";
              sourceProvenance = with sourceTypes; [ binaryNativeCode ];
              license = licenses.unfree;
              platforms = [ "x86_64-linux" ];
              maintainers = with maintainers; [ bfortz ];
            };
          }
        ) {};

      in
      {
        # development environment
        devShells.default = pkgs.mkShell {
          packages = [
            # C/C++ development
            pkgs.gcc
            pkgs.gnumake
            pkgs.gdb
            pkgs.valgrind
            pkgs.cmake
            cplex # Include the custom CPLEX package
          ];
        };
      }
    );
}
