open Lib;;

let input_file =
  "./input/day_4_input.txt"
let timestamp_regex =
  "\\[\\([0-9]+\\)-\\([0-9]+\\)-\\([0-9]+\\) \\([0-9]+\\):\\([0-9]+\\)\\]"
let action_regex =
  "\\([0-9]+\\)\\] \\(Guard #\\([0-9]+\\)\\|\\(falls asleep\\)\\|\\(wakes up\\)\\)"


let parse_and_sort input_file =
  let parse_time input =
    let matcher = Lib.regex_matches_to_list timestamp_regex in
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
  List.sort
    (fun x y ->
     let [t1; t2] = List.map parse_time [x; y] in
     compare t1 t2)
    unsorted_list

let rec make_sleep_map list ht matcher guard_id sleep_min =
  match list with
  | [] -> ht
  | hd :: tl ->
     match matcher hd with
     | [minute; _; id] ->
        if not (Hashtbl.mem ht id) then
          Hashtbl.add ht id (Hashtbl.create 60);
        make_sleep_map tl ht matcher id (-1)
     | [minute; "falls asleep"] ->
        make_sleep_map tl ht matcher guard_id (int_of_string minute)
     | [minute; "wakes up"] ->
        let sleep_ht = Hashtbl.find ht guard_id in
        for i = sleep_min to ((int_of_string minute) - 1) do
          let value = match (Hashtbl.find_opt sleep_ht i) with
          | Some(x) -> x
          | None -> 0
          in
          Hashtbl.replace sleep_ht i (value + 1);
        done;
        make_sleep_map tl ht matcher guard_id (-1)
     | _ -> assert false

let guard_sleep_duration ht =
  Hashtbl.fold
    (fun id ht list ->
     let sum = Hashtbl.fold (fun k v sum -> sum + v) ht 0 in
     (id, sum) :: list) ht []

let find_sleepiest_minute_for_guard id ht =
  let ht = try Hashtbl.find ht id with Not_found -> assert false in
  Hashtbl.fold
    (fun minute freq max ->
     let _, max_freq = max in
     if freq > max_freq then (minute, freq) else max)
     ht ((-1), (-1))

let part_one() =
  let sorted_list = parse_and_sort input_file in
  let matcher = Lib.regex_matches_to_list action_regex in
  let ht = make_sleep_map sorted_list (Hashtbl.create 60) matcher "" (-1) in
  let sleepiest_guard_id, total_minutes
    = List.fold_left
        (fun curr max ->
         let _, x = curr in
         let _, y = max in
         if x > y then curr else max)
        ("-1", (-1))
        (guard_sleep_duration ht)
  in
  let minute, sum = find_sleepiest_minute_for_guard sleepiest_guard_id ht in
  Printf.printf "[4.1] = (%s * %d = %d)]\n" sleepiest_guard_id minute ((int_of_string sleepiest_guard_id) * minute)

let () =
  print_endline "* Day 4 *";
  part_one()
