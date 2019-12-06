# Advent of Code

## 2019

### language: C++

```shell session
cd 2019
make day_<day_num>
make day_<day_num>_test
```

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
