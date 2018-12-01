let input_file = "./input/day_1_input.txt"

let read channel parse_fn =
  Stream.from
    (fun _ -> try Some (parse_fn channel) with End_of_file -> None)

let stream_fold f stream init =
  let result = ref init in
  Stream.iter
    (fun x -> result := f x !result)
    stream;
    !result;;

let main () =
  let in_channel = open_in input_file in
  let ints       = read in_channel (fun chan -> (int_of_string (input_line chan)))  in
  let result     = stream_fold (+) ints 0 in
  Printf.printf "result: %d\n" result

let () = main ()
