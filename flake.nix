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

        # Import the custom CPLEX package derivation
        cplex = (import ./cplex.nix) {
          inherit (pkgs) lib stdenv makeWrapper openjdk gtk2 xorg glibcLocales;
          releasePath = "/home/onyr/cplex2210"; # Set the path to your downloaded CPLEX installer
        };
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
