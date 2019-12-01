# Advent of Code

## 2019

### language: C++

## 2018

### language: OCaml

#### setup for osx

```shell session
brew install ocaml
brew install opam
opam init
opam install ocamlbuild
# add packages installed via opam to PATH
echo export PATH="\$PATH:$HOME/.opam/default/bin" >> ~/.bashrc
```

#### compiling an exercise

```shell session
# assuming cwd contains some_file.ml
ocamlbuild some_file.native
./some_file.native
```
