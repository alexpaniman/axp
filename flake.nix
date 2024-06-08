{
  description = "axp :: c++ support library";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
  {

    packages.${system}.default = pkgs.clangStdenv.mkDerivation {
      name = "axp";
      src = ./.;

      nativeBuildInputs = with pkgs; [ clang-tools ];
      buildInputs = with pkgs; [ fmt ];
    };

  };
}
