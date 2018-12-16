open Lib;;

module Int = struct
   type t = int
   (* use Pervasives compare *)
   let compare = compare
end

module Ints = Set.Make(Int)

let input_file = "./input/day_1_input.txt"

(* @todo: add problem description *)
let part_one () =
  let stream = make_stream of_ints input_file in
  Printf.printf "[1.1] = %d\n" (Lib.stream_fold (+) stream 0)

(* @todo: add problem description *)
let part_two () =
  let array = make_stream of_ints input_file |> array_of_stream in
  let rec find array index length sum set =
    let sum = sum + array.(index) in
    if (Ints.mem sum set) then sum
    else
      let index = (mod) (index + 1) length in
      find array index length sum (Ints.add sum set)
  in
  Printf.printf "[1.2] = %d\n" (find array 0 (Array.length array) 0 Ints.empty)

let () =
  print_endline "* Day 1 *";
  part_one ();
  part_two ()
