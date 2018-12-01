module Int = struct
   type t = int
   (* use Pervasives compare *)
   let compare = compare
end

module Ints = Set.Make(Int)

let input_file = "./input/day_1_input.txt"

let make_stream channel =
  Stream.from
    (fun _ -> try Some (int_of_string (input_line channel)) with End_of_file -> None)

let part_one () =
  let stream_fold f stream init =
    let result = ref init in
    Stream.iter (fun x -> result := f x !result) stream;
    !result in
  let ints       = make_stream (open_in input_file) in
  let result     = stream_fold (+) ints 0 in
  Printf.printf "[1.1] = %d\n" result

let part_two () =
  let rec find array index length acc set =
    let value = ((+) acc (Array.get array index)) in
    if (Ints.mem value set) then value
    else find array ((mod) ((+) index 1) length) length value (Ints.add value set)
  in
  let list_of_stream stream =
    let result = ref [] in
    Stream.iter (fun value -> result := value :: !result) stream;
    List.rev !result
  in
  let ints   = Array.of_list (list_of_stream (make_stream (open_in input_file))) in
  let result = find ints 0 (Array.length ints) 0 Ints.empty in
  Printf.printf "[1.2] = %d\n" result

let () =
  print_endline "* Day 1 *";
  part_one ();
  part_two ()
