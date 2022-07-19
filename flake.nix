{
    inputs = {
        nixpkgs.url = "nixpkgs";
    };

    outputs = { nixpkgs, ... }@inputs:
    let
        pkgs = import nixpkgs { localSystem = { system = "x86_64-linux"; }; };

        inherit (pkgs.stdenv)
            mkDerivation;

        drv = mkDerivation {
            pname = "ckana";
            version = "0.1.0";
            src = ./src;
            nativeBuildInputs = with pkgs; [ ncurses ];
            makeFlags = [ "PREFIX=$(out)" ];
        };
    in
    {
        packages.x86_64-linux.default = drv;
    };
}
