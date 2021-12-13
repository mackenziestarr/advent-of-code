#[derive(Debug)]
struct Point {
    x: i32,
    y: i32,
}
#[derive(Debug)]
struct Segment {
    start: Point,
    end: Point,
}


fn parse(input: &str) -> Vec<Segment> {
    input.lines()
        .map(|line| {
            // TODO replace with ?
            let (start, end) = line.trim().split_once(" -> ").unwrap();
            let (x1, y1) = start.split_once(',').unwrap();
            let (x2, y2) = end.split_once(',').unwrap();
            Segment {
                start: Point {x: x1.parse().unwrap(), y: y1.parse().unwrap()},
                end:   Point {x: x2.parse().unwrap(), y: y2.parse().unwrap()},
            }
        }).collect()
}

#[test]
fn test_part_one() {
    let example_input =
        "0,9 -> 5,9
         8,0 -> 0,8
         9,4 -> 3,4
         2,2 -> 2,1
         7,0 -> 7,4
         6,4 -> 2,0
         0,9 -> 2,9
         3,4 -> 1,4
         0,0 -> 8,8
         5,5 -> 8,2";
    let input = parse(example_input);
    assert_eq!(part_one(&input), 5);
}

fn part_one(input: &[Segment]) -> u32 {
    2
}

fn main() {
}