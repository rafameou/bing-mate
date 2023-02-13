{
  description = ''Small program to get the current bing "today in bing" image and set it as wallpaper on mate-desktop.'';

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/22.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
    let pkgs = import nixpkgs { inherit system; };
    in {
      devShell       = import ./shell.nix     { inherit pkgs; };
      defaultPackage = import ./bing-mate.nix { inherit pkgs; };
    }
  );

}

