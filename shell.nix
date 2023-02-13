{ pkgs ? import <nixpkgs> { }, ... }:
pkgs.mkShell {
  packages = with pkgs; [
    curl.dev
    gcc
    gnumake
    gtk3.dev
    json_c.dev
    libconfig
    pkg-config

    gdb
    valgrind
  ];
}

