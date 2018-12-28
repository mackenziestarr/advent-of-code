open Lib;;

let input_file = "./input/day_5_input.txt"
let input = (Str.split (Str.regexp "") (String.trim (input_line (open_in input_file))))

let reacts a b =
  not (String.equal a b) && (String.uppercase_ascii a = String.uppercase_ascii b)

let reduce_polymer polymer x =
  match polymer with
  | y :: ys when reacts x y -> ys
  | ys -> x :: ys

let part_one() =
  let reduced_polymer = List.fold_left reduce_polymer [] input in
  Printf.printf "[5.1] = %d\n" (String.length (String.concat "" reduced_polymer))

let () =
  print_endline "* Day 5 *";
  part_one()
