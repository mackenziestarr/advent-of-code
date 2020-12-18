# Advent of Code

## 2020

### language: C++

```shell session
cd 2020
# run all tests
bazelisk test //...
# run a single day
bazelisk run //src:day-01
```

## 2019

### language: C++

```shell session
cd 2019
wget -O third_party/catch.hpp https://github.com/catchorg/Catch2/releases/download/v2.11.0/catch.hpp
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
