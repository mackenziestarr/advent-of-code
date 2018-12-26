open Lib;;

let input_file = "./input/day_4_input.txt"
let matcher = Lib.regex_matches_to_list
                "\\[\\([0-9]+\\)-\\([0-9]+\\)-\\([0-9]+\\) \\([0-9]+\\):\\([0-9]+\\)\\]"


let part_one() =
  let parse_time input =
    let matches = List.map int_of_string (matcher input) in
    match matches with
      | [year; month; day; hour; minute] ->
         let (ts, tm) =
           Unix.mktime {
               tm_year = year;
               tm_mon  = month;
               tm_mday = day;
               tm_hour = hour;
               tm_min  = minute;
               tm_sec  = 0;
               (* ignored *)
               tm_isdst = false;
               tm_wday = 0;
               tm_yday = 0;
             }
         in ts
      | _ -> raise Not_found;
  in
  let stream = Lib.make_stream input_line input_file in
  let unsorted_list = Lib.list_of_stream stream in
  let sorted_list
    = List.sort
        (fun x y ->
         let [t1; t2] = List.map parse_time [x; y] in
         compare t1 t2
        )
        unsorted_list
  in
  List.iter (fun x -> Printf.printf "%s\n" x) sorted_list



let () =
  print_endline "* Day 4 *";
  part_one()
