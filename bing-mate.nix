{pkgs ? import <nixpkgs> { }, ...}:
pkgs.stdenv.mkDerivation rec {
  pname = "bing-mate";
  version = "1.0.0";

  src = ./.;

  nativeBuildInputs = with pkgs; [
    curl.dev
    gcc
    gnumake
    gtk3.dev
    json_c.dev
    libconfig
    pkg-config
  ];

  buildInputs = with pkgs; [
    xdg-user-dirs
  ];

  buildPhase = ''
    make
  '';

  installPhase = ''
    mkdir -p $out/bin
    mv bin/bing-mate $out/bin
  '';

  meta = with pkgs.lib; {
    description = ''Small program to get the current bing "today in bing" image and set it as wallpaper on mate-desktop.'';
    homepage = "https://github.com/rafameou/bing-mate";
    licence = licences.bsd3;
  };
}

