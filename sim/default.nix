with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "sim";
  src = ./.;

  nativeBuildInputs = [ scons ];

  buildPhase = ''
    scons platform=linux
  '';

  installPhase = ''
    mkdir -p $out/lib
    cp libpolitsim.so $out/lib/
  '';
}
