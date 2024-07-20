{
  description = "CPLEX C++ projects for OeGOR Summer School 2024, Krems, Austria";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        # Override the Nix package set to allow unfree packages
        pkgs = import nixpkgs {
          system = system; 
          config.allowUnfree = true; 
        };

        # # Setup CPLEX installer as a fixed-output derivation
        # cplexInstaller = pkgs.stdenv.mkDerivation {
        #   name = "cplex-installer";
        #   src = /home/onyr/cplex2210/cplex_studio2210.linux_x86_64.bin;
        #   outputHashMode = "flat";
        #   outputHashAlgo = "sha256";
        #   outputHash = "07cak2594vgipk728v4xs3gwdd1wwbzphv4qnpfkyhsmxfifdy68"; # Calculate the hash of the installer: nix-hash --flat --base32 --type sha256 /home/onyr/cplex2210/cplex_studio2210.linux_x86_64.bin
        #   phases = ["installPhase"];
        #   installPhase = "cp $src $out";
        # };

        # # Import the custom CPLEX package derivation
        # cplex = (import ./cplex.nix) {
        #   inherit (pkgs) lib stdenv makeWrapper openjdk gtk2 xorg glibcLocales;
        #   releasePath = cplexInstaller; # Pass the installer derivation
        # };
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
            #cplex # Include the custom CPLEX package
          ];
        };
      }
    );
}
