open Lib;;

module Int = struct
   type t = int
   (* use Pervasives compare *)
   let compare = compare
end

module Ints = Set.Make(Int)

let input_file = "./input/day_1_input.txt"

let part_one () =
  let ints       = Lib.make_stream input_file in
  let result     = Lib.stream_fold (fun x sum -> sum + (int_of_string x)) ints 0 in
  Printf.printf "[1.1] = %d\n" result

let part_two () =
  let rec find array index length acc set =
    let value = ((+) acc (Array.get array index)) in
    if (Ints.mem value set) then value
    else find array ((mod) ((+) index 1) length) length value (Ints.add value set)
  in
  let strings   = Array.of_list (Lib.list_of_stream (Lib.make_stream input_file)) in
  let ints = Array.map int_of_string strings in
  let result = find ints 0 (Array.length ints) 0 Ints.empty in
  Printf.printf "[1.2] = %d\n" result

let () =
  print_endline "* Day 1 *";
  part_one ();
  part_two ()
