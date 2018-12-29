open Lib;;

let input_file = "./input/day_5_input.txt"
let input = (Str.split (Str.regexp "") (String.trim (input_line (open_in input_file))))

let reacts a b =
  not (String.equal a b) && (String.uppercase_ascii a = String.uppercase_ascii b)

let reduce_polymer polymer x =
  match polymer with
  | y :: ys when reacts x y -> ys
  | ys -> x :: ys

let reduced_polymer = List.fold_left reduce_polymer [] input

let part_one() =
  Printf.printf "[5.1] = %d\n" (List.length reduced_polymer)

let part_two() =
  let edited_polymers =
    List.map
      (fun offset ->
       let x = offset + 65 in
       let y = x + 32 in
       (List.filter
          (fun s ->
           let ch : int = Char.code((s.[0])) in
           (ch != x && ch != y)
          )
          input))
      (Lib.range 0 25)
  in
  let reduced_polymers =
    List.map
      (fun x -> List.length(List.fold_left reduce_polymer [] x))
      edited_polymers
  in
  Printf.printf "[5.2] = %d\n" (List.fold_left min max_int reduced_polymers)


let () =
  print_endline "* Day 5 *";
  part_one();
  part_two()
