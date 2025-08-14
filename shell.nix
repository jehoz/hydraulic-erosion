{ pkgs ? import <nixpkgs> {} }:

  pkgs.mkShell {
    buildInputs = with pkgs; [
      ccls
    ];

    nativeBuildInputs = with pkgs; [
      raylib

      xorg.libXcursor
      xorg.libXrandr
      xorg.libXinerama
      xorg.libXi
      xorg.libX11.dev
    ];
}
